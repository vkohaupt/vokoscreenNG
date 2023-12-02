#include "mainWindow_wl.h"
#include "QvkInformation_wl.h"
#include "QvkImageFromTabs_wl.h"

#include "global.h"
#include "QvkLicenses.h"
#include "qvkdirdialog.h"

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

    set_LogController();

    QFile fileCSS( ":/pictures/css/css.qss" );
    fileCSS.open( QFile::ReadOnly | QFile::Text );
    QTextStream streamCSS( &fileCSS );
    qApp->setStyleSheet( streamCSS.readAll() );
    fileCSS.close();

    set_WindowTitle();
    ui->tabWidgetScreencast->setCurrentIndex( 0 );
    ui->tabWidgetSideBar->setCurrentIndex( 0 );
    set_CornerWidget();
    set_system_info();
    set_SpezialSliders();
    QvkInformation_wl *vkInformation = new QvkInformation_wl( this, ui );
    connect( this, SIGNAL( signal_newVideoFilename( QString ) ), vkInformation, SLOT( slot_newVideoFilename( QString ) ) );
    set_RegionChoice();
    set_Connects();
    set_check_all_Elements_available();
    vkContainerController_wl = new QvkContainerController_wl( ui );

    vkCountdown_wl = new QvkCountdown_wl();
    vkCountdown_wl->init();

    new QvkLicenses( ui->pushButtonLicense );
    new QvkImageFromTabs_wl( this );

    // Misc
    videoFileSystemWatcher = new QFileSystemWatcher();
    ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    connect( ui->toolButtonVideoPath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newVideoPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetNewPath() ) );

    // About
    ui->labelSourcecodeUrl->setText( "<a href='https://github.com/vkohaupt/vokoscreenNG'>" + tr( "Sourcecode" ) + "</a>" );
    ui->labelWebSiteUrl->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + tr( "Homepage" ) + "</a>" );
    ui->labelLanguageUrl->setText( "<a href='https://app.transifex.com/vkohaupt/vokoscreen/'>" + tr( "Translations" ) + "</a>" );
    ui->labelDonateUrl->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen-donate.html'>" + tr( "Donate" ) + "</a>" );

    ui->pushButtonPause->hide();
    ui->pushButtonContinue->hide();
    ui->pushButtonPlay->hide();
    ui->pushButtonScreencastOpenfolder->hide();
    ui->frame_video_audio_on_off->hide();
    ui->comboBoxAudioCodec->hide();
    ui->frame_cisco->hide();
    ui->line_cisco->hide();

    ui->radioButtonScreencastFullscreen->click();

    // Hide to time not needed tabs
    ui->tabWidgetScreencast->removeTab(1); // Audio

    qDebug();
    qDebug().noquote() << global::nameOutput << "Hint:";
    qDebug().noquote() << global::nameOutput << "When recording in full-screen mode, the screen resolution is recorded";
    qDebug().noquote() << global::nameOutput << "When recording an area, only the native resolution and optional scaling are supported.";
    qDebug();

    QList<QScreen *> screen = QGuiApplication::screens();
    if ( !screen.empty() ) {
        for ( int i = 0; i < screen.size(); i++ ) {
            if ( screen.at(i)->name() == QGuiApplication::primaryScreen()->name() ) {
                qDebug();
                qDebug().noquote() << global::nameOutput << "This screen is the primary screen: " << QGuiApplication::primaryScreen()->name();
            } else {
                qDebug().noquote() << global::nameOutput << "Name from screen: " << screen.at(i)->name();
            }
            qDebug().noquote() << global::nameOutput << "DevicePixelRatio:" << myDevicePixelRatio( screen.at(i) );
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
}


qreal QvkMainWindow_wl::myDevicePixelRatio( QScreen *m_screen )
{
    return (qreal)qRound( m_screen->logicalDotsPerInch() / m_screen->physicalDotsPerInchX() * 100 ) / 100;
}


QvkMainWindow_wl::~QvkMainWindow_wl()
{
}


void QvkMainWindow_wl::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    ui->pushButtonStop->click();
    vkRegionChoise_wl->close();
}


void QvkMainWindow_wl::set_LogController()
{
    vklogController = new QvkLogController();
    connect( vklogController, SIGNAL( signal_newLogText( QString ) ), ui->textBrowserLog, SLOT( append( QString ) ) );
}


void QvkMainWindow_wl::set_WindowTitle()
{
    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );
}


void QvkMainWindow_wl::set_system_info()
{
    qDebug().noquote() << global::nameOutput << "Version:" << global::version;
    qDebug().noquote() << global::nameOutput << "Locale:" << QLocale::system().name();
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug().noquote() << global::nameOutput << "Log from:" << dateTime.toString( "yyyy-MM-dd hh:mm:ss" );
    QLocale locale;
    qDebug().noquote() << global::nameOutput << "Country:" << QLocale::territoryToString( locale.territory() );
    qDebug().noquote() << global::nameOutput << "Qt:" << qVersion();
    qDebug().noquote() << global::nameOutput << gst_version_string();
//    qDebug().noquote() << global::nameOutput << "PulseAudio library version:" << pa_get_library_version();
    qDebug().noquote() << global::nameOutput << "Operating system:" << QSysInfo::prettyProductName();
    qDebug().noquote() << global::nameOutput << "CPU Architecture:" << QSysInfo::currentCpuArchitecture();
    qDebug().noquote() << global::nameOutput << "Count CPU:" << QThread::idealThreadCount();
    qDebug().noquote() << global::nameOutput << global::name << "running as:" << QGuiApplication::platformName() << "client";
    qDebug().noquote() << global::nameOutput << global::name << "running on:" << qgetenv( "XDG_SESSION_TYPE" ).toLower();
    qDebug().noquote() << global::nameOutput << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    qDebug().noquote() << global::nameOutput << "Icon-Theme:" << QIcon::themeName();
    qDebug().noquote() << global::nameOutput << "Styles:" << QStyleFactory::keys();
    qDebug().noquote() << global::nameOutput << "Qt-PluginsPath:     " << QLibraryInfo::path( QLibraryInfo::PluginsPath );
    qDebug().noquote() << global::nameOutput << "Qt-TranslationsPath:" << QLibraryInfo::path( QLibraryInfo::TranslationsPath );
    qDebug().noquote() << global::nameOutput << "Qt-LibraryPath:     " << QLibraryInfo::path( QLibraryInfo::LibrariesPath );
//    qDebug().noquote() << global::nameOutput << "Settings:" << vkSettings.getFileName();
    qDebug().noquote() << global::nameOutput << "Log:" << vklogController->get_logPath();
    qDebug().noquote() << global::nameOutput << "Default Videopath:" << QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
//    qDebug().noquote() << global::nameOutput << "User Videopath:" << vkSettings.getVideoPath();
    qDebug().noquote();
}


void QvkMainWindow_wl::set_CornerWidget()
{
    QList<QTabWidget *> list;
    list << ui->tabWidgetScreencast;
    list << ui->tabWidgetLog;

    QPixmap pixmap( ":/pictures/cornerWidget/wayland.png" );
    pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

    for ( int i = 0; i < list.count(); i++ ) {
        QLabel *label = new QLabel();
        label->setPixmap( pixmap );
        label->setEnabled( false );
        list.at( i )->setCornerWidget( label, Qt::TopRightCorner );
    }
}


void QvkMainWindow_wl::set_Connects()
{
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,                 SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastFullscreen, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastWindow,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastArea,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabVideo,                        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoPath,                  SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                                SLOT( slot_start() ) );

    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), this,                                SLOT( slot_stop() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStop,                  SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStart,                 SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->radioButtonScreencastFullscreen, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->radioButtonScreencastWindow,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->radioButtonScreencastArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->tabVideo,                        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->frameVideoPath,                  SLOT( setDisabled( bool ) ) );

    connect( ui->radioButtonScreencastFullscreen, SIGNAL( clicked( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastWindow,     SIGNAL( clicked( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastArea,       SIGNAL( clicked( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setEnabled( bool ) ) );

    connect( ui->radioButtonScreencastFullscreen, SIGNAL( clicked( bool ) ), ui->frame_area, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastWindow,     SIGNAL( clicked( bool ) ), ui->frame_area, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastArea,       SIGNAL( clicked( bool ) ), ui->frame_area, SLOT( setEnabled( bool ) ) );

    connect( portal_wl, SIGNAL( signal_portal_fd_path( QString, QString ) ), this, SLOT( slot_start_gst( QString, QString ) ) );
    connect( portal_wl, SIGNAL( signal_portal_cancel( uint ) ), this,              SLOT( slot_portal_cancel( uint ) ) );

    connect( ui->toolButtonScreencastAreaReset, SIGNAL( clicked( bool ) ), vkRegionChoise_wl, SLOT( slot_areaReset() ) );
    connect( ui->toolButtonFramesReset,         SIGNAL( clicked( bool ) ), this,              SLOT( slot_frames_Reset() ) );
}


QString QvkMainWindow_wl::get_Videocodec_Encoder()
{
    QString value;
    QString encoder = ui->comboBoxVideoCodec->currentData().toString();

    if ( encoder == "openh264enc" ) {
        QStringList list;
        list << encoder;
        list << "qp-min=23"; // + QString::number( sliderOpenh264->value() );
        list << "qp-max=23"; // + QString::number( sliderOpenh264->value() );
        list << "usage-type=camera"; // We need camera not screen. With screen and a fast sequence of images the video jerks.
        list << "complexity=low";
        list << "multi-thread=" + QString::number( QThread::idealThreadCount() );
        list << "slice-mode=auto"; // Number of slices equal to number of threads
        value = list.join( " " );
        value.append( " ! h264parse" );
    }

    if ( encoder == "x264enc" ) {
        QStringList list;
        list << encoder;
        list << "qp-min=17"; // + QString::number( sliderOpenh264->value() );
        list << "qp-max=17"; // + QString::number( sliderOpenh264->value() );
        list << "speed-preset=superfast";
        list << "threads=" + QString::number( QThread::idealThreadCount() );
        value = list.join( " " );
        value.append( " ! video/x-h264, profile=baseline" );
    }

    if ( encoder == "vp8enc" ) {
        QStringList list;
        list << encoder;
        list << "min_quantizer=20"; // + QString::number( sliderVp8->value() );
        list << "max_quantizer=20";  // + QString::number( sliderVp8->value() );
        list << "cpu-used=" + QString::number( QThread::idealThreadCount() );
        list << "deadline=1000000";
        list << "threads=" + QString::number( QThread::idealThreadCount() );
        value = list.join( " " );
    }

    return value;
}


QString QvkMainWindow_wl::get_Muxer()
{
    QString value = ui->comboBoxFormat->currentData().toString();
    if ( ui->comboBoxFormat->currentData().toString() == "matroskamux" ) {
        value = ui->comboBoxFormat->currentData().toString() + " name=mux writing-app=" + global::name + "_" + QString( global::version ).replace( " ", "_" );
    } else {
        value = ui->comboBoxFormat->currentData().toString() + " name=mux";
    }
    return value;
}


void QvkMainWindow_wl::slot_start()
{
    // https://flatpak.github.io/xdg-desktop-portal/portal-docs.html#gdbus-property-org-freedesktop-portal-ScreenCast.AvailableSourceTypes
    // Value 1 = MONITOR
    // Value 2 = WINDOW
    int sourceType = 1;
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

    // https://flatpak.github.io/xdg-desktop-portal/portal-docs.html#gdbus-property-org-freedesktop-portal-ScreenCast.AvailableCursorModes
    // Value 1 = hidden cursor
    // Value 2 = record cursor
    int mousecursorONOff = 1;
    if ( ui->checkBoxMouseCursorOnOff->isChecked() == true ) {
        qDebug().noquote() << global::nameOutput << "Mouse cursor is not recording";
        mousecursorONOff = 1;
    }

    if ( ui->checkBoxMouseCursorOnOff->isChecked() == false ) {
        qDebug().noquote() << global::nameOutput << "Mouse cursor is recording";
        mousecursorONOff = 2;
    }

    portal_wl->requestScreenSharing( sourceType, mousecursorONOff );
}

/*
QString QvkMainWindow_wl::get_Area_Videocrop()
{
    QString videocrop = "";
    vkRegionChoise_wl->set_recordMode( true );

    QString top    = QString::number( vkRegionChoise_wl->get_YRecordArea() );
    QString right  = QString::number( vkRegionChoise_wl->screenSizeX() - ( vkRegionChoise_wl->get_XRecordArea() + vkRegionChoise_wl->get_WidthRecordArea() ) );
    QString bottom = QString::number( vkRegionChoise_wl->screenSizeY() - ( vkRegionChoise_wl->get_YRecordArea() + vkRegionChoise_wl->get_HeightRecordArea() ) );
    QString left   = QString::number( vkRegionChoise_wl->get_XRecordArea() );
    videocrop = "videocrop top=" + top + " " + "right=" + right + " " + "bottom=" + bottom + " " + "left=" + left;

    qDebug().noquote() << global::nameOutput << "Area crop from the screen"
                       << vkRegionChoise_wl->screen()->name() + ","
                       << vkRegionChoise_wl->screen()->manufacturer() + ","
                       << vkRegionChoise_wl->screen()->model() + ","
                       << QString::number( vkRegionChoise_wl->screen()->size().width() ) + "/" + QString::number( vkRegionChoise_wl->screen()->size().height() );

    return videocrop;
}
*/
QString QvkMainWindow_wl::get_Area_Videocrop()
{
    QString videocrop = "";
    vkRegionChoise_wl->set_recordMode( true );
    int divTop = 0;
    int divBottom = 0;

    if ( ui->toolButton_area_top->isChecked() == true ) {
        divTop = screen()->size().height() - vkRegionChoise_wl->screenHeight;
    }

    if ( ui->toolButton_area_bottom->isChecked() == true ) {
        divBottom = screen()->size().height() - vkRegionChoise_wl->screenHeight;
    }

    qDebug() << "--------------------------------" << screen()->size().height() << height() << divTop;

    QString top    = QString::number( vkRegionChoise_wl->get_YRecordArea() + divTop );
    QString right  = QString::number( vkRegionChoise_wl->screenSizeX() - ( vkRegionChoise_wl->get_XRecordArea() + vkRegionChoise_wl->get_WidthRecordArea() ) );
    QString bottom = QString::number( vkRegionChoise_wl->screenSizeY() - ( vkRegionChoise_wl->get_YRecordArea() + vkRegionChoise_wl->get_HeightRecordArea() ) );
    QString left   = QString::number( vkRegionChoise_wl->get_XRecordArea() );
    videocrop = "videocrop top=" + top + " " + "right=" + right + " " + "bottom=" + bottom + " " + "left=" + left;

    qDebug().noquote() << global::nameOutput << "Area crop from the screen"
                       << vkRegionChoise_wl->screen()->name() + ","
                       << vkRegionChoise_wl->screen()->manufacturer() + ","
                       << vkRegionChoise_wl->screen()->model() + ","
                       << QString::number( vkRegionChoise_wl->screen()->size().width() ) + "x" + QString::number( vkRegionChoise_wl->screen()->size().height() );

    return videocrop;
}


void QvkMainWindow_wl::slot_start_gst( QString vk_fd, QString vk_path )
{
    ui->pushButtonStop->setEnabled( true );
    vkCountdown_wl->startCountdown( sliderScreencastCountDown->value() );

    QStringList stringList;
    stringList << QString( "pipewiresrc fd=" ).append( vk_fd ).append( " path=" ).append( vk_path ).append( " do-timestamp=true" );
    stringList << "videoconvert";
    stringList << "videorate";
    if ( ui->radioButtonScreencastArea->isChecked() ) { stringList << get_Area_Videocrop(); }
    stringList << "video/x-raw, framerate=" + QString::number( sliderFrames->value() ) + "/1";
    stringList << get_Videocodec_Encoder();
    stringList << get_Muxer();

    QString newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
    stringList << "filesink location=\"" + ui->lineEditVideoPath->text() + "/" + newVideoFilename + "\"";

    QString launch = stringList.join( " ! " );
    qDebug().noquote() << global::nameOutput << launch;

    pipeline = gst_parse_launch( launch.toUtf8(), nullptr );
    gst_element_set_state( pipeline, GST_STATE_PLAYING );

    emit signal_newVideoFilename( newVideoFilename );

}


void QvkMainWindow_wl::slot_stop()
{
    // send EOS to pipeline
    gst_element_send_event( pipeline, gst_event_new_eos() );

    // wait for the EOS to traverse the pipeline and is reported to the bus
    GstBus *bus = gst_element_get_bus( pipeline );
    gst_bus_timed_pop_filtered( bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS );
    gst_object_unref( bus );

    gst_element_set_state( pipeline, GST_STATE_NULL );
    gst_object_unref ( pipeline );

    qDebug().noquote() << global::nameOutput << "Stop record";

    if ( ui->radioButtonScreencastArea->isChecked() ) {
       vkRegionChoise_wl->set_recordMode( false );
       vkRegionChoise_wl->repaint();
       vkRegionChoise_wl->setMask( vkRegionChoise_wl->pixmap.mask() );
    }
}


void QvkMainWindow_wl::slot_portal_cancel( uint value )
{
    Q_UNUSED(value)
    ui->pushButtonStart->setEnabled( true );
    ui->pushButtonStop->setEnabled( false );
    ui->radioButtonScreencastFullscreen->setEnabled( true );
    ui->radioButtonScreencastWindow->setEnabled( true );
    ui->radioButtonScreencastArea->setEnabled( true );
    ui->tabVideo->setEnabled( true );
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
    ui->horizontalLayout_slider_frames->insertWidget( 0, sliderFrames );
    sliderFrames->setObjectName( "sliderFrames" );
    sliderFrames->setTracking( true );
    sliderFrames->setMinimum( 10 );
    sliderFrames->setMaximum( 144 );
    sliderFrames->setValue( 25 );
    sliderFrames->show();
}


void QvkMainWindow_wl::slot_frames_Reset()
{
    sliderFrames->setValue( 25 );
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


void QvkMainWindow_wl::set_check_all_Elements_available()
{
    QStringList list;
    list << "pipewiresrc";
    list << "pulsesrc";
    list << "queue";
    list << "capsfilter";
    list << "videoconvert";
    list << "videorate";
    list << "audioconvert";
    list << "audiorate";
    list << "filesink";
    list << "videoscale";
    list << "h264parse";
    list << "audiomixer";
    list << "videocrop";

    qDebug().noquote() << global::nameOutput << "--- GStreamer elements ---";

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


void QvkMainWindow_wl::set_RegionChoice()
{
    vkRegionChoise_wl = new QvkRegionChoise_wl( ui );
    connect( ui->radioButtonScreencastArea, SIGNAL( toggled( bool ) ), vkRegionChoise_wl, SLOT( setVisible( bool ) ) );

    connect( ui->toolButton_area_top, &QPushButton::clicked, this, [=]() { ui->toolButton_area_top->setIcon( QIcon( ":/pictures/screencast/accept.png" ) );
                                                                           ui->toolButton_area_right->setIcon( QIcon( "" ) );
                                                                           ui->toolButton_area_bottom->setIcon( QIcon( "" ) );
                                                                           ui->toolButton_area_left->setIcon( QIcon( "" ) );
                                                                         } );

    connect( ui->toolButton_area_right, &QPushButton::clicked, this, [=]() { ui->toolButton_area_right->setIcon( QIcon( ":/pictures/screencast/accept.png" ) );
                                                                             ui->toolButton_area_bottom->setIcon( QIcon( "" ) );
                                                                             ui->toolButton_area_left->setIcon( QIcon( "" ) );
                                                                             ui->toolButton_area_top->setIcon( QIcon( "" ) );
                                                                           } );

    connect( ui->toolButton_area_bottom, &QPushButton::clicked, this, [=]() { ui->toolButton_area_bottom->setIcon( QIcon( ":/pictures/screencast/accept.png" ) );
                                                                              ui->toolButton_area_left->setIcon( QIcon( "" ) );
                                                                              ui->toolButton_area_top->setIcon( QIcon( "" ) );
                                                                              ui->toolButton_area_right->setIcon( QIcon( "" ) );
                                                                            } );

    connect( ui->toolButton_area_left, &QPushButton::clicked, this, [=]() { ui->toolButton_area_left->setIcon( QIcon( ":/pictures/screencast/accept.png" ) );
                                                                            ui->toolButton_area_top->setIcon( QIcon( "" ) );
                                                                            ui->toolButton_area_right->setIcon( QIcon( "" ) );
                                                                            ui->toolButton_area_bottom->setIcon( QIcon( "" ) );
                                                                          } );

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
