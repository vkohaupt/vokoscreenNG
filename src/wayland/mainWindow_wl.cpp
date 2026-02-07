#include "mainWindow_wl.h"
#include "QvkInformation_wl.h"
#include "QvkImageFromTabs_wl.h"
#include "QvkShowMessage_wl.h"
#include "QvkCameraController_wl.h"
#include "QvkAudioController_wl.h"
#include "QvkConvert_mkv_mp4_wl.h"
#include "QvkConvert_mkv_gif_wl.h"
#include "QvkConvert_mkv_to_webm_wl.h"
#include "QvkConvert_mkv_repair_wl.h"

#include "global.h"
#include "QvkLicenses.h"
#include "qvkdirdialog.h"
#include "QvkFileDialog.h"

#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QThread>
#include <QMessageBox>
#include <QDebug>
#include <QStyleFactory>
#include <QLibraryInfo>
#include <QTimer>
#include <QFile>
#include <QToolButton>
#include <QScreen>
#include <QList>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QProcess>
#include <QTimer>
#include <QImageWriter>

// Snapshot
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QVariantMap>
#include <QDesktopServices>
#include <QMessageBox>
#include <QPixmap>
#include <QUrl>
#include <QFileInfo>
// Snapshot

// Audio
#include <pulse/pulseaudio.h>
// Audio

QvkMainWindow_wl::QvkMainWindow_wl( QWidget *parent, Qt::WindowFlags f )
    : QMainWindow(parent, f)
    , ui(new Ui::formMainWindow_wl)
{
    bool bo = translator.load( QLocale::system().name(), ":/language" );
    if ( bo == true ) {
        qApp->installTranslator( &translator );
    }

    bo = qtTranslator.load( "qt_" + QLocale::system().name(), QLibraryInfo::path( QLibraryInfo::TranslationsPath ) );
    if ( bo == true ) {
        qApp->installTranslator( &qtTranslator );
    }

    ui->setupUi( this );

    supportedImageFormats();

    QFile fileCSS( ":/pictures/css/css.qss" );
    bool boCSS = fileCSS.open( QFile::ReadOnly | QFile::Text );
    if ( boCSS == false ) {
        qDebug().noquote() << global::nameOutput << "Can not open" << fileCSS.fileName();
    }
    QTextStream streamCSS( &fileCSS );
    qApp->setStyleSheet( streamCSS.readAll() );
    fileCSS.close();

    // Sidebar
    connect( ui->toolButtonScreencast, &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(0); } );
    connect( ui->toolButtonCamera,     &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(1); } );
    connect( ui->toolButtonSnapshot,   &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(2); } );
    connect( ui->toolButtonConvert,    &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(3); } );
    connect( ui->toolButtonLog,        &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(4);
        ui->pushButton_log_refresh->click();
    } );
    ui->tabWidgetSideBar->tabBar()->hide();

    ui->toolButtonCamera->hide();  // QvkCameraController_wl wurde auch deaktiviert ----------------------------

    set_WindowTitle();
    ui->tabWidgetScreencast->setCurrentIndex( 0 );
    ui->tabWidgetSideBar->setCurrentIndex( 0 );
    get_system_info();
    set_CornerWidget();
    set_SpezialSliders();
    QvkInformation_wl *vkInformation = new QvkInformation_wl( this, ui );
    connect(this, &QvkMainWindow_wl::signal_newVideoFilename, vkInformation, [vkInformation](QString filename){vkInformation->slot_newVideoFilename(filename);});
    connect(this, &QvkMainWindow_wl::signal_beginRecordTime,  vkInformation, [vkInformation](QString beginTime){vkInformation->slot_beginRecordTime(beginTime);});

    vkRegionMargins_wl = new QvkRegionMargins_wl( ui );

    set_RegionChoice();
    set_Connects();
    set_check_screencast_elements_available();
    set_check_GIF_convert_elements_available();
    set_check_MP4_convert_elements_available();
    set_check_WEBM_convert_elements_available();

    vkContainerController_wl = new QvkContainerController_wl( ui );

    QvkAudioController_wl *vkAudioController = new QvkAudioController_wl( ui );
    vkAudioController->init();

    connect(ui->toolButtonSeparatedAudioTracksReset, &QToolButton::clicked, this, [=](){
        ui->checkBoxSeparatedAudioTracks->setCheckState( Qt::Unchecked);
    });

    vkHelp = new QvkHelp_wl( ui );
    new QvkLicenses( ui->pushButtonLicense );
    new QvkImageFromTabs_wl( this );

    // Misc
    videoFileSystemWatcher = new QFileSystemWatcher();
    connect(ui->toolButtonVideoPath, &QToolButton::clicked, this, [this](){slot_newVideoPath();});
    connect(ui->lineEditVideoPath, &QLineEdit::textChanged, this, [this](){slot_videoFileSystemWatcherSetNewPath();});
    connect(ui->toolButtonVideoPathReset, &QToolButton::clicked, this, [this](){
        ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    });

    // About
    ui->labelSourcecodeUrl->setText( "<a href='https://github.com/vkohaupt/vokoscreenNG'>" + tr( "Sourcecode" ) + "</a>" );
    ui->labelWebSiteUrl->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + tr( "Homepage" ) + "</a>" );
    ui->labelLanguageUrl->setText( "<a href='https://app.transifex.com/vkohaupt/vokoscreen/'>" + tr( "Translations" ) + "</a>" );
    ui->labelDonateUrl->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen-donate.html'>" + tr( "Donate" ) + "</a>" );

    ui->help_screencast_audiocodec->hide();
    ui->help_screencast_audio->hide();
    ui->help_screencast_frames->hide();
    ui->help_screencast_nomousecursor->hide();
    ui->help_screencast_format->hide();
    ui->help_screencast_videocodec->hide();
    ui->frame_cisco->hide();
    ui->line_cisco->hide();
    ui->label_Upate_tab_2->hide();

    // new QvkCameraController_wl( ui ); //------------------------------------------------------------------------
    new QvkConvert_mkv_mp4_wl( this, ui );
    new QvkConvert_mkv_gif_wl( this, ui );
    new QvkConvert_mkv_to_webm_wl( this, ui );
    new QvkConvert_mkv_repair_wl( this, ui );

    QList<QScreen *> screen = QGuiApplication::screens();
    if ( !screen.empty() ) {
        for ( int i = 0; i < screen.size(); i++ ) {
            if ( screen.at(i)->name() == QGuiApplication::primaryScreen()->name() ) {
                qDebug().noquote() << global::nameOutput << "This screen is the primary screen: " << QGuiApplication::primaryScreen()->name();
            } else {
                qDebug().noquote() << global::nameOutput << "Name from screen: " << screen.at(i)->name();
            }
            qDebug().noquote() << global::nameOutput << "DevicePixelRatio:" << screen.at(i)->devicePixelRatio() << " (Normal displays is 1, Retina display is 2)";
            qDebug().noquote() << global::nameOutput << "myDevicePixelRatio:" << vkRegionChoise_wl->myDevicePixelRatio();
            qDebug().noquote() << global::nameOutput << "Screen Resolution width :" << screen.at(i)->geometry().width();
            qDebug().noquote() << global::nameOutput << "Screen Resolution height :" << screen.at(i)->geometry().height();
            qDebug().noquote() << global::nameOutput << "Vertical refresh rate of the screen in Hz:" << screen.at(i)->refreshRate();
            qDebug().noquote() << global::nameOutput << "Screen orientation" << screen.at(i)->orientation();
            qDebug().noquote() << global::nameOutput << "Color depth of the screen: " << screen.at(i)->depth();
            qDebug().noquote() << global::nameOutput << "Model from screen: " << screen.at(i)->model();
            qDebug().noquote() << global::nameOutput << "Manufactur from screen: " << screen.at(i)->manufacturer();
            qDebug().noquote() << global::nameOutput << "SerialNumber from screen: " << screen.at(i)->serialNumber();
        }
    }

    vkSystray = new QvkSystray_wl( ui );
    if ( QSystemTrayIcon::isSystemTrayAvailable() == true ) {
        vkSystray->init();
        connect(ui->checkBoxShowInSystray, &QCheckBox::clicked, vkSystray, [this](bool value){vkSystray->setVisible(value);});
        connect(vkSystray, &QvkSystray_wl::signal_SystemtrayIsClose, vkSystray, [this](){close();});
    }

    vkSettings_wl.readAll( ui, this );
    vkSettings_wl.readAreaScreencast( vkRegionChoise_wl );

    ui->widgetLanguageAndHelp->setVisible( false );
 }


QvkMainWindow_wl::~QvkMainWindow_wl()
{
}


void QvkMainWindow_wl::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);

    qDebug();
    qDebug().noquote() << global::nameOutput << "QvkMainWindow_wl::closeEvent Begin close";

    QList<QvkLevelMeterController_wl *> list = ui->scrollAreaAudioDevice->findChildren<QvkLevelMeterController_wl *>();
    for ( int i = 0; i < list.count(); i++ ) {
        QvkLevelMeterController_wl *vkLevelMeterController = list.at(i);
        vkLevelMeterController->vkLevelMeter->stop();
        qDebug().noquote() << global::nameOutput << "Stop levelmeter on:" << vkLevelMeterController->objectName();
    }

    ui->pushButtonStop->click();
    vkRegionChoise_wl->close();
    vkSettings_wl.saveAll( ui, this );
    vkSettings_wl.saveAreaScreencast( vkRegionChoise_wl->get_XRecordArea() / vkRegionChoise_wl->screen()->devicePixelRatio(),
                                      vkRegionChoise_wl->get_YRecordArea() / vkRegionChoise_wl->screen()->devicePixelRatio(),
                                      vkRegionChoise_wl->get_WidthRecordArea() / vkRegionChoise_wl->screen()->devicePixelRatio(),
                                      vkRegionChoise_wl->get_HeightRecordArea() / vkRegionChoise_wl->screen()->devicePixelRatio()
                                      );
    vkSystray->deleteLater();

    qDebug().noquote() << global::nameOutput << "QvkMainWindow_wl::closeEvent End close";
}


void QvkMainWindow_wl::slot_log_refresh()
{
    QFile file( vkLogController_wl->get_log_filePath() );
    bool bo = file.open( QIODevice::ReadOnly );
    if ( bo == false ) {
        qDebug().noquote() << global::nameOutput << "[QvkMainWindow_wl::slot_log_refresh] Can not open" << file.fileName();
        return;
    }
    QTextStream in( &file );
    ui->textBrowser->clear();
    ui->textBrowser->setText( in.readAll() );
    ui->textBrowser->moveCursor( QTextCursor::End ) ;
}


QString QvkMainWindow_wl::get_pipewire_version()
{
    if ( isFlatpak == true ) {
        QString app = "flatpak-spawn";
        QString version = "";
        QProcess process;
        process.setProcessChannelMode( QProcess::MergedChannels );
        process.start( app,  QStringList() << "--host" << "/usr/bin/pipewire" << "--version" );
        if ( process.waitForFinished( 30000 ) ) {
            QString text( process.readAll() );
            QStringList list = text.split( "\n" );
            version = list.at(1).section( " ", 3 );
        }
        return version;
    } else {
        QString app = "pipewire";
        QString version = "";
        QProcess process;
        process.setProcessChannelMode( QProcess::MergedChannels );
        process.start( app,  QStringList() << "--version" );
        if ( process.waitForFinished( 30000 ) ) {
            QString text( process.readAll() );
            QStringList list = text.split( "\n" );
            version = list.at(1).section( " ", 3 );
        }
        return version;
    }
}


QString QvkMainWindow_wl::get_KDE_Version()
{
    QString version;

    QString desktop = qgetenv( "XDG_CURRENT_DESKTOP" );
    if ( desktop == "KDE") {
        version = qgetenv( "KDE_SESSION_VERSION" );
    }

    return version;
}


void QvkMainWindow_wl::set_WindowTitle()
{
    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );
}


void QvkMainWindow_wl::get_system_info()
{
    qDebug().noquote() << global::nameOutput << "Version:" << global::version;
    qDebug().noquote() << global::nameOutput << "Locale:" << QLocale::system().name();
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug().noquote() << global::nameOutput << "Log from:" << dateTime.toString( "yyyy-MM-dd hh:mm:ss" );
    QLocale locale;
    qDebug().noquote() << global::nameOutput << "Country:" << QLocale::territoryToString( locale.territory() );
    qDebug().noquote() << global::nameOutput << "Qt:" << qVersion();
    qDebug().noquote() << global::nameOutput << gst_version_string();
    qDebug().noquote() << global::nameOutput << "PulseAudio library version:" << pa_get_library_version();
    if ( QSysInfo::prettyProductName().contains( "Flatpak" ) ) {
        isFlatpak = true;
    }
    qDebug().noquote() << global::nameOutput << "Pipewire version:" << get_pipewire_version();
    qDebug().noquote() << global::nameOutput << "Operating system:" << QSysInfo::prettyProductName();
    qDebug().noquote() << global::nameOutput << "CPU Architecture:" << QSysInfo::currentCpuArchitecture();
    qDebug().noquote() << global::nameOutput << "Count CPU:" << QThread::idealThreadCount();
    qDebug().noquote() << global::nameOutput << global::name << "running as:" << QGuiApplication::platformName() << "client";
    qDebug().noquote() << global::nameOutput << global::name << "running on:" << qgetenv( "XDG_SESSION_TYPE" ).toLower();
    qDebug().noquote() << global::nameOutput << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    if ( qgetenv( "XDG_CURRENT_DESKTOP" ) == "KDE" ) {
        qDebug().noquote() << global::nameOutput << "KDE Version:" << get_KDE_Version();
        if ( get_KDE_Version() >= "6" ) {
            ui->radioButtonScreencastFullscreen->setText( ui->radioButtonScreencastFullscreen->text() +
                                                          " / " +
                                                          ui->radioButtonScreencastArea->text() );
            ui->radioButtonScreencastArea->hide();
            ui->frame_area->hide();
            ui->toolButtonScreencastAreaReset->hide();
        }
    }
    if ( QSystemTrayIcon::isSystemTrayAvailable() == true ) {
        qDebug().noquote() << global::nameOutput << "Systray: Supported";
    } else {
        qDebug().noquote() << global::nameOutput << "Systray: Not supported";
    }
    qDebug().noquote() << global::nameOutput << "Icon-Theme:" << QIcon::themeName();
    qDebug().noquote() << global::nameOutput << "Styles:" << QStyleFactory::keys();
    qDebug().noquote() << global::nameOutput << "Qt-PluginsPath:     " << QLibraryInfo::path( QLibraryInfo::PluginsPath );
    qDebug().noquote() << global::nameOutput << "Qt-TranslationsPath:" << QLibraryInfo::path( QLibraryInfo::TranslationsPath );
    qDebug().noquote() << global::nameOutput << "Qt-LibraryPath:     " << QLibraryInfo::path( QLibraryInfo::LibrariesPath );
    qDebug().noquote() << global::nameOutput << "SettingsPath:" << vkSettings_wl.getFileName();
    qDebug().noquote() << global::nameOutput << "LogPath:" << vkLogController_wl->get_log_filePath();

    // Clear all settings if checkBoxResetAtNextStart is set
    QString folderName_wl = global::name;
    QString fileName_wl = global::name + "_wl";
    QString groupName_wl = global::name + "_wl";
    QSettings setting( QSettings::IniFormat, QSettings::UserScope, folderName_wl, fileName_wl, Q_NULLPTR );
    setting.beginGroup( groupName_wl );
    QList<QCheckBox *> listCheckBoxReset = ui->centralwidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBoxReset.count(); i++ ) {
        QCheckBox *checkBox = listCheckBoxReset.at(i);
        if ( ( checkBox->objectName() == "checkBoxResetAtNextStart" ) and
             ( setting.value( checkBox->objectName(), false ).toBool() == true ) )
        {
            setting.clear();
        }
    }
    setting.endGroup();

    qDebug().noquote() << global::nameOutput << "Default Videopath:" << QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
    if ( vkSettings_wl.getVideoPath().isEmpty() == true ) {
        qDebug().noquote() << global::nameOutput << "User Videopath:" << QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );;
    } else {
        qDebug().noquote() << global::nameOutput << "User Videopath:" << vkSettings_wl.getVideoPath();
    }
    qDebug().noquote() << global::nameOutput << "Default Picturepath:" << QStandardPaths::writableLocation( QStandardPaths::PicturesLocation );
    if ( vkSettings_wl.getPicturePath().isEmpty() == true ) {
        qDebug().noquote() << global::nameOutput << "User Picturepath:" << QStandardPaths::writableLocation( QStandardPaths::PicturesLocation );;
    } else {
        qDebug().noquote() << global::nameOutput << "User Picturepath:" << vkSettings_wl.getPicturePath();
    }
    qDebug().noquote();
}


void QvkMainWindow_wl::set_CornerWidget()
{
    QList<QTabWidget *> list;
    list << ui->tabWidgetScreencast;
    list << ui->tabWidgetCamera;
    list << ui->tabWidgetSnapshot;
    list << ui->tabWidgetConvert;
    list << ui->tabWidgetLog;

    QString cornerPicture;
    if ( isFlatpak == true ) {
        cornerPicture = ":/pictures/cornerWidget/wayland-flatpak.png";
    } else {
        cornerPicture = ":/pictures/cornerWidget/wayland.png";
    }

    QPixmap pixmap( cornerPicture );
    pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

    for ( int i = 0; i < list.count(); i++ ) {
        QLabel *label = new QLabel();
        label->setPixmap( pixmap );
        label->setEnabled( false );
        QTabWidget *tabWidget = list.at( i );
        tabWidget->setCornerWidget( label, Qt::TopRightCorner );
    }
}


void QvkMainWindow_wl::set_Connects()
{
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->pushButtonStart->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->radioButtonScreencastFullscreen->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->radioButtonScreencastWindow->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->radioButtonScreencastArea->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->frameVideoPath->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->frame_area->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->toolButtonScreencastAreaReset->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->frame_video->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->frame_audio->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){ui->frame_3->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, vkRegionMargins_wl, [=](){vkRegionMargins_wl->whatWasClicked = "pushButtonStart";});
    connect(ui->pushButtonStart, &QPushButton::clicked, this,               [=](){whatWasClicked = "pushButtonStart";});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){slot_pre_portal_start();});
    connect(vkRegionMargins_wl,  &QvkRegionMargins_wl::signal_regionMargins, this, [=](){slot_portal_start();});

    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){slot_stop();});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->pushButtonStop->setDisabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->pushButtonStart->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->pushButtonPause->setDisabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->radioButtonScreencastFullscreen->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->radioButtonScreencastWindow->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->radioButtonScreencastArea->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->frameVideoPath->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->frame_video->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->frame_audio->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){ui->frame_3->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){
        if (ui->radioButtonScreencastArea->isChecked() == true){
            ui->frame_area->setEnabled(true);
            ui->toolButtonScreencastAreaReset->setEnabled(true);
        };
    });
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){portal_wl->slot_stopScreenCast();});

    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){slot_Pause();});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){ui->pushButtonPause->hide();});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){ui->pushButtonContinue->show();});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){ui->pushButtonStop->setDisabled(true);});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){ui->pushButtonContinue->setEnabled(true);});

    connect(ui->pushButtonContinue, &QPushButton::clicked, this, [=](){slot_Continue();});
    connect(ui->pushButtonContinue, &QPushButton::clicked, this, [=](){ui->pushButtonContinue->setDisabled(true);});
    connect(ui->pushButtonContinue, &QPushButton::clicked, this, [=](){ui->pushButtonContinue->hide();});
    connect(ui->pushButtonContinue, &QPushButton::clicked, this, [=](){ui->pushButtonPause->show();});
    connect(ui->pushButtonContinue, &QPushButton::clicked, this, [=](){ui->pushButtonStop->setEnabled(true);});
    ui->pushButtonContinue->hide();

    connect(ui->pushButtonOpenfolder, &QPushButton::clicked, this, [=](){slot_folder();});

    connect(ui->radioButtonScreencastFullscreen, &QRadioButton::clicked, this, [=](){ui->toolButtonScreencastAreaReset->setDisabled(true);});
    connect(ui->radioButtonScreencastWindow,     &QRadioButton::clicked, this, [=](){ui->toolButtonScreencastAreaReset->setDisabled(true);});
    connect(ui->radioButtonScreencastArea,       &QRadioButton::clicked, this, [=](){ui->toolButtonScreencastAreaReset->setEnabled(true);});

    connect(portal_wl, &Portal_wl::signal_portal_fd_path, this, [=](QString vk_fd, QString vk_path){slot_pre_start(vk_fd,vk_path);});
    connect(portal_wl, &Portal_wl::signal_portal_aborted, this, [=](){slot_portal_dialog_aborted();});

    connect(ui->toolButtonScreencastAreaReset, &QToolButton::clicked, this, [=](){vkRegionChoise_wl->slot_areaReset();});
    connect(ui->toolButtonFramesReset,         &QToolButton::clicked, this, [=](){sliderFrames->setValue(25);});
    connect(ui->toolButtonOpenh264Reset,       &QToolButton::clicked, this, [=](){sliderOpenh264->setValue(23);});

    connect(ui->pushButtonSnapshot, &QPushButton::clicked, vkRegionMargins_wl, [=](){vkRegionMargins_wl->whatWasClicked = "pushButtonSnapshot";});
    connect(ui->pushButtonSnapshot, &QPushButton::clicked, this,               [=](){whatWasClicked = "pushButtonSnapshot";});
    connect(ui->pushButtonSnapshot, &QPushButton::clicked, this,               [=](){slot_snapshotHideBeforeRecording();});
    connect(ui->pushButtonSnapshotOpenFolder, &QPushButton::clicked, this,     [=](){slot_path_to_snapshot_folder();});

    connect(ui->toolButtonSnapshotFormatsReset, &QPushButton::clicked, this, [=](){
        ui->comboBoxSnapshotImageFormats->setCurrentText("png");});
    connect(ui->toolButtonSnapshotHideBeforeRecordingReset, &QPushButton::clicked, this, [=](){
        ui->checkBoxSnapshotHideBeforeRecording->setChecked(false);});
    QvkSpezialSlider *spezialSlider = ui->centralwidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
    connect(ui->toolButtonSnapshotSecondsWaitBeforeRecordingReset, &QPushButton::clicked, this, [=](){
        spezialSlider->setValue(4);});

    connect(ui->pushButton_log_openfolder, &QPushButton::clicked, this, [=](){slot_log_folder();});
    connect(ui->pushButton_log_refresh,    &QPushButton::clicked, this, [=](){slot_log_refresh();});
}


void QvkMainWindow_wl::slot_snapshotHideBeforeRecording() {
    if ( ui->checkBoxSnapshotHideBeforeRecording->isChecked() == true ) {
        qDebug().noquote() << global::nameOutput << "[Snapshot]" << "Hide this window is checked";
        showMinimized();
        hide();
    } else {
        qDebug().noquote() << global::nameOutput << "[Snapshot]" << "Hide this window is not checked";
    }
    QvkSpezialSlider *spezialSlider = ui->centralwidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
    QTimer::singleShot( spezialSlider->value() * 100, Qt::PreciseTimer, this, [=](){slot_pushButton_snapshot();});
}


void QvkMainWindow_wl::slot_pushButton_snapshot()
{
    // https://pythonhosted.org/txdbus/dbus_overview.html
    // https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.portal.Screenshot.html#
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *i = new QDBusInterface("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", "org.freedesktop.portal.Screenshot", bus, NULL);

    QVariantMap options;
    if ( ui->checkBox_snapshot_interactive->isChecked() == false ) {
        options["interactive"] = false;
    } else {
        options["interactive"] = true;
    }
    QDBusReply<QDBusObjectPath> reply = i->call( "Screenshot", "", options );

    if( reply.isValid() ) {
        bus.connect( "", reply.value().path(), "org.freedesktop.portal.Request", "Response", this, SLOT( slot_handle_response_snapshot(uint,QVariantMap) ) );
        qDebug().noquote() << global::nameOutput << "[Snapshot]" << reply.value().path();
    } else {
        qDebug().noquote() << global::nameOutput << "[Snapshot] Something is wrong: " << reply.error();
    }
}


void QvkMainWindow_wl::slot_handle_response_snapshot( uint responseCode, QVariantMap results )
{
    if ( responseCode == 0 ) {
        QUrl url( results["uri"].toString() );
        QFileInfo fileInfo( url.toLocalFile() );
        path_to_snapshot_folder = fileInfo.absolutePath();
        QString filePath_org = fileInfo.absoluteFilePath();

        if ( filePath_org > "" ) {
            if ( whatWasClicked == "pushButtonSnapshot" ) {
                QPixmap pixmap = QPixmap( filePath_org );
                QString filePath_new = path_to_snapshot_folder +
                        "/" +
                        global::name +
                        "-" +
                        QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss.zzz" ) +
                        "." +
                        ui->comboBoxSnapshotImageFormats->currentText().toUtf8();
                pixmap.save( filePath_new );

                QFile file( filePath_org );
                file.remove();

                if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                    QvkShowMessage_wl *vkShowMessage_wl = new QvkShowMessage_wl();
                    vkShowMessage_wl->set_StatusIcon( ":/pictures/status/information.png" );
                    vkShowMessage_wl->set_Image( filePath_new );
                    vkShowMessage_wl->set_timeOut( 10000 );
                    vkShowMessage_wl->showMessage( "" );
                    vkShowMessage_wl->set_WindowTitle( "Snapshot" );
                    vkShowMessage_wl->set_folderPath( path_to_snapshot_folder );
                }

                qDebug().noquote() << global::nameOutput << "[Snapshot] Saved under:" << filePath_new;
            }
        }
    } else {
        qDebug().noquote() << global::nameOutput << "[Snapshot] Unable to take a screenshot" << results["uri"];
    }

    show();
}


void QvkMainWindow_wl::slot_path_to_snapshot_folder()
{
    if ( path_to_snapshot_folder == "" ) {
        path_to_snapshot_folder = QStandardPaths::writableLocation( QStandardPaths::PicturesLocation );
    }

    if ( QDesktopServices::openUrl( QUrl( "file:///" + path_to_snapshot_folder, QUrl::TolerantMode ) ) == false ) {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}


void QvkMainWindow_wl::supportedImageFormats()
{
    QList<QByteArray> listFormats = QImageWriter::supportedImageFormats();
    if ( listFormats.empty() == false ) {
        for ( int x = 0; x < listFormats.count(); x++ ) {
            ui->comboBoxSnapshotImageFormats->addItem( QString( listFormats.at(x) ) );
        }
    }

    ui->comboBoxSnapshotImageFormats->setCurrentIndex( ui->comboBoxSnapshotImageFormats->findText( "png" ) );
}

//------------------------------------------------
//             pushButtonStart
//                   |
//            slot_pre_portal_start
//                   |
//            slot_portal_start
//                   |
//     portal_wl->slot_startScreenCast
//                   |
//           "Show Portal Dialog"
//             |             |
//          Accepted       Cancel
//             |             |
//      slot_pre_start  slot_portal_dialog_aborted
//             |
//         Countdown
//          |     |
//     Accepted Cancel
//        |
//
//------------------------------------------------

void QvkMainWindow_wl::slot_pre_portal_start()
{
    qDebug().noquote() << global::nameOutput << "[QvkMainWindow_wl] slot_pre_portal_start()";

    if ( ui->radioButtonScreencastArea->isChecked() ) {
        vkRegionMargins_wl->setVisible(true);
        vkRegionMargins_wl->slot_singleShot(true);
    }

    if ( ui->radioButtonScreencastWindow->isChecked() ) {
        slot_portal_start();
    }

    if ( ui->radioButtonScreencastFullscreen->isChecked() ) {
        slot_portal_start();
    }
}


void QvkMainWindow_wl::slot_portal_start()
{
    // Value 1 = MONITOR
    // Value 2 = WINDOW
    uint sourceType = 1;
    if ( ui->radioButtonScreencastFullscreen->isChecked() ) {
        qDebug();
        qDebug().noquote() << global::nameOutput << "Start portal fullscreen";
        sourceType = 1;
    }

    if ( ui->radioButtonScreencastWindow->isChecked() ) {
        qDebug();
        qDebug().noquote() << global::nameOutput << "Start portal window";
        sourceType = 2;
    }

    if ( ui->radioButtonScreencastArea->isChecked() ) {
        qDebug();
        qDebug().noquote() << global::nameOutput << "Start portal area";
        sourceType = 1;
    }

    // Value 1 = HIDDEN;
    // Value 2 = EMBEDDED;
    uint mousecursorONOff = 2;
    if ( ui->checkBoxMouseCursorOnOff->isChecked() == true ) {
        qDebug().noquote() << global::nameOutput << "Mouse cursor is not recording";
        mousecursorONOff = 1;
    }

    if ( ui->checkBoxMouseCursorOnOff->isChecked() == false ) {
        qDebug().noquote() << global::nameOutput << "Mouse cursor is recording";
        mousecursorONOff = 2;
    }

    portal_wl->slot_startScreenCast( sourceType, mousecursorONOff );
}


QString QvkMainWindow_wl::get_Area_Videocrop()
{
    QString videocrop = "";
    vkRegionChoise_wl->set_recordMode( true );
    int divTop = 0;
    int divRight = 0;
    int divBottom = 0;
    int divLeft = 0;

    divTop    = vkRegionMargins_wl->get_top();
    divRight  = vkRegionMargins_wl->get_right();
    divBottom = vkRegionMargins_wl->get_bottom();
    divLeft   = vkRegionMargins_wl->get_left();

    QString top    = QString::number( vkRegionChoise_wl->get_YRecordArea() + divTop );
    QString right  = QString::number( vkRegionChoise_wl->get_width_from_window() - ( vkRegionChoise_wl->get_XRecordArea() + vkRegionChoise_wl->get_WidthRecordArea() - divRight ) );
    QString bottom = QString::number( vkRegionChoise_wl->get_height_from_window() - ( vkRegionChoise_wl->get_YRecordArea() + vkRegionChoise_wl->get_HeightRecordArea() - divBottom ) );
    QString left   = QString::number( vkRegionChoise_wl->get_XRecordArea() + divLeft );
    videocrop = "videocrop top=" + top + " " + "right=" + right + " " + "bottom=" + bottom + " " + "left=" + left;

    qDebug().noquote() << global::nameOutput << "Area crop from the screen"
                       << vkRegionChoise_wl->screen()->name() + ","
                       << vkRegionChoise_wl->screen()->manufacturer() + ","
                       << vkRegionChoise_wl->screen()->model() + ","
                       << QString::number( vkRegionChoise_wl->screen()->size().width() ) + "x" + QString::number( vkRegionChoise_wl->screen()->size().height() ) + videocrop;

    return videocrop;
}


void QvkMainWindow_wl::slot_pre_start( QString vk_fd, QString vk_path )
{
    if ( sliderScreencastCountDown->value() > 0 ) {
        QvkCountdown_wl vkCountdown_wl;
        vkCountdown_wl.timer->start();
        vkCountdown_wl.animationTimer->start();
        vkCountdown_wl.countValue = sliderScreencastCountDown->value();
        int ret = vkCountdown_wl.exec();

        if ( ret == QDialog::Accepted ) {
            if ( QSystemTrayIcon::isSystemTrayAvailable() == true ) {
                vkSystray->startAction->setDisabled(true);
                vkSystray->stopAction->setEnabled(true);
                vkSystray->pauseAction->setEnabled(true);
                vkSystray->continueAction->setDisabled(true);
                vkSystray->slot_setRecordIcon();
            }
            slot_start_gst( vk_fd, vk_path );
        }

        if ( ret == QDialog::Rejected ) {
            ui->pushButtonStart->setEnabled( true );
            ui->pushButtonStop->setDisabled( true );
            ui->radioButtonScreencastFullscreen->setEnabled( true );
            ui->radioButtonScreencastWindow->setEnabled( true );
            ui->radioButtonScreencastArea->setEnabled( true );
            ui->toolButtonScreencastAreaReset->setEnabled( true );
            ui->frameVideoPath->setEnabled( true );
            ui->frame_video->setEnabled( true );
            ui->frame_audio->setEnabled( true );
            ui->frame_3->setEnabled( true );

            if ( QSystemTrayIcon::isSystemTrayAvailable() == true ) {
                vkSystray->startAction->setEnabled(true);
                vkSystray->stopAction->setDisabled(true);
                vkSystray->pauseAction->setDisabled(true);
                vkSystray->continueAction->setDisabled(true);
                vkSystray->slot_setSystrayIcon();
            }
            portal_wl->slot_stopScreenCast();
        }

    } else {
        slot_start_gst( vk_fd, vk_path );
        if ( QSystemTrayIcon::isSystemTrayAvailable() == true ) {
            vkSystray->startAction->setDisabled(true);
            vkSystray->stopAction->setEnabled(true);
            vkSystray->pauseAction->setEnabled(true);
            vkSystray->continueAction->setDisabled(true);
        }
        vkSystray->slot_setRecordIcon();    }
}


QString QvkMainWindow_wl::Pipeline_structured_output( QString pipeline )
{
    QString string;
    QString nl;

    nl = "\\";
    string = pipeline.prepend( "gst-launch-1.0 -e " + nl + "\n    " );

    string = pipeline.replace( "mux.", "mux. " + nl + "\n   " );
    string = pipeline.replace( "mix.", "mix. " + nl + "\n   " );
    string = pipeline.replace( "!", nl + "\n        !" );
    string.append( "\n" );
    return string;
}


QStringList QvkMainWindow_wl::VK_getSelectedAudioDevice()
{
    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        if ( checkBox->checkState() == Qt::Checked ) {
            list << checkBox->accessibleName();
        }
    }
    return list;
}


QMessageBox *msgBox;
//---------------------------------------------------------------------------------------------------
GstBusSyncReply QvkMainWindow_wl::call_bus_message( GstBus *bus, GstMessage *message, gpointer user_data )
{
    Q_UNUSED(bus);
    Q_UNUSED(user_data)
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_ERROR:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_ERROR";
            break;
        case GST_MESSAGE_EOS:
        {qDebug().noquote() << global::nameOutput << "GST_MESSAGE_EOS";
            msgBox->setText("The document has been modified.");
            msgBox->exec();
            break; }
        case GST_MESSAGE_DURATION_CHANGED:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_DURATION_CHANGED";
            break;
        case GST_MESSAGE_STEP_DONE:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_STEP_DONE";
            break;
        case GST_MESSAGE_TAG:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_TAG";
            break;
        case GST_MESSAGE_STATE_CHANGED:
            //qDebug().noquote() << global::nameOutput << "GST_MESSAGE_STATE_CHANGED";
            break;
        case GST_MESSAGE_STREAM_START:
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_STREAM_START";
            break;
        case GST_MESSAGE_APPLICATION:
            {
            qDebug().noquote() << global::nameOutput << "GST_MESSAGE_APPLICATION";
            break;
            }
        default:
            break;
    }

    return GST_BUS_PASS;
}


void QvkMainWindow_wl::slot_start_gst( QString vk_fd, QString vk_path )
{
    ui->pushButtonStop->setEnabled( true );
    ui->pushButtonPause->setEnabled( true );

    QThread::msleep( static_cast<unsigned long>( sliderSecondWaitBeforeRecording->value()) * 1000 );
    qDebug().noquote() << global::nameOutput << "SecondWaitBeforeRecording:" << sliderSecondWaitBeforeRecording->value();
    qDebug().noquote();

    QStringList stringList;
    stringList << QString( "pipewiresrc fd=" ).append( vk_fd ).append( " path=" ).append( vk_path ).append( " do-timestamp=true" );
    stringList << "videoconvert";
    stringList << "videorate";
    stringList << "queue max-size-bytes=1073741824 max-size-time=10000000000 max-size-buffers=1000";
    if ( ui->radioButtonScreencastArea->isChecked() ) { stringList << get_Area_Videocrop(); }
    stringList << "video/x-raw, framerate=" + QString::number( sliderFrames->value() ) + "/1";

    QString value;
    QStringList list;
    list << "openh264enc" ;
    list << "qp-min=" + QString::number( sliderOpenh264->value() );
    list << "qp-max=" + QString::number( sliderOpenh264->value() );
    list << "usage-type=camera"; // We need camera not screen. With screen and a fast sequence of images the video jerks.
    list << "complexity=low";
    list << "multi-thread=" + QString::number( 0 );
    list << "slice-mode=auto"; // Number of slices equal to number of threads
    value = list.join( " " );
    value.append( " ! h264parse" );
    stringList << value;

    // Only if one or more audiodevice is selected
    if ( ( VK_getSelectedAudioDevice().count() > 0 ) and ( ui->comboBoxAudioCodec->count() > 0 ) ) {
        stringList << "queue";
        stringList << "mux.";
    }

    // Pipeline for one selected audiodevice
    if ( ( VK_getSelectedAudioDevice().count() == 1 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        stringList << "pulsesrc device=" + VK_getSelectedAudioDevice().at(0);
        stringList << "audioconvert";
        stringList << "audiorate";
        stringList << "audio/x-raw, channels=2";
        stringList << "queue max-size-bytes=1000000 max-size-time=10000000000 max-size-buffers=1000";
        stringList << ui->comboBoxAudioCodec->currentData().toString();
        stringList << "queue";
        stringList << "mux.";
    }

    // Pipeline for more as one audiodevice
    if ( ( VK_getSelectedAudioDevice().count() > 1 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        if ( ui->checkBoxSeparatedAudioTracks->isChecked() == false ) {
            for ( int x = 0; x < VK_getSelectedAudioDevice().count(); x++ )
            {
                stringList << "pulsesrc device=" + VK_getSelectedAudioDevice().at(x);
                stringList << "audioconvert";
                stringList << "audioresample";
                stringList << "queue";
                stringList << "mix.";
            }
            stringList << "audiomixer name=mix";
            stringList << "audioconvert";
            stringList << "audiorate";
            stringList << "queue";
            stringList << ui->comboBoxAudioCodec->currentData().toString();
            stringList << "queue";
            stringList << "mux.";
        } else {
            for ( int x = 0; x < VK_getSelectedAudioDevice().count(); x++ ) {
                stringList << "pulsesrc device=" + VK_getSelectedAudioDevice().at(x);
                stringList << "audio/x-raw,channels=2";
                stringList << "audioconvert";
                stringList << "audioresample";
                stringList << "queue";
                stringList << ui->comboBoxAudioCodec->currentData().toString();
                stringList << "mux.";
            }
        }
    }

    stringList << "matroskamux name=mux writing-app=" + global::name + "_" + QString( global::version ).replace( " ", "_" );
    stringList.removeAll( "" );

    QString newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + ".mkv";
    stringList << "filesink location=\"" + ui->lineEditVideoPath->text() + "/" + newVideoFilename + "\"";

    QString VK_Pipeline = stringList.join( " ! " );
    VK_Pipeline = VK_Pipeline.replace( "mix. !", "mix." );
    VK_Pipeline = VK_Pipeline.replace( "mux. !", "mux." );

    qDebug().noquote() << global::nameOutput << "Free disk space at the beginning of the recording:" << ui->labelFreeSize->text() << "MB";
    qDebug();
    qDebug().noquote() << global::nameOutput << "Separated audio tracks:" << ui->checkBoxSeparatedAudioTracks->isChecked();
    qDebug();
    qDebug().noquote() << global::nameOutput << "Start record with:" << VK_Pipeline;
    qDebug();
    qDebug().noquote() << Pipeline_structured_output( VK_Pipeline );

    QByteArray byteArray = VK_Pipeline.toUtf8();
    const gchar *line = byteArray.constData();
    GError *error = Q_NULLPTR;
    pipeline = gst_parse_launch( line, &error );

    // Da ist irgendwo ein Bug, stürzt bei STOP ab
//    GstBus *bus = gst_pipeline_get_bus( GST_PIPELINE ( pipeline ) );
//    gst_bus_set_sync_handler( bus, (GstBusSyncHandler)call_bus_message, this, NULL );
//    gst_object_unref( bus );

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
    if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
    if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
    if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_ASYNC"   << "Returncode =" << ret;   } // 2
    if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3
    if ( ret == GST_STATE_CHANGE_FAILURE )
    {
        qDebug().noquote() << global::nameOutput << "Unable to set the pipeline to the playing state.";
        gst_object_unref( pipeline );
        return;
    }

    emit signal_beginRecordTime( QTime::currentTime().toString( "hh:mm:ss" ) );
    emit signal_newVideoFilename( newVideoFilename );
}

/*
21:34:40 gst-launch-1.0 -e \
    ximagesrc display-name=:0 use-damage=false show-pointer=true startx=0 starty=0 endx=1919 endy=1079 \
        ! video/x-raw, framerate=25/1 \
        ! videoconvert \
        ! videorate \
        ! queue max-size-bytes=1073741824 max-size-time=10000000000 max-size-buffers=1000 \
        ! openh264enc qp-min=23 qp-max=23 usage-type=camera complexity=low multi-thread=4 slice-mode=auto \
        ! h264parse \
        ! queue \
        ! mux. \
    pulsesrc device=alsa_output.pci-0000_00_1b.0.analog-stereo.monitor client-name=[vokoscreenNG]. \
        ! audioconvert \
        ! audioresample \
        ! queue \
        ! mix. \
    pulsesrc device=alsa_input.usb-046d_0809_A6307261-02.mono-fallback client-name=[vokoscreenNG]. \
        ! audioconvert \
        ! audioresample \
        ! queue \
        ! mix. \
    audiomixer name=mix \
        ! audioconvert \
        ! audiorate \
        ! queue \
        ! vorbisenc \
        ! queue \
        ! mux. \
    matroskamux name=mux writing-app=vokoscreenNG_4.3.0-beta-01 \
        ! filesink location="/home/vk/Videos/vokoscreenNG-2024-09-05_21-34-40.mkv"
*/

void QvkMainWindow_wl::slot_stop()
{
    // wait for EOS
    bool a = gst_element_send_event( pipeline, gst_event_new_eos() );
    Q_UNUSED(a);

    GstClockTime timeout = 5 * GST_SECOND;
    GstMessage *msg = gst_bus_timed_pop_filtered( GST_ELEMENT_BUS (pipeline), timeout, GST_MESSAGE_EOS );
    Q_UNUSED(msg);

    GstStateChangeReturn ret ;
    Q_UNUSED(ret);
    ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
    Q_UNUSED(ret);
    ret = gst_element_set_state( pipeline, GST_STATE_READY );
    Q_UNUSED(ret);
    ret = gst_element_set_state( pipeline, GST_STATE_NULL );
    Q_UNUSED(ret);
    gst_object_unref( pipeline );
    qDebug().noquote() << global::nameOutput << "Stop record";
    qDebug().noquote() << global::nameOutput << "Free disk space at the end of the recording:" << ui->labelFreeSize->text() << "MB";

    if ( ui->radioButtonScreencastArea->isChecked() ) {
       vkRegionChoise_wl->set_recordMode( false );
       vkRegionChoise_wl->repaint();
       vkRegionChoise_wl->setMask( vkRegionChoise_wl->pixmap.mask() );
    }
}


void QvkMainWindow_wl::slot_portal_dialog_aborted()
{
    ui->pushButtonStart->setEnabled( true );
    ui->pushButtonStop->setEnabled( false );
    ui->radioButtonScreencastFullscreen->setEnabled( true );
    ui->radioButtonScreencastWindow->setEnabled( true );
    ui->radioButtonScreencastArea->setEnabled( true );
    ui->frameVideoPath->setEnabled( true );
    ui->frame_video->setEnabled( true );
    ui->frame_audio->setEnabled( true );
    ui->frame_3->setEnabled( true );
    if ( ui->radioButtonScreencastArea->isChecked() == true ) {
       ui->frame_area->setEnabled( true );
       ui->toolButtonScreencastAreaReset->setEnabled( true );
    }

    if ( QSystemTrayIcon::isSystemTrayAvailable() == true ) {
        vkSystray->startAction->setEnabled(true);
        vkSystray->stopAction->setEnabled(false);
        vkSystray->pauseAction->setEnabled(false);
        vkSystray->continueAction->setEnabled(false);
        vkSystray->slot_setSystrayIcon();
    }
}


void QvkMainWindow_wl::set_SpezialSliders()
{
    sliderScreencastCountDown = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_10->insertWidget( 1, sliderScreencastCountDown );
    sliderScreencastCountDown->setObjectName( "sliderScreencastCountDown" );
    sliderScreencastCountDown->setTracking( true );
    sliderScreencastCountDown->setMinimum( 0 );
    sliderScreencastCountDown->setMaximum( 30 );
    sliderScreencastCountDown->setValue( 0 );
    sliderScreencastCountDown->setPageStep( 1 );
    sliderScreencastCountDown->show();

    sliderFrames = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_33->insertWidget( 0, sliderFrames );
    sliderFrames->setObjectName( "sliderFrames" );
    sliderFrames->setTracking( true );
    sliderFrames->setMinimum( 10 );
    sliderFrames->setMaximum( 60 );
    sliderFrames->setValue( 0 );
    sliderFrames->show();

    sliderSecondWaitBeforeRecording = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_9->insertWidget( 1, sliderSecondWaitBeforeRecording );
    sliderSecondWaitBeforeRecording->setObjectName( "sliderSecondWaitBeforeRecording" );
    sliderSecondWaitBeforeRecording->setMinimum( 0 );
    sliderSecondWaitBeforeRecording->setMaximum( 3 );
    sliderSecondWaitBeforeRecording->setValue( 1 );
    sliderSecondWaitBeforeRecording->setPageStep( 1 );
    sliderSecondWaitBeforeRecording->show();

    sliderOpenh264 = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_openh264->insertWidget( 1, sliderOpenh264 );
    sliderOpenh264->setObjectName( "sliderOpenh264" );
    sliderOpenh264->setTracking( true );
    sliderOpenh264->setMinimum( 1 ); // we need minimum 1, with 0 we get wrong colors.
    sliderOpenh264->setMaximum( 51 );
    sliderOpenh264->setValue( 23 );
    sliderOpenh264->show();

    sliderWaitBeforeSnapshot = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_59->insertWidget( 1, sliderWaitBeforeSnapshot );
    sliderWaitBeforeSnapshot->setObjectName( "sliderWaitBeforeSnapshot" );
    sliderWaitBeforeSnapshot->setMinimum( 0 );
    sliderWaitBeforeSnapshot->setMaximum( 30 );
    sliderWaitBeforeSnapshot->setValue( 4 );
    sliderWaitBeforeSnapshot->setDecimalPoint( true );
    sliderWaitBeforeSnapshot->show();
}


void QvkMainWindow_wl::messageBox( QString value )
{
    QPixmap pixmap( ":/pictures/status/warning.png" );
    pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    QMessageBox *messageBox = new QMessageBox();
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    messageBox->setWindowIcon( icon );
    messageBox->setWindowTitle( global::name + " " + global::version );
    messageBox->setIconPixmap( pixmap );
    messageBox->setText( "Wayland desktop session detected" );
    messageBox->setInformativeText( ( "Please install the package\n" + value ) );
    messageBox->exec();
    exit(0);
}


void QvkMainWindow_wl::set_check_screencast_elements_available()
{
    QStringList list;
    list << "pipewiresrc";
    list << "videoconvert";
    list << "videorate";
    list << "queue";
    list << "h264parse";
    list << "pulsesrc";
    list << "audioconvert";
    list << "audiorate";
    list << "audioresample";
    list << "audiomixer";
    list << "filesink";
    list << "videocrop";

    qDebug().noquote() << global::nameOutput << "--- Screencast: GStreamer elements ---";

    for ( int i = 0; i < list.count(); i++ ) {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
            if ( list.at(i) == "pipewiresrc" ) {
               messageBox( "gstreamer-plugin-pipewire" );
            }
        } else {
            qDebug().noquote() << global::nameOutput << "+" << list.at(i);
            gst_object_unref( factory );
        }
    }
    qDebug();
}


void QvkMainWindow_wl::set_check_GIF_convert_elements_available()
{
    QStringList list;
    list << "filesrc";
    list << "matroskademux";
    list << "h264parse";
    list << "openh264dec";
    list << "queue";
    list << "videoconvert";
    list << "gifenc";
    list << "filesink";

    qDebug().noquote() << global::nameOutput << "--- Convert to GIF: GStreamer elements ---";

    for ( int i = 0; i < list.count(); i++ ) {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
        } else {
            qDebug().noquote() << global::nameOutput << "+" << list.at(i);
            gst_object_unref( factory );
        }
    }
    qDebug();
}


void QvkMainWindow_wl::set_check_MP4_convert_elements_available()
{
    QStringList list;
    list << "filesrc";
    list << "matroskademux";
    list << "h264parse";
    list << "queue";
    list << "mp4mux";
    list << "filesink";
    list << "mpegaudioparse";
    list << "opusparse";

    qDebug().noquote() << global::nameOutput << "--- Convert to MP4: GStreamer elements ---";

    for ( int i = 0; i < list.count(); i++ ) {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
        } else {
            qDebug().noquote() << global::nameOutput << "+" << list.at(i);
            gst_object_unref( factory );
        }
    }
    qDebug();
}


void QvkMainWindow_wl::set_check_WEBM_convert_elements_available()
{
    QStringList list;
    list << "filesrc";
    list << "matroskademux";
    list << "h264parse";
    list << "openh264dec";
    list << "queue";
    list << "videoconvert";
    list << "vp8enc";
    list << "webmmux";
    list << "filesink";
    list << "uridecodebin";
    list << "audioconvert";
    list << "audiorate";
    list << "vorbisenc";

    qDebug().noquote() << global::nameOutput << "--- Convert to WEBM: GStreamer elements ---";

    for ( int i = 0; i < list.count(); i++ ) {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory ) {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
        } else {
            qDebug().noquote() << global::nameOutput << "+" << list.at(i);
            gst_object_unref( factory );
        }
    }
    qDebug();
}


void QvkMainWindow_wl::set_RegionChoice()
{
    vkRegionChoise_wl = new QvkRegionChoise_wl( ui );
    connect(ui->radioButtonScreencastArea, &QRadioButton::toggled, this, [=](bool value){vkRegionChoise_wl->setVisible(value);});
    vkRegionChoise_wl->slot_init();
}


void QvkMainWindow_wl::slot_newVideoPath()
{
    QvkDirDialog *vkDirDialog = new QvkDirDialog;
    if ( vkDirDialog->exec() == QDialog::Accepted ) {
        if ( !vkDirDialog->selectedFiles().empty() ) {
            ui->lineEditVideoPath->setText( vkDirDialog->selectedFiles().at(0) );
        }
    }
}


void QvkMainWindow_wl::slot_videoFileSystemWatcherSetNewPath()
{
    if ( !videoFileSystemWatcher->directories().isEmpty() ) {
        videoFileSystemWatcher->removePaths( videoFileSystemWatcher->directories() );
    }
    videoFileSystemWatcher->addPath( ui->lineEditVideoPath->text() );
}


void QvkMainWindow_wl::slot_folder()
{
    if ( QDesktopServices::openUrl( QUrl( "file:///" + ui->lineEditVideoPath->text(), QUrl::TolerantMode ) ) == false ) {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}


void QvkMainWindow_wl::slot_log_folder()
{
    QUrl url( vkLogController_wl->get_log_filePath() );
    QString path = url.adjusted( QUrl::RemoveFilename ).toString();

    if ( QDesktopServices::openUrl( QUrl( "file:///" + path, QUrl::TolerantMode ) ) == false ) {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}


void QvkMainWindow_wl::slot_Pause()
{
    if ( ui->pushButtonStart->isEnabled() == false ) {
        qDebug().noquote() << global::nameOutput << "Pause was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_ASYNC" << "Returncode =" << ret;   }   // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3

        /* wait until it's up and running or failed */
        if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
          g_error ("Failed to go into PAUSED state");
        }
    }
}


void QvkMainWindow_wl::slot_Continue()
{
    if ( ( ui->pushButtonStart->isEnabled() == false ) and ( ui->pushButtonContinue->isEnabled() == true ) ) {
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_ASYNC" << "Returncode =" << ret;   }   // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3

        /* wait until it's up and running or failed */
        if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
          g_error ("Failed to go into PLAYING state");
        } else {
            qDebug().noquote() << global::nameOutput << "Continue was clicked";
        }
    }
}
