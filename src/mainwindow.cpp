#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "ui_QvkNoPlayerDialog.h"
#include "QvkScreenshot.h"
#include "QvkInformation.h"
#include "QvkShowClickDialog.h"
#include "QvkAnimateControl.h"

#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QScreen>
#include <QDesktopWidget>
#include <QAudioDeviceInfo>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMimeDatabase>
#include <QStringList>
#include <QImageWriter>
#include <QScreen>
#include <QLibraryInfo>
#include <QThread>
#include <QtDebug>

#ifdef Q_OS_LINUX
  #include <QX11Info>
#endif

// gstreamer-plugins-bad-orig-addon
// gstreamer-plugins-good-extra
// libgstinsertbin-1_0-0

#ifdef Q_OS_LINUX
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          vkWinInfo(new QvkWinInfo),
                                          vkCountdown(new QvkCountdown),
                                          vkRegionChoise(new QvkRegionChoise)
#endif

#ifdef Q_OS_WIN
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          vkWinInfo(new QvkWinInfo),
                                          vkCountdown(new QvkCountdown),
                                          vkRegionChoise(new QvkRegionChoise),
                                          vkAudioWindows(new QvkAudioWindows(ui))
#endif
{
    ui->setupUi(this);
    //qSetMessagePattern("%{file}(%{line}): %{message}");
#ifdef Q_OS_LINUX
    vkAudioPulse = new QvkAudioPulse( ui );
    vkAudioAlsa = new QvkAudioAlsa( ui );
#endif
    vkSettings.readAll();
    vkMagnifierController = new QvkMagnifierController(ui);
    QvkHelp *vkHelp = new QvkHelp( ui );
    Q_UNUSED( vkHelp );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/vokoscreen.png" ), QSize(), QIcon::Normal, QIcon::Off );
    MainWindow::setWindowIcon( icon );

    setWindowTitle( vkSettings.getProgName() + " " + vkSettings.getVersion() );

    qDebug().noquote() << "[vokoscreen]" << "Version:" << vkSettings.getVersion();
    qDebug().noquote() << "[vokoscreen]" << "Locale:" << QLocale::system().name();
    qDebug().noquote() << "[vokoscreen]" << "Qt: " << qVersion();
    qDebug().noquote() << "[vokoscreen]" << gst_version_string();
    qDebug().noquote() << "[vokoscreen]" << "Operating system:" << QSysInfo::prettyProductName();
    qDebug().noquote() << "[vokoscreen]" << "vokoscreen running as:" << QGuiApplication::platformName() << "client";

#ifdef Q_OS_LINUX
    if ( QX11Info::isPlatformX11() == true )
        qDebug().noquote() << "[vokoscreen]" << "vokoscreen running on X11";
    else
        qDebug().noquote() << "[vokoscreen]" << "vokoscreen running on Wayland";
#endif
    qDebug().noquote() << "[vokoscreen]" << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    qDebug().noquote() << "[vokoscreen] current icon-theme: " << QIcon::themeName();
    qDebug().noquote() << "[vokoscreen] Qt-PluginsPath:     " << QLibraryInfo::location( QLibraryInfo::PluginsPath );
    qDebug().noquote() << "[vokoscreen] Qt-TranslationsPath:" << QLibraryInfo::location( QLibraryInfo::TranslationsPath );
    qDebug().noquote() << "[vokoscreen] Qt-LibraryPath:     " << QLibraryInfo::location( QLibraryInfo::LibrariesPath );
#ifdef Q_OS_LINUX
    qDebug().noquote() << "[vokoscreen] CompositingManager running:" << QX11Info::isCompositingManagerRunning();
#endif
    qDebug( " " );

    resolutionStringList << "320 x 200 CGA 16 : 10"
                         << "320 x 240 QCGA 4 : 3"
                         << "640 x 480 VGA 4 : 3"
                         << "720 x 480 NTSC 3 : 2"
                         << "800 x 480 WVGA 5 : 3"
                         << "800 x 600 SVGA 4 : 3"
                         << "854 x 450 WVGA 16 : 9"
                         << "768 x 567 PAL 4 :3"
                         << "1024 x 768 XVGA 4 : 3"
                         << "1152 x 768 N/A 3 : 2"
                         << "1280 x 720 HD-720 16 : 9"
                         << "1280 x 768 WXGA 5 : 3"
                         << "1280 x 800 WXGA 16 : 10"
                         << "1280 x 1024 SXGA 5 : 4"
                         << "1920 x 1080 HD1080 16 : 9";

#ifdef Q_OS_LINUX
    QPixmap pixmap( ":/pictures/linux.png" );
#endif
#ifdef Q_OS_WIN
    QPixmap pixmap( ":/pictures/windows.png" );
#endif
    pixmap = pixmap.scaled( 42, 42 );
    QLabel *label = new QLabel();
    label->setPixmap( pixmap );
    label->setEnabled( false );
    ui->tabWidgetScreencast->setCornerWidget( label, Qt::TopRightCorner);

    QvkInformation *vkInformation = new QvkInformation(ui);
    connect( this, SIGNAL( signal_newVideoFilename( QString ) ), vkInformation, SLOT( slot_newVideoFilename( QString ) ) );

    // need a move
    move( 0, 0 );

    makeAndSetValidIconForSideBar( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/video-display.svg" ) ) );
    makeAndSetValidIconForSideBar( 1, QIcon::fromTheme( "computer", QIcon( ":/pictures/computer.svg" ) ) );
    makeAndSetValidIconForSideBar( 2, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/camera-web.svg" ) ) );
    makeAndSetValidIconForSideBar( 3, QIcon::fromTheme( "help-about", QIcon( ":/pictures/help-about.svg" ) ) );

    makeAndSetValidIcon( ui->tabWidgetScreencast, 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/video-display.svg" ) ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/audio-input-microphone.svg" ) ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, 2, QIcon::fromTheme( "appointment-new", QIcon( ":/pictures/appointment-new.svg" ) ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, 3, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/systemsettings.svg" ) ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, 4, QIcon::fromTheme( "help-contents", QIcon( ":/pictures/help-contents.svg" ) ) );

    // Bar for start, stop etc.
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonContinue,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonFullscreen, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonWindow,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonArea,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxScreen,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameAudio,            SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->spinBoxFrames,         SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonFramesDefault,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonFormatDefault,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( slot_IfStartAudioCodecWidgetsSetEnabled() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoPath,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameStartTime,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStart() ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonFullscreen, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonWindow,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxScreen,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameAudio,            SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->spinBoxFrames,         SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonFramesDefault,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonFormatDefault,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this, SLOT( slot_IfStopAudioCodecWidgetsSetDisabled() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoPath,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameStartTime,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxStopRecordingAfter, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStop() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_Stop() ) );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), this,                   SLOT( slot_Pause() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonPause,    SLOT( hide() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( show() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonStop,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( setDisabled( bool ) ) );

    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this,                   SLOT( slot_Continue() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( hide() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonPause,    SLOT( show() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonStop,     SLOT( setDisabled( bool ) ) );
    ui->pushButtonContinue->hide();

    connect( ui->pushButtonPlay, SIGNAL( clicked( bool ) ), this, SLOT( slot_Play() ) );

    connect( ui->pushButtonScreencastOpenfolder, SIGNAL( clicked( bool ) ), this, SLOT( slot_Folder() ) );


    // Tab 1 Screen
#ifdef Q_OS_WIN
    ui->radioButtonWindow->hide();
    ui->toolButtonHelpWindow->hide();
#endif

    ui->radioButtonFullscreen->setText( tr("Fullscreen") ); // QT Creator sets an ampersand, translation now here
    ui->radioButtonWindow->setText( tr("Window") ); // QT Creator sets an ampersand, translation now here

    ui->toolButtonHelpFullscreen->setIcon( ui->toolButtonHelpFullscreen->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonHelpWindow->setIcon( ui->toolButtonHelpWindow->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonHelpArea->setIcon( ui->toolButtonHelpArea->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonAreaReset->setIcon( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/edit-undo.svg" ) ) );

    connect( ui->radioButtonFullscreen, SIGNAL( toggled( bool ) ), ui->toolButtonAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonFullscreen, SIGNAL( toggled( bool ) ), ui->comboBoxAreaSize, SLOT( setDisabled( bool ) ) );//**

    connect( ui->radioButtonWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonWindow, SIGNAL( toggled( bool ) ), ui->toolButtonAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonWindow, SIGNAL( toggled( bool ) ), ui->comboBoxAreaSize, SLOT( setDisabled( bool ) ) );

    connect( this,                  SIGNAL( signal_close()  ), vkRegionChoise,   SLOT( close() ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), vkRegionChoise,   SLOT( setVisible( bool ) ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), ui->toolButtonAreaReset, SLOT( setEnabled( bool ) ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), ui->comboBoxAreaSize, SLOT( setEnabled( bool ) ) );

    connect( ui->toolButtonAreaReset, SIGNAL( clicked( bool ) ), vkRegionChoise, SLOT( slot_areaReset() ) );
    connect( ui->toolButtonAreaReset, SIGNAL( clicked( bool ) ), this,           SLOT( slot_areaReset() ) );
    connect( ui->pushButtonStart,     SIGNAL( clicked( bool ) ), this, SLOT( slot_disableAreaWidgets() ) );
    connect( ui->pushButtonStop ,     SIGNAL( clicked( bool ) ), this, SLOT( slot_enableAreaWidgets() ) );

    ui->comboBoxAreaSize->addItems( resolutionStringList );
    connect( ui->comboBoxAreaSize, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( slot_areaSetResolution( QString ) ) );

    ui->toolButtonHelpCountdown->setIcon( ui->toolButtonHelpCountdown->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    // Begin showclick
    ui->toolButtonHelpShowclick->setIcon( ui->toolButtonHelpShowclick->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    QColor color   = vkSettings.getShowClickColor();
    int radiant    = vkSettings.getShowClickRadiant();
    double opacity = vkSettings.getShowClickOpacity();

    QvkShowClickDialog *ShowClickDialog = new QvkShowClickDialog( color, radiant, opacity );
    connect( ui->toolButtonShowKlick, SIGNAL( clicked() ), ShowClickDialog, SLOT( show() ) );

    QvkAnimateControl *animateControl = new QvkAnimateControl( (double) ShowClickDialog->myUiDialog.horizontalSliderShowtime->value()/10,
                                                               ShowClickDialog->myUiDialog.horizontalSliderCircle->value(),
                                                               ShowClickDialog->myUiDialog.checkBoxRadiant->checkState(),
                                                               (double) ShowClickDialog->myUiDialog.horizontalSliderOpacity->value()/100,
                                                               color
                                                             );

    connect( ui->checkBoxShowClick, SIGNAL( clicked( bool ) ),      animateControl, SLOT( pointerOnOff( bool ) ) );
    connect( this,                  SIGNAL( signal_close( bool ) ), animateControl, SLOT( pointerOnOff( bool ) ) );

    connect( ShowClickDialog, SIGNAL( newCircleWidgetValue( int, QColor ) ), animateControl, SLOT( setDiameterColor( int, QColor ) ) );
    connect( ShowClickDialog, SIGNAL( newShowtime( double ) ), animateControl, SLOT( setShowTime( double ) ) );
    connect( ShowClickDialog, SIGNAL( newOpacity( double ) ), animateControl, SLOT( setOpacity( double ) ) );
    connect( ShowClickDialog, SIGNAL( newRadiant( bool ) ), animateControl, SLOT( setRadiant( bool ) ) );
    // End showclick

    // Magnifier
    ui->toolButtonHelpMagnifier->setIcon( ui->toolButtonHelpMagnifier->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );


    // Tab 2 Audio
    ui->toolButtonFramesHelp->setIcon( ui->toolButtonFramesHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonAudioHelp->setIcon( ui->toolButtonAudioHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonNoMouseCursorHelp->setIcon( ui->toolButtonNoMouseCursorHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), this,                      SLOT( slot_audioIconOnOff( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->framePulseAlsa,        SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->scrollAreaAudioDevice, SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->labelAudioCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->comboBoxAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->pushButtonAudiocodecDefault, SLOT( setEnabled( bool ) ) );

#ifdef Q_OS_WIN
    ui->radioButtonPulse->hide();
    ui->radioButtonAlsa->hide();
    vkAudioWindows->slot_start( true );
#endif

    // Tab 2 Codec and Audio
    ui->pushButtonFramesDefault->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/edit-undo.svg" ) ) );
    ui->pushButtonFormatDefault->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/edit-undo.svg" ) ) );
    ui->pushButtonAudiocodecDefault->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/edit-undo.svg" ) ) );

    connect( ui->pushButtonFramesDefault, SIGNAL( clicked( bool ) ), this, SLOT( slot_setFramesStandard( bool ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_VideoCodecs_in_Combox( QString ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_AudioCodecs_in_Combox( QString ) ) );


    // Tab 3 Time
    connect( ui->checkBoxStartTime, SIGNAL( toggled( bool ) ), this, SLOT( slot_StartTimer( bool ) ) );
    ui->toolButtonHelpStartTime->setIcon( ui->toolButtonHelpStartTime->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    timerStartTimer = new QTimer();
    timerStartTimer->setTimerType( Qt::PreciseTimer );
    connect( timerStartTimer,  SIGNAL( timeout() ),           this, SLOT( slot_startTime() ) );
    connect( ui->checkBoxStartTime,  SIGNAL( clicked( bool ) ), ui->pushButtonStart, SLOT( setHidden( bool ) ) );
    connect( ui->SliderHouer,  SIGNAL( valueChanged( int ) ), this, SLOT( slot_setHour( int ) ) );
    connect( ui->SliderMinute, SIGNAL( valueChanged( int ) ), this, SLOT( slot_setMinute( int ) ) );
    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ), ui->labelCountdown, SLOT( setDisabled( bool ) ) );
    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ), ui->spinBoxCountDown, SLOT( setDisabled( bool ) ) );

    ui->toolButtonHelpStopRecordingAfter->setIcon( ui->toolButtonHelpStopRecordingAfter->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    connect( ui->checkBoxStopRecordingAfter, SIGNAL( toggled( bool ) ), ui->frameStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( timerStopRecordingAfter, SIGNAL( timeout() ), ui->pushButtonStop, SLOT( click() ) );


    // Tab 4 Misc
    ui->toolButtonHelpVideoPath->setIcon( ui->toolButtonHelpVideoPath->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    videoFileSystemWatcher = new QFileSystemWatcher();
    connect( ui->PushButtonVideoPath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newVideoPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    connect( videoFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );

    ui->comboBoxScale->addItems( resolutionStringList );
    ui->toolButtonHelpScale->setIcon( ui->toolButtonHelpScale->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    connect( ui->checkBoxScale,   SIGNAL( toggled( bool ) ), ui->comboBoxScale, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameScale, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->frameScale, SLOT( setDisabled( bool ) ) );

    ui->toolButtonHelpLimitOfFreeDiskSpace->setIcon( ui->toolButtonHelpLimitOfFreeDiskSpace->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    ui->toolButtonHelpShowInSystray->setIcon( ui->toolButtonHelpShowInSystray->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    connect( ui->checkBoxShowInSystray, SIGNAL( clicked( bool ) ), this, SLOT( slot_setVisibleSystray( bool ) ) );
    ui->checkBoxShowInSystray->clicked( true );

    ui->toolButtonHelpMinimizedWhenRecordingStarts->setIcon( ui->toolButtonHelpMinimizedWhenRecordingStarts->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    connect( ui->checkBoxMinimizedWhenRecordingStarts, SIGNAL( clicked( bool ) ),    ui->frameWaitXSecondBeforRecording, SLOT( setEnabled( bool ) ) );

    ui->toolButtonHelpVokoscreenStartsMinimized->setIcon( ui->toolButtonHelpVokoscreenStartsMinimized->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );


    // Tab 5 Available muxer, encoder etc.
    ui->toolButtonAvalaibleHelp->setIcon( ui->toolButtonAvalaibleHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    QIcon iconAvailable = ui->labelAvalible->style()->standardIcon( QStyle::SP_DialogApplyButton );
    QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    ui->labelAvalible->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

    QIcon iconNotAvailable = ui->labelNotAvailable->style()->standardIcon( QStyle::SP_DialogCancelButton );
    ui->labelNotAvailable->setPixmap( iconNotAvailable.pixmap( size, QIcon::Normal, QIcon::On ));
    // End Tabs

    // Close vokoscreen GUI
    connect( this,      SIGNAL( signal_close() ),       ui->pushButtonContinue, SLOT( click() ) );
    connect( this,      SIGNAL( signal_close() ),       ui->pushButtonStop,     SLOT( click() ) );
    connect( this,      SIGNAL( signal_close( bool ) ), ui->checkBoxCamera,     SLOT( setChecked( bool ) ) );
    connect( this,      SIGNAL( signal_close() ),       vkHelp,                 SLOT( close() ) );

    VK_Supported_Formats_And_Codecs();
    VK_Check_is_Format_available();
    VK_set_available_Formats_in_Combox();

    VK_gst_Elements_available();

    QDesktopWidget *desk = QApplication::desktop();
    connect( desk, SIGNAL( screenCountChanged(int) ), this, SLOT( slot_screenCountChanged( int ) ) );
    connect( desk, SIGNAL( resized( int ) ),          this, SLOT( slot_screenCountChanged( int ) ) );

    // Checkable Widget sind in vokoscreen standardmäßig nicht gesetzt.
    // Diese werden hier beziehungsweise wenn die Settings vorhanden sind dort gesetzt.
    ui->radioButtonFullscreen->click();


    // **************** Begin Screenshot *****************************
    QvkScreenshot *vkScreenshot = new QvkScreenshot( this, ui );
    Q_UNUSED(vkScreenshot);
    makeAndSetValidIcon( ui->tabWidgetScreenshot, 0, QIcon::fromTheme( "computer", QIcon( ":/pictures/computer.svg" ) ) );
    makeAndSetValidIcon( ui->tabWidgetScreenshot, 1, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/systemsettings.svg" ) ) );
    // **************** End Screenshot *******************************


    // *****************Begin Camera *********************************
    QvkCameraController *cameraController = new QvkCameraController( ui );
    Q_UNUSED(cameraController);
    makeAndSetValidIcon( ui->tabWidgetCamera, 0, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/camera-web.svg" ) ) );
    // *****************End Camera ***********************************
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    emit signal_close();
    emit signal_close( false );
}


void MainWindow::slot_setVisibleSystray( bool value )
{
    if ( value == false )
    {
        vkSystray->slot_closeSystray();
        delete vkSystray;
    }

    if ( value == true )
    {
        vkSystray = new QvkSystray(ui);
        vkSystray->init();
        connect( vkSystray, SIGNAL( signal_SystemtrayIsClose() ), this,      SLOT( close() ) );
        connect( this,      SIGNAL( signal_close() ),             vkSystray, SLOT( slot_closeSystray() ) );
    }
}


void MainWindow::slot_IfStartAudioCodecWidgetsSetEnabled()
{
    if ( ui->checkBoxAudioOnOff->checkState() == Qt::Checked )
    {
        ui->labelAudioCodec->setEnabled( false );
        ui->comboBoxAudioCodec->setEnabled( false );
        ui->pushButtonAudiocodecDefault->setEnabled( false );
    }
}


void MainWindow::slot_IfStopAudioCodecWidgetsSetDisabled()
{
    if ( ui->checkBoxAudioOnOff->checkState() == Qt::Checked )
    {
        ui->labelAudioCodec->setEnabled( true );
        ui->comboBoxAudioCodec->setEnabled( true );
        ui->pushButtonAudiocodecDefault->setEnabled( true );
    }
}


void MainWindow::slot_areaSetResolution( QString value )
{
    QString width = value.section( " ", 0, 0 );
    QString height = value.section( " ", 2, 2 );
    vkRegionChoise->areaSetResolution( QString(width).toInt(), QString(height).toInt() );
}


void MainWindow::slot_areaReset()
{
    ui->comboBoxAreaSize->setCurrentIndex( 0 );
}


void MainWindow::slot_disableAreaWidgets()
{
   if ( ui->radioButtonArea->isChecked() == true  )
   {
       ui->toolButtonAreaReset->setEnabled( false );
       ui->comboBoxAreaSize->setEnabled( false );
   }
}


void MainWindow::slot_enableAreaWidgets()
{
   if ( ui->radioButtonArea->isChecked() == true  )
   {
       ui->toolButtonAreaReset->setEnabled( true );
       ui->comboBoxAreaSize->setEnabled( true );
   }
}


void MainWindow::slot_StartTimer( bool value )
{
    if ( value == true )
    {
        timerStartTimer->start( 1000 );
    }
    else
    {
        timerStartTimer->stop();
    }
}


void MainWindow::slot_startTime()
{
    QTime time;
    if ( ( time.currentTime().hour() == ui->timeEditStartTime->time().hour() ) and
         ( time.currentTime().minute() == ui->timeEditStartTime->time().minute() ) and
         ( time.currentTime().second() == ui->timeEditStartTime->time().second() ) )
    {
        ui->pushButtonStart->setVisible( true );
        ui->pushButtonStart->click();
    }
}


void MainWindow::slot_setHour( int value )
{
    QTime time;
    time.setHMS( value, ui->timeEditStartTime->time().minute(), 0 );
    ui->timeEditStartTime->setTime( time );
}


void MainWindow::slot_setMinute( int value )
{
    QTime time;
    time.setHMS( ui->timeEditStartTime->time().hour(), value, 0 );
    ui->timeEditStartTime->setTime( time );
}


void MainWindow::slot_newVideoPath()
{
    QString dir = QFileDialog::getExistingDirectory( this,
                                                     "",
                                                     QStandardPaths::writableLocation( QStandardPaths::HomeLocation ),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dir > "" )
    {
        ui->lineEditVideoPath->setText( dir );
    }
}


void MainWindow::slot_videoFileSystemWatcherSetNewPath()
{
    if ( !videoFileSystemWatcher->directories().isEmpty() )
    {
       videoFileSystemWatcher->removePaths( videoFileSystemWatcher->directories() );
    }
    videoFileSystemWatcher->addPath( ui->lineEditVideoPath->text() );
}


void MainWindow::slot_videoFileSystemWatcherSetButtons()
{
  QDir dir( ui->lineEditVideoPath->text() );
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = dir.entryList( filters, QDir::Files, QDir::Time );

  if ( List.isEmpty() || ( ui->pushButtonStart->isEnabled() == false ) )
  {
    ui->pushButtonPlay->setEnabled( false );
  }
  else
  {
    ui->pushButtonPlay->setEnabled( true );
  }
}


void MainWindow::resizeEvent( QResizeEvent *event )
{
    emit signal_resizeEvent( event );
}


void MainWindow::makeAndSetValidIcon( QTabWidget *tabWidget, int index , QIcon icon )
{
    QSize size = tabWidget->iconSize();
    QPixmap workPixmap( icon.pixmap( size ) );
    workPixmap = workPixmap.scaled( size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    tabWidget->setTabIcon( index, QIcon( workPixmap ) );
}


void MainWindow::makeAndSetValidIconForSideBar( int index, QIcon icon )
{
    QSize size = ui->tabWidgetSideBar->iconSize();
    QPixmap workPixmap( icon.pixmap( size ) );
    workPixmap = workPixmap.scaled( size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QTransform transform;
    transform.rotate( 90 );
    workPixmap = workPixmap.transformed( transform, Qt::SmoothTransformation );
    ui->tabWidgetSideBar->setTabIcon( index, QIcon( workPixmap ) );
}


/*
 * Setzt neues Icon um aufzuzeigen das Audio abgeschaltet ist
 */
void MainWindow::slot_audioIconOnOff( bool state )
{
  if ( state == Qt::Unchecked )
  {
    QIcon myIcon = ui->tabWidgetScreencast->tabIcon( 1 );
    QSize size = ui->tabWidgetScreencast->iconSize();
    QPixmap workPixmap( myIcon.pixmap( size ) );
    QPainter painter;
    QPen pen;
    painter.begin( &workPixmap );
      pen.setColor( Qt::red );
      pen.setWidth( 2 );
      painter.setPen( pen );
      painter.drawLine ( 5, 5, size.width()-5, size.height()-5 );
      painter.drawLine ( 5, size.height()-5, size.width()-5, 5 );
    painter.end();
    makeAndSetValidIcon( ui->tabWidgetScreencast, 1, QIcon( workPixmap ) );
  }
  else{
    makeAndSetValidIcon( ui->tabWidgetScreencast, 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/audio-input-microphone.svg" ) ) );
  }
}


void MainWindow::slot_setFramesStandard( bool value )
{
    Q_UNUSED(value);
    ui->spinBoxFrames->setValue( 25 );
}


#ifdef Q_OS_LINUX
QString MainWindow::VK_getXimagesrc()
{
    QString showPointer = "true";
    if( ui->checkBoxMouseCursorOnOff->checkState() == Qt::Checked )
    {
        showPointer = "false";
    }

    if( ( ui->radioButtonFullscreen->isChecked() == true ) and ( ui->comboBoxScreen->currentData().toInt() == -1  ) )
    {
        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer;
        QString value = stringList.join( " " );
        return value;
    }

    if ( ( ui->radioButtonFullscreen->isChecked() == true ) and ( ui->comboBoxScreen->currentIndex() <= ui->comboBoxScreen->count()-1 ) )
    {
          QStringList stringList;
          stringList << "ximagesrc"
                     << "display-name=" + qgetenv( "DISPLAY" )
                     << "use-damage=false"
                     << "show-pointer=" + showPointer
                     << "startx=" + get_x_From_Screen()
                     << "starty=" + get_y_From_Screen()
                     << "endx="   + QString::number( get_x_From_Screen().toInt() + get_width_From_Screen().toInt()-1 )
                     << "endy="   + QString::number( get_y_From_Screen().toInt() + get_height_From_Screen().toInt()-1 );
          QString value = stringList.join( " " );
          return value;
    }

    if( ui->radioButtonWindow->isChecked() == true )
    {
        QString xid;
        if ( ui->checkBoxStartTime->isChecked() == true )
        {
            xid = QString::number( QvkWinInfo::activeWindow() );
        }
        else
        {
            xid = QString::number( vkWinInfo->getWinID() );
        }

        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer
                   << "xid=" + xid;

        QString value = stringList.join( " " );
        return value;
    }

    if ( ui->radioButtonArea->isChecked() == true )
    {
        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer
                   << "startx=" + QString::number( vkRegionChoise->getXRecordArea() )
                   << "starty=" + QString::number( vkRegionChoise->getYRecordArea() )
                   << "endx="   + QString::number( vkRegionChoise->getXRecordArea() + vkRegionChoise->getWidth()-1 )
                   << "endy="   + QString::number( vkRegionChoise->getYRecordArea() + vkRegionChoise->getHeight()-1 );
        QString value = stringList.join( " " );
        return value;
    }

    return ""; // prophylactic no error at compiletime
}
#endif

// https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good/html/gst-plugins-good-plugins-multifilesrc.html
#ifdef Q_OS_WIN
QString MainWindow::VK_getXimagesrc()
{
    QString value;
    QString showPointer = "true";

    if( ui->checkBoxMouseCursorOnOff->checkState() == Qt::Checked )
    {
        showPointer = "false";
    }

    if( ui->radioButtonFullscreen->isChecked() == true )
    {
        QStringList stringList;
        stringList << "gdiscreencapsrc"
                   << "cursor=" + showPointer;
        value = stringList.join( " " );
    }

    if ( ui->radioButtonArea->isChecked() == true )
    {
        QStringList stringList;
        stringList << "gdiscreencapsrc"
                   << "cursor=" + showPointer
                   << "x=" + QString::number( vkRegionChoise->getXRecordArea() )
                   << "y=" + QString::number( vkRegionChoise->getYRecordArea() )
                   << "width=" + QString::number( vkRegionChoise->getWidth() )
                   << "height=" + QString::number( vkRegionChoise->getHeight() );
        value = stringList.join( " " );
    }

    return value;
}
#endif


QString MainWindow::VK_getCapsFilter()
{
   QStringList stringList;
   stringList << "capsfilter caps=video/x-raw,framerate="
              << ui->spinBoxFrames->text()
              << "/1";
   return QString( stringList.join( "" ) );
}


QString MainWindow::VK_getVideoScale()
{
    QString value = ui->comboBoxScale->currentText();
    QStringList valuList = value.split( " " );
    value = "videoscale ! video/x-raw,width=" + valuList.at(0) + ",height=" + valuList.at(2);
    return value;
}


// Check format, video and audoicodec on tab availability
void MainWindow::VK_gst_Elements_available()
{
    int rowCount = 0;
    for ( int i = 0; i < globalFormatsList.count(); i++ )
    {
        int rowMuxer = 1;
        int rowVideo = 1;
        int rowAudio = 1;
        QStringList listElements = QString( globalFormatsList.at(i) ).split( "," ); // listElement beinhaltet muxer, Video und Audio
        for ( int x = 0; x < listElements.count(); x++ )
        {
            bool available;
            QString element = QString( listElements.at( x ) ).section( ":", 1, 1 );
            GstElementFactory *factory = gst_element_factory_find( element.toLatin1() );
            if ( !factory )
            {
                available = false;
            }
            else
            {
                available = true;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "muxer" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogCancelButton );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowMuxer, 0 );

                ui->gridLayoutAvailable->addWidget( new QLabel( QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowMuxer, 1 );
                rowMuxer++;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "videocodec" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogCancelButton );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowVideo, 2 );

                ui->gridLayoutAvailable->addWidget( new QLabel( QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowVideo, 3 );
                rowVideo++;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "audiocodec" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogCancelButton );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowAudio, 4 );
                ui->gridLayoutAvailable->addWidget( new QLabel( QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowAudio, 5 );
                rowAudio++;
            }

        }

        rowCount = ui->gridLayoutAvailable->rowCount();
        for ( int x = 0; x <= 5; x++ )
        {
            ui->gridLayoutAvailable->addWidget( new QLabel(""), rowCount, x );
        }
    }

    ui->gridLayoutAvailable->addItem( new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                                      ui->gridLayoutAvailable->rowCount() + 1, 0 );
}


// This is the base for format, video and audiocodec
void MainWindow::VK_Supported_Formats_And_Codecs()
{
    QStringList MKV_QStringList = ( QStringList()
                                    << "muxer:matroskamux:mkv"
                                    << "videomimetype:video/x-matroska"
                                    << "audiomimetype:audio/x-matroska"
                                    << "videocodec:x264enc:x264"
                                    << "videocodec:vp8enc:vp8"
                                    << "audiocodec:vorbisenc:vorbis"
                                    << "audiocodec:flacenc:flac"
                                    << "audiocodec:opusenc:opus"
                                    << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList WEBM_QStringList = ( QStringList()
                                     << "muxer:webmmux:webm"
                                     << "videomimetype:video/webm"
                                     << "audiomimetype:audio/webm"
                                     << "videocodec:vp8enc:vp8"
                                     << "audiocodec:vorbisenc:vorbis"
                                     << "audiocodec:opusenc:opus"
                                   );

    QStringList AVI_QStringList = ( QStringList()
                                     << "muxer:avimux:avi"
                                     << "videomimetype:video/x-msvideo"
                                     << "audiomimetype:audio/x-msvideo"
                                     << "videocodec:x264enc:x264"
                                     << "videocodec:vp8enc:vp8"
                                     << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList MP4_QStringList = ( QStringList()
                                    << "muxer:mp4mux:mp4"
                                    << "videomimetype:video/mpeg"
                                    << "audiomimetype:audio/mpeg"
                                    << "videocodec:x264enc:x264"
                                    << "audiocodec:lamemp3enc:mp3"
                                    << "audiocodec:opusenc:opus"
                                  );

    videoFormatsList.clear();
    videoFormatsList.append( MKV_QStringList.join( ","  ) );
    videoFormatsList.append( WEBM_QStringList.join( ","  ) );
    videoFormatsList.append( AVI_QStringList.join( "," ) );
    videoFormatsList.append( MP4_QStringList.join( ",") );

    globalFormatsList << videoFormatsList;

    /*
        videoFormatsList.append( "asfmux:asf" );
        videoFormatsList.append( "flvmux:flv" );
    */

}


void MainWindow::VK_Check_is_Format_available()
{
    QStringList tempList;
    for ( int x = 0; x < videoFormatsList.count(); x++ )
    {
        QString stringAllKeys = videoFormatsList.at( x );
        QStringList listKeys = stringAllKeys.split( "," );
        QStringList listKey = listKeys.filter( "muxer" );
        QString muxer = QString( listKey.at( 0 ) ).section( ":", 1, 1 );

        GstElementFactory *factory = gst_element_factory_find( muxer.toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << "[vokoscreen] Fail Muxer not available:" << muxer;
        }
        else
        {
            qDebug().noquote() << "[vokoscreen] Muxer available:" << muxer;
            tempList << videoFormatsList.at( x );
        }
    }
    videoFormatsList.clear();
    videoFormatsList << tempList;
}


void MainWindow::VK_set_available_Formats_in_Combox()
{
    ui->comboBoxFormat->clear();

    for ( int x = 0; x < videoFormatsList.count(); x++  )
    {
        QString stringAllKeys = videoFormatsList.at( x );
        QStringList listKeys = stringAllKeys.split( "," );
        QStringList listKeyMuxer = listKeys.filter( "muxer" );

        QMimeDatabase mimeDatabase;
        QStringList listKeyVideoMimetype = listKeys.filter( "videomimetype" );
        QMimeType mimetype = mimeDatabase.mimeTypeForName( QString( listKeyVideoMimetype.at( 0 ) ).section( ":", 1 ) );
        QIcon icon = QIcon::fromTheme( mimetype.iconName(), QIcon( ":/pictures/videooptionen.png" ) );

        ui->comboBoxFormat->addItem( QIcon::fromTheme( mimetype.iconName(), QIcon( ":/pictures/videooptionen.png" ) ), // Picture
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ), // suffix
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 1, 1 ) ); // muxer
    }
}


void MainWindow::slot_set_available_VideoCodecs_in_Combox( QString suffix )
{
    ui->comboBoxVideoCodec->clear();

    QStringList listSuffix = videoFormatsList.filter( suffix );
    QString stringSuffix = listSuffix.at( 0 );
    QStringList listKeys = stringSuffix.split( "," );
    QStringList listKeyVideoCodec = listKeys.filter( "videocodec" );
    for ( int i = 0; i < listKeyVideoCodec.count(); i++ )
    {
        QString encoder = QString( listKeyVideoCodec.at( i ) ).section( ":", 1, 1 );
        QString name =    QString( listKeyVideoCodec.at( i ) ).section( ":", 2, 2 );
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << "[vokoscreen] Fail video encoder not available:" << encoder;
        }
        else
        {
            qDebug().noquote() << "[vokoscreen] Video encoder avalaible:" << encoder;
            ui->comboBoxVideoCodec->addItem( name, encoder );
        }
    }
}


void MainWindow::slot_set_available_AudioCodecs_in_Combox( QString suffix )
{
    ui->comboBoxAudioCodec->clear();

    QStringList listSuffix = videoFormatsList.filter( suffix );
    QString stringSuffix = listSuffix.at( 0 );
    QStringList listKeys = stringSuffix.split( "," );
    QStringList listKeyAudioCodec = listKeys.filter( "audiocodec" );
    for ( int i = 0; i < listKeyAudioCodec.count(); i++ )
    {
        QString encoder = QString( listKeyAudioCodec.at( i ) ).section( ":", 1, 1 );
        QString name =    QString( listKeyAudioCodec.at( i ) ).section( ":", 2, 2 );
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << "[vokoscreen] Fail audio encoder not available:" << encoder;
        }
        else
        {
            qDebug().noquote() << "[vokoscreen] Audio encoder avalaible:" << encoder;
            ui->comboBoxAudioCodec->addItem( name, encoder );
        }
    }
    qDebug( " " );
}


QString MainWindow::Vk_get_Videocodec_Encoder()
{
    QString value;
    QString encoder = ui->comboBoxVideoCodec->currentData().toString();
    if ( encoder == "x264enc" )
    {
        value = "x264enc speed-preset=veryfast pass=qual threads=0";
    }

    if ( encoder == "vp8enc" )
    {
        value = "vp8enc min_quantizer=20 max_quantizer=20 cpu-used=4 deadline=1000000 threads=4";
    }

    return value;
}


void MainWindow::slot_preStart()
{
    if ( ui->checkBoxStopRecordingAfter->isChecked() == true )
    {
        int value = ui->spinBoxStopRecordingAfterHouers->value()*60*60*1000;
        value += ui->spinBoxStopRecordingAfterMinutes->value()*60*1000;
        value += + ui->spinBoxStopRecordingAfterSeconds->value()*1000;
        timerStopRecordingAfter->setTimerType( Qt::PreciseTimer );
        timerStopRecordingAfter->start( value );
    }


    if ( ( ui->radioButtonFullscreen->isChecked() == true ) and ( ui->checkBoxStartTime->isChecked() == true ) )
    {
        slot_Start();
        return;
    }


    if ( ( ui->radioButtonFullscreen->isChecked() == true ) and  ( ui->spinBoxCountDown->value() > 0 ) )
    {
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkCountdown->startCountdown( ui->spinBoxCountDown->value() );
        return;
    }

    if ( ui->radioButtonFullscreen->isChecked() == true )
    {
        slot_Start();
        return;
    }


    if ( ( ui->radioButtonWindow->isChecked() == true ) and ( ui->checkBoxStartTime->isChecked() == true ) )
    {
        slot_Start();
        return;
    }


    if ( ( ui->radioButtonWindow->isChecked() == true ) and ( ui->spinBoxCountDown->value() > 0 ) )
    {
        disconnect( vkWinInfo, 0, 0, 0 );
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkWinInfo,   SIGNAL( signal_windowChanged( bool ) ),   this,                SLOT( slot_startCounter( bool ) ) );
        connect( vkWinInfo,   SIGNAL( signal_showCursor( bool ) ),      ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo,   SIGNAL( signal_showCursor( bool ) ),      ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkWinInfo->slot_start();
        return;
    }

    if ( ui->radioButtonWindow->isChecked() == true )
    {
        disconnect( vkWinInfo, 0, 0, 0 );
        connect( vkWinInfo, SIGNAL( signal_showCursor( bool ) ),    ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_showCursor( bool ) ),    ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this,                SLOT( slot_Start() ) );
        vkWinInfo->slot_start();
        return;
    }


    if ( ( ui->radioButtonArea->isChecked() == true ) and ( ui->checkBoxStartTime->isChecked() == true ) )
    {
        slot_Start();
        return;
    }


    if ( ( ui->radioButtonArea->isChecked() == true ) and ( ui->spinBoxCountDown->value() > 0 ) )
    {
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkRegionChoise->recordMode( true );
        vkCountdown->startCountdown( ui->spinBoxCountDown->value() );
        return;
    }


    if ( ui->radioButtonArea->isChecked() == true )
    {
       vkRegionChoise->recordMode( true );
       vkRegionChoise->repaint();
       vkRegionChoise->update();
       slot_Start();
       return;
    }
}


void MainWindow::slot_startCounter( bool value )
{
    Q_UNUSED(value);
    if ( ui->spinBoxCountDown->value() > 0 )
    {
        vkCountdown->startCountdown( ui->spinBoxCountDown->value() );
    }
}


QString MainWindow::VK_get_AudioDevice()
{
    QString audioDevice;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
        if ( listQCheckBox.at(i)->checkState() == Qt::Checked )
        {
            audioDevice = listQCheckBox.at(i)->accessibleName();
        }
    }
    return audioDevice;
}


#ifdef Q_OS_LINUX
// Audiosystem is Pulse, Alsa, etc.
QString MainWindow::VK_get_AudioSystem()
{
    QString audioSystem;
    if ( ui->radioButtonPulse->isChecked() )
    {
        audioSystem = "pulsesrc";
    }
    if ( ui->radioButtonAlsa->isChecked() )
    {
        audioSystem = "alsasrc";
    }
    return audioSystem;
}
#endif


#ifdef Q_OS_WIN
QString MainWindow::VK_get_AudioSystem()
{
    return "directsoundsrc";
}
#endif


QString MainWindow::VK_getMuxer()
{
    QString value = ui->comboBoxFormat->currentData().toString();
    if ( ( ui->checkBoxAudioOnOff->isChecked() == true ) and ( !VK_get_AudioDevice().isEmpty() ) and ( ui->comboBoxAudioCodec->count() > 0  ) )
    {
        value = "mux. " + ui->comboBoxFormat->currentData().toString() + " name=mux";
    }

    return value;
}



QStringList MainWindow::VK_getSelectedAudioDevice()
{
    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
        if ( listQCheckBox.at(i)->checkState() == Qt::Checked )
        {
            list << listQCheckBox.at(i)->accessibleName();
        }
    }
    return list;
}


void MainWindow::slot_Start()
{
    if ( ui->checkBoxMinimizedWhenRecordingStarts->isChecked() == true  )
    {
        setWindowState( Qt::WindowMinimized );
        QThread::msleep( ui->spinBoxMinimizedWhenRecordingStarts->value() * 1000 );
    }
    QString newVideoFilename = "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
    QString path = ui->lineEditVideoPath->text();

    QStringList VK_PipelineList;
    VK_PipelineList << VK_getXimagesrc();
    VK_PipelineList << VK_getCapsFilter();
    VK_PipelineList << "queue flush-on-eos=true";
    VK_PipelineList << "videoconvert";
    VK_PipelineList << "queue flush-on-eos=true";
    if ( ui->checkBoxScale->isChecked() )
    {
       VK_PipelineList << VK_getVideoScale();
    }
    VK_PipelineList << "videorate";
    VK_PipelineList << "queue flush-on-eos=true";
    VK_PipelineList << Vk_get_Videocodec_Encoder();
    VK_PipelineList << "queue flush-on-eos=true";
/*
    if ( ( ui->checkBoxAudioOnOff->isChecked() == true ) and ( !VK_get_AudioDevice().isEmpty() ) and ( ui->comboBoxAudioCodec->count() > 0  ) )
    {
        #ifdef Q_OS_LINUX
        VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device=" ).append( VK_get_AudioDevice() );
        #endif

        #ifdef Q_OS_WIN
        VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device-name=" ).append( "'" + VK_get_AudioDevice() +"'" );
        #endif

        VK_PipelineList << "queue flush-on-eos=true";
        VK_PipelineList << "audioconvert";
        VK_PipelineList << "queue flush-on-eos=true";
        VK_PipelineList << "audiorate";
        VK_PipelineList << "queue flush-on-eos=true";
        VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
        VK_PipelineList << "queue flush-on-eos=true";
    }

    if ( VK_secondAudioDevice() > "" )
    {
        if ( ( ui->checkBoxAudioOnOff->isChecked() == true ) and ( !VK_secondAudioDevice().isEmpty() ) and ( ui->comboBoxAudioCodec->count() > 0  ) )
        {
            #ifdef Q_OS_LINUX
            VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device=" ).append( VK_secondAudioDevice() );
            #endif

            #ifdef Q_OS_WIN
            VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device-name=" ).append( "'" + VK_secondAudioDevice() +"'" );
            #endif

            VK_PipelineList << "queue flush-on-eos=true";
            VK_PipelineList << "audioconvert";
            VK_PipelineList << "queue flush-on-eos=true";
            VK_PipelineList << "audiorate";
            VK_PipelineList << "queue flush-on-eos=true";
            VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
            VK_PipelineList << "queue flush-on-eos=true";
        }
    }
*/


    for ( int x = 0; x < VK_getSelectedAudioDevice().count(); x++ )
    {
        if ( ( ui->checkBoxAudioOnOff->isChecked() == true ) and ( !VK_getSelectedAudioDevice().isEmpty() ) and ( ui->comboBoxAudioCodec->count() > 0  ) )
        {
            #ifdef Q_OS_LINUX
            VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device=" ).append( VK_getSelectedAudioDevice().at(x) );
            #endif

            #ifdef Q_OS_WIN
            VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device-name=" ).append( "'" + VK_getSelectedAudioDevice().at(x) +"'" );
            #endif

            VK_PipelineList << "queue flush-on-eos=true";
            VK_PipelineList << "audioconvert";
            VK_PipelineList << "queue flush-on-eos=true";
            VK_PipelineList << "audiorate";
            VK_PipelineList << "queue flush-on-eos=true";
            VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
            VK_PipelineList << "queue flush-on-eos=true";
        }
    }

    VK_PipelineList << VK_getMuxer();
    VK_PipelineList << "filesink location=" + path + "/" + newVideoFilename;

    QString VK_Pipeline = VK_PipelineList.join( VK_Gstr_Pipe );
    qDebug() << "[vokoscreen] Start record with:" << VK_Pipeline;

    QByteArray byteArray = VK_Pipeline.toUtf8();
    const gchar *line = byteArray.constData();
    pipeline = gst_parse_launch( line, &error );

    // Start playing
    GstStateChangeReturn ret;
    ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
    if ( ret == GST_STATE_CHANGE_FAILURE )
    {
        g_printerr("[vokoscreen] Unable to set the pipeline to the playing state.\n");
        gst_object_unref( pipeline );
        return;
    }

    emit signal_newVideoFilename( newVideoFilename );
}

/*
    g_print( error->message );

    GstElement *fpsdisplaysink;
    //myPipeline = gst_parse_launch( "ximagesrc use-damage=false ! capsfilter caps=video/x-raw ! videoconvert ! fpsdisplaysink video-sink=fakesink signal-fps-measurements=true name=sink", &error );
    fpsdisplaysink = gst_bin_get_by_name (GST_BIN(pipeline), "sink" );
    if (fpsdisplaysink)
    {
        qDebug() << "*********************************************************************************";
        g_object_set (G_OBJECT (fpsdisplaysink), "signal-fps-measurements", TRUE, NULL);
        g_signal_connect (fpsdisplaysink, "fps-measurements", G_CALLBACK (cb_fps_measurements), NULL);
        g_print("fps-measurements connected\n");
    }
*/


void MainWindow::slot_preStop()
{
    if ( ui->checkBoxStartTime->isChecked() == true )
    {
         ui->checkBoxStartTime->click();
    }

    if ( timerStopRecordingAfter->isActive() )
    {
        timerStopRecordingAfter->stop();
        ui->frameStopRecordingAfter->setEnabled( true );
    }

    if ( ui->radioButtonArea->isChecked() == true )
    {
        vkRegionChoise->recordMode( false );
        vkRegionChoise->repaint();
        vkRegionChoise->update();
    }
}


void MainWindow::slot_Stop()
{
    // wait for EOS
    bool a = gst_element_send_event (pipeline, gst_event_new_eos());
    Q_UNUSED(a);

    GstClockTime timeout = 5 * GST_SECOND;
    GstMessage *msg = gst_bus_timed_pop_filtered (GST_ELEMENT_BUS (pipeline), timeout, GST_MESSAGE_EOS );
    Q_UNUSED(msg);

    GstStateChangeReturn ret ;
    Q_UNUSED(ret);
    ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
    ret = gst_element_set_state( pipeline, GST_STATE_READY );
    ret = gst_element_set_state( pipeline, GST_STATE_NULL );
    gst_object_unref( pipeline );
    qDebug() << "[vokoscreen] Stop record";
}

// https://gstreamer.freedesktop.org/documentation/application-development/basics/elements.html
// https://gstreamer.freedesktop.org/documentation/tutorials/basic/toolkit-integration.html
void MainWindow::slot_Pause()
{
    if ( ui->pushButtonStart->isEnabled() == false )
    {
        qDebug() << "[vokoscreen] Pause was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PAUSED ); // so wie es aussieht hängt er nur mit Audio
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug() << "[vokoscreen] Pause was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug() << "[vokoscreen] Pause was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug() << "[vokoscreen] Pause was clicked" << "GST_STATE_CHANGE_ASYNC" << "Returncode =" << ret;   }   // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug() << "[vokoscreen] Pause was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3


        /* wait until it's up and running or failed */
        if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
        {
          g_error ("Failed to go into PAUSED state");
        }

    }
}


void MainWindow::slot_Continue()
{
    if ( ( ui->pushButtonStart->isEnabled() == false ) and ( ui->pushButtonContinue->isEnabled() == true ) )
    {
        qDebug() << "[vokoscreen] Continue was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug() << "[vokoscreen] Continue was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug() << "[vokoscreen] Continue was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug() << "[vokoscreen] Continue was clicked" << "GST_STATE_CHANGE_ASYNC" << "Returncode =" << ret;   }   // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug() << "[vokoscreen] Continue was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3

        /* wait until it's up and running or failed */
        if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
        {
          g_error ("Failed to go into PLAYING state");
        }
        else
        {
            qDebug() << "***********************" << ret;
        }
    }
}


void MainWindow::slot_Play()
{
    qDebug() << "[vokoscreen] play video with standard system player";
    QDir dir( ui->lineEditVideoPath->text() );
    QStringList filters;
    filters << "vokoscreen*";
    QStringList videoFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    QString string;
    string.append( "file:///" );
    string.append( ui->lineEditVideoPath->text() );
    string.append( "/" );
    string.append( videoFileList.at( 0 ) );
    bool b = QDesktopServices::openUrl( QUrl( string, QUrl::TolerantMode ) );
    if ( b == false )
    {
        QDialog *newDialog = new QDialog;
        Ui_NoPlayerDialog myUiDialog;
        myUiDialog.setupUi( newDialog );
        myUiDialog.labelPleaseInstall->setText( tr( "Please install an video player" ) );
        newDialog->setModal( true );
        newDialog->setWindowTitle( "vokoscreen" );
        newDialog->show();
    }
}


void MainWindow::slot_Folder()
{
    QString path = ui->lineEditVideoPath->text();
    bool b = QDesktopServices::openUrl( QUrl( path, QUrl::TolerantMode ) );
    if ( b == false )
    {
        QDialog *newDialog = new QDialog;
        Ui_NoPlayerDialog myUiDialog;
        myUiDialog.setupUi( newDialog );
        myUiDialog.labelPleaseInstall->setText( tr( "Please install an filemanager" ) );
        newDialog->setModal( true );
        newDialog->setWindowTitle( "vokoscreen" );
        newDialog->show();
    }
}


QString MainWindow::get_x_From_Screen()
{
    QString value = ui->comboBoxScreen->currentData().toString().section( " ", 0, 0 ).split( "=" ).at( 1 );
    return value;
}

QString MainWindow::get_y_From_Screen()
{
    QString value = ui->comboBoxScreen->currentData().toString().section( " ", 1, 1 ).split( "=" ).at( 1 );
    return value;
}

QString MainWindow::get_width_From_Screen()
{
    QString value = ui->comboBoxScreen->currentData().toString().section( " ", 2, 2 ).split( "=" ).at( 1 );
    return value;
}

QString MainWindow::get_height_From_Screen()
{
    QString value = ui->comboBoxScreen->currentData().toString().section( " ", 3, 3 ).split( "=" ).at( 1 );
    return value;
}


void MainWindow::slot_screenCountChanged( int value )
{
    Q_UNUSED(value);
    ui->comboBoxScreen->clear();
    QList <QScreen *> screen = QGuiApplication::screens();
    qDebug().noquote() << "[vokoscreen] Detected count screens:" << screen.count();
    qDebug( " " );
    for ( int x = 0 ; x <= screen.count()-1; x++ )
    {
        qDebug().noquote() << "[vokoscreen] Name from screen: " << screen.at(x)->name();
        qDebug().noquote() << "[vokoscreen] Screen available desktop width :" << QString::number( screen.at(x)->geometry().width() * screen.at(x)->devicePixelRatio() );
        qDebug().noquote() << "[vokoscreen] Screen available desktop height:" << QString::number( screen.at(x)->geometry().height() * screen.at(x)->devicePixelRatio() );
        qDebug().noquote() << "[vokoscreen] DevicePixelRatio:" << screen.at(x)->devicePixelRatio() << " (Normal displays is 1, Retina display is 2)";
        qDebug().noquote() << "[vokoscreen] Vertical refresh rate of the screen in Hz:" << screen.at(x)->refreshRate();
        qDebug().noquote() << "[vokoscreen] Screen orientation" << screen.at(x)->orientation();
        qDebug().noquote() << "[vokoscreen] Color depth of the screen: " << screen.at(x)->depth();
        qDebug().noquote() << "[vokoscreen] Model from screen: " << screen.at(x)->model() ;
        qDebug().noquote() << "[vokoscreen] Manufactur from screen: " << screen.at(x)->manufacturer();
        qDebug().noquote() << "[vokoscreen] SerialNumber from screen: " << screen.at(x)->serialNumber();

        QDesktopWidget *desk = QApplication::desktop();
        QString X = QString::number( desk->screenGeometry( x ).left() * screen.at(x)->devicePixelRatio() );
        QString Y = QString::number( desk->screenGeometry( x ).top() * screen.at(x)->devicePixelRatio() );
        QString Width = QString::number( desk->screenGeometry( x ).width() * screen.at(x)->devicePixelRatio() );
        QString Height = QString::number( desk->screenGeometry( x ).height() * screen.at(x)->devicePixelRatio() );
        QString stringText = screen.at(x)->name() + ":  " + Width + " x " + Height;
        QString stringData = "x=" + X + " " +
                             "y=" + Y + " " +
                             "with=" + Width + " " +
                             "height=" + Height;
        ui->comboBoxScreen->addItem( stringText, stringData );
        qDebug().noquote() << "[vokoscreen] ItemText in Combobox:" << ui->comboBoxScreen->itemText(x);
        qDebug().noquote() << "[vokoscreen] ItemData in Combobox:" << ui->comboBoxScreen->itemData(x).toString();
        qDebug( " " );
    }
}

