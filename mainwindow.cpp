#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "ui_QvkNoPlayerDialog.h"

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
#include <QTest>
#include <QLibraryInfo>

#ifdef Q_OS_LINUX
  #include <QX11Info>
#endif

// gstreamer-plugins-bad-orig-addon
// gstreamer-plugins-good-extra
// libgstinsertbin-1_0-0

#include <gst/gst.h>

QString MainWindow::get_AudioDeviceString( GstDevice *device )
{
  static const char *const ignored_propnames[] = { "name", "parent", "direction", "template", "caps", NULL };
  GString *launch_line = NULL;
  GstElement *element;
  GstElement *pureelement;
  GParamSpec **properties, *property;
  GValue value = G_VALUE_INIT;
  GValue pvalue = G_VALUE_INIT;
  guint i, number_of_properties;
  GstElementFactory *factory;

  element = gst_device_create_element( device, NULL );

  if ( !element )
    return NULL;

  factory = gst_element_get_factory( element );
  if ( !factory )
  {
    gst_object_unref( element );
    return NULL;
  }

  if ( !gst_plugin_feature_get_name( factory ) )
  {
    gst_object_unref( element );
    return NULL;
  }

  pureelement = gst_element_factory_create( factory, NULL );

  properties = g_object_class_list_properties( G_OBJECT_GET_CLASS( element ), &number_of_properties );
  if ( properties )
  {
    for ( i = 0; i < number_of_properties; i++ )
    {
      gint j;
      gboolean ignore = FALSE;
      property = properties[i];

      if ( ( property->flags & G_PARAM_READWRITE ) != G_PARAM_READWRITE )
        continue;

      for ( j = 0; ignored_propnames[j]; j++ )
        if ( !g_strcmp0( ignored_propnames[j], property->name ) )
          ignore = TRUE;

      if ( ignore )
        continue;

      g_value_init( &value, property->value_type );
      g_value_init( &pvalue, property->value_type );
      g_object_get_property( G_OBJECT( element ), property->name, &value );
      g_object_get_property( G_OBJECT( pureelement ), property->name, &pvalue );
      if (gst_value_compare( &value, &pvalue ) != GST_VALUE_EQUAL )
      {
        gchar *valuestr = gst_value_serialize( &value );
        if ( !valuestr )
        {
          GST_WARNING( "Could not serialize property %s:%s", GST_OBJECT_NAME( element ), property->name );
          g_free( valuestr );
          goto next;
        }

        launch_line = g_string_new( valuestr );

        g_free( valuestr );
      }

    next:
      g_value_unset( &value );
      g_value_unset( &pvalue );
    }
    g_free( properties );
  }

  gst_object_unref( element );
  gst_object_unref( pureelement );

  QString string = g_string_free( launch_line, FALSE );
  return string;
}


QStringList MainWindow::get_all_Audio_devices()
{
   GstDeviceMonitor *monitor;
   GstCaps *caps;
   GstDevice *device;
   gchar *name;
   GList *iterator = NULL;
   GList *list = NULL;
   QString stringDevice;
   QStringList stringList;

   monitor = gst_device_monitor_new();
   caps = gst_caps_new_empty_simple( "audio/x-raw" );
   gst_device_monitor_add_filter( monitor, "Audio/Source", caps );

   list = gst_device_monitor_get_devices( monitor );
   for ( iterator = list; iterator; iterator = iterator->next )
   {
       device = (GstDevice*)iterator->data;
       name = gst_device_get_display_name( device );
       //g_print("%s   %s\n", get_launch_line( device ), name );
       stringDevice = get_AudioDeviceString( device );
       stringDevice.append( " ::: " ).append( name );
       stringList.append( stringDevice );
   }

   return stringList;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    qDebug().noquote() << "[vokoscreen]" << "Locale:" << QLocale::system().name();
    qDebug().noquote() << "[vokoscreen]" << "Qt version: " << qVersion();
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
    qDebug().noquote() << "[vokoscreen] screen available desktop width :" << screen->availableSize().width();
    qDebug().noquote() << "[vokoscreen] screen available desktop height:" << screen->availableSize().height();
    qDebug().noquote() << "[vokoscreen] Name from screen: " << screen->name();
    qDebug().noquote() << "[vokoscreen] Vertical refresh rate of the screen in Hz:" << screen->refreshRate();
    qDebug().noquote() << "[vokoscreen] Screen orientation" << screen->orientation();
    qDebug().noquote() << "[vokoscreen] Color depth of the screen: " << screen->depth();
    qDebug().noquote() << "[vokoscreen] Model from screen: " << screen->model() ;
    qDebug().noquote() << "[vokoscreen] Manufactur from screen: " << screen->manufacturer();
    qDebug().noquote() << "[vokoscreen] SerialNumber from screen: " << screen->serialNumber();
    qDebug( " " );

    ui->setupUi(this);

    // need a move
    move( 0, 0 );

    qDebug() << "[vokoscreen]" << gst_version_string ();

    makeValidIconForSideBar( 0 );
    makeValidIconForSideBar( 1 );
    ui->tabWidgetSideBar->setTabIcon( 2, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/webcam.png" ) ) );
    makeValidIconForSideBar( 2 );


    ui->tabWidget->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );
    makeAndSetValidIcon( 0 );

    ui->tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
    makeAndSetValidIcon( 1 );

    ui->tabWidget->setTabIcon( 2, QIcon::fromTheme( "applications-multimedia", QIcon( ":/pictures/videooptionen.png" ) ) );
    makeAndSetValidIcon( 2 );

    ui->tabWidget->setTabIcon( 3, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/tools.png" ) ) );
    makeAndSetValidIcon( 3 );

    ui->tabWidget->setTabIcon( 4, QIcon::fromTheme( "help-contents", QIcon( ":/pictures/webcam.png" ) ) );
    makeAndSetValidIcon( 4 );

    vkWinInfo = new QvkWinInfo();
    vkCountdown = new QvkCountdown();
    regionChoise = new QvkRegionChoise();
    regionChoise->hide();

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
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxAudioOnOff,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonPulse,      SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonAlsa,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->scrollAreaAudioDevice, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->toolButtonAudioHelp,   SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabMisc,               SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->spinBoxFrames,         SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonFramesDefault,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonFormatStandard,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelAudioCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonAudiocodecStandard,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStart() ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonFullscreen, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonWindow,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxScreen,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxAudioOnOff,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonPulse,      SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonAlsa,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->scrollAreaAudioDevice, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->tabMisc,               SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->spinBoxFrames,         SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonFramesDefault,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonFormatStandard,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelAudioCodec,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxAudioCodec,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonAudiocodecStandard,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setDisabled( bool ) ) );

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


    // Tab 1 Screen
#ifdef Q_OS_WIN
    ui->radioButtonWindow->hide();
#endif
    connect( this,                  SIGNAL( signal_close()  ), regionChoise,   SLOT( close() ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), regionChoise,   SLOT( setVisible( bool ) ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );

    connect( ui->radioButtonWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );


    // Tab 2 Audio
    ui->toolButtonFramesHelp->setIcon( ui->toolButtonFramesHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonAudioHelp->setIcon( ui->toolButtonAudioHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    ui->toolButtonNoMouseCursor->setIcon( ui->toolButtonNoMouseCursor->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    connect( ui->toolButtonAudioHelp, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioHelp() ) );;
    ui->radioButtonPulse->setAccessibleName( "pulsesrc" );
    ui->radioButtonAlsa->setAccessibleName( "alsasrc" );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), this,                      SLOT( slot_audioIconOnOff( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->radioButtonPulse,      SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->radioButtonAlsa,       SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->scrollAreaAudioDevice, SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->labelAudioCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->comboBoxAudioCodec,    SLOT( setEnabled( bool ) ) );

    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), this, SLOT( slot_clearVerticalLayoutAudioDevices( bool ) ) );
    connect( ui->radioButtonPulse, SIGNAL( toggled( bool ) ), this, SLOT( slot_getPulsesDevices( bool ) ) );
    connect( ui->radioButtonAlsa,  SIGNAL( toggled( bool ) ), this, SLOT( slot_clearVerticalLayoutAudioDevices( bool ) ) );
    connect( ui->radioButtonAlsa,  SIGNAL( toggled( bool ) ), this, SLOT( slot_getAlsaDevices( bool ) ) );

    // Pulse is Standard. If no pulsedevice found change to alsa
    ui->radioButtonPulse->click();

    // Tab 3 Codec
    ui->pushButtonFramesDefault->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    connect( ui->pushButtonFramesDefault, SIGNAL( clicked( bool ) ), this, SLOT( slot_setFramesStandard( bool ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_VideoCodecs_in_Combox( QString ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_AudioCodecs_in_Combox( QString ) ) );

    // Tab 4 Misc
    videoFileSystemWatcher = new QFileSystemWatcher();
    connect( ui->PushButtonVideoPath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newVideoPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    connect( videoFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );


    // Tab 5 Available muxer, encoder etc.
    ui->toolButtonAvalaibleHelp->setIcon( ui->toolButtonAvalaibleHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
    connect( ui->toolButtonAvalaibleHelp, SIGNAL( clicked( bool ) ), SLOT( slot_availableHelp() ) );

    QIcon iconAvailable = ui->labelAvalible->style()->standardIcon( QStyle::SP_DialogApplyButton );
    QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    ui->labelAvalible->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

    QIcon iconNotAvailable = ui->labelNotAvailable->style()->standardIcon( QStyle::SP_MessageBoxCritical );
    ui->labelNotAvailable->setPixmap( iconNotAvailable.pixmap( size, QIcon::Normal, QIcon::On ));
    // End Tabs

    // Close
    connect( this, SIGNAL( signal_close() ),  ui->pushButtonContinue, SLOT( click() ) );
    connect( this, SIGNAL( signal_close() ),  ui->pushButtonStop, SLOT( click() ) );
    connect( this, SIGNAL( signal_close_webcam( bool ) ),  ui->CheckBoxCamera, SLOT( setChecked( bool ) ) );

    VK_Supported_Formats_And_Codecs();
    VK_Check_is_Format_available();
    VK_set_available_Formats_in_Combox();

    VK_gst_Elements_available();

    QDesktopWidget *desk = QApplication::desktop();
    connect( desk, SIGNAL( screenCountChanged(int) ), this, SLOT( slot_screenCountChanged( int ) ) );
    connect( desk, SIGNAL( resized( int ) ),          this, SLOT( slot_screenCountChanged( int ) ) );
    //emit desk->screenCountChanged(0);

    // Checkable Widget sind in vokoscreen standardmäßig nicht gesetzt.
    // Diese werden hier beziehungsweise wenn die Settings vorhanden sind dort gesetzt.
    ui->radioButtonFullscreen->click();


    // **************** Begin Screenshot *******************************
    ui->toolButtonScreenshotHelp->setIcon( ui->toolButtonScreenshotHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    ui->tabWidgetScreenshot->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );
    makeAndSetValidIcon( 0 );

    ui->tabWidgetScreenshot->setTabIcon( 1, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/tools.png" ) ) );
    makeAndSetValidIcon( 1 );

    pictureFileSystemWatcher = new QFileSystemWatcher();
    connect( ui->toolButtonPicturePath, SIGNAL( clicked( bool ) ),      this, SLOT( slot_newPicturePath() ) );
    connect( ui->lineEditPicturePath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_pictureFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditPicturePath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_pictureFileSystemWatcherSetButtons() ) );
    connect( pictureFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_pictureFileSystemWatcherSetButtons() ) );
    ui->lineEditPicturePath->setText( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );

    connect( desk, SIGNAL( screenCountChanged(int) ), this, SLOT( slot_Screenshot_count_changed( int ) ) );
    connect( desk, SIGNAL( resized( int ) ),          this, SLOT( slot_Screenshot_count_changed( int ) ) );
    connect( ui->radioButtonScreenshotArea, SIGNAL( toggled( bool ) ), regionChoise, SLOT( setVisible( bool ) ) );

    connect( ui->pushButtonScreenshotShot, SIGNAL( clicked( bool ) ), this, SLOT( slot_preshot_Screenshot() ) );
    connect( ui->pushButtonScreenshotShow, SIGNAL( clicked( bool ) ), this, SLOT( slot_show_Screenshoot() ) );

    emit desk->screenCountChanged(0);
    slot_formats_Screenshot();

    // Checkable Widget sind in vokoscreen standardmäßig nicht gesetzt.
    // Diese werden hier beziehungsweise wenn die Settings vorhanden sind dort gesetzt.
    ui->radioButtonScreenshotFullscreen->click();

    // **************** End Screenshot *******************************


    // ************************* Camera ****************************
    // Tab 0 Camera
    webcamController = new QvkWebcamController( ui );
    ui->tabWidgetCamera->setTabIcon( 0, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/webcam.png" ) ) );
    makeAndSetValidIcon( 0 );
}


MainWindow::~MainWindow()
{
    delete ui;
}

// ----------------- Begin Screenshot ----------------------------------------------------------------------------

void MainWindow::slot_formats_Screenshot()
{
    ui->comboBoxScreenShotFormat->clear();
    QImageWriter imageWriter;
    QList<QByteArray> list = imageWriter.supportedImageFormats();

    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            ui->comboBoxScreenShotFormat->addItem( list.at( i ) );
        }
    }
}


void MainWindow::slot_preshot_Screenshot()
{
    if ( ( ui->radioButtonScreenshotFullscreen->isChecked() == true ) and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        hide();
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if ( ui->radioButtonScreenshotFullscreen->isChecked() == true )
    {
        hide();
        slot_shot_Screenshot();
        return;
    }


    if( ( ui->radioButtonScreenshotWindow->isChecked() == true )  and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {

        hide();
        disconnect( vkCountdown, 0, 0, 0 );
        disconnect( vkWinInfo, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), vkWinInfo, SLOT( slot_start() ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if( ui->radioButtonScreenshotWindow->isChecked() == true )
    {
        hide();
        disconnect( vkWinInfo, 0, 0, 0 );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_shot_Screenshot() ) );
        vkWinInfo->slot_start();
        return;
    }


    if ( ( ui->radioButtonScreenshotArea->isChecked() == true ) and ( ui->spinBoxScreenshotCountDown->value() > 0 ) )
    {
        hide();
        regionChoise->hide();
        disconnect( vkCountdown, 0, 0, 0 );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ),   this,         SLOT( slot_shot_Screenshot() ) );
        connect( this,        SIGNAL( signal_finish_screenshot( bool ) ), regionChoise, SLOT( show( bool ) ) );
        vkCountdown->startCountdown( ui->spinBoxScreenshotCountDown->value() );
        return;
    }

    if ( ui->radioButtonScreenshotArea->isChecked() == true )
    {
        hide();
        regionChoise->hide();
        slot_shot_Screenshot();
        regionChoise->show();
    }
}


void MainWindow::slot_shot_Screenshot()
{
    QTest::qSleep( 1000 );
    QApplication::beep();

    if ( ui->radioButtonScreenshotFullscreen->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( QApplication::desktop()->winId() );
    }

    if( ui->radioButtonScreenshotWindow->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( vkWinInfo->getWinID() );
    }

    if ( ui->radioButtonScreenshotArea->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        pixmap = screen->grabWindow( QApplication::desktop()->winId(),
                                     regionChoise->getXRecordArea(),
                                     regionChoise->getYRecordArea(),
                                     regionChoise->getWidth(),
                                     regionChoise->getHeight() );
    }

    bool ok = pixmap.save( ui->lineEditPicturePath->text() + \
                           "/" + \
                           "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxScreenShotFormat->currentText() );
    Q_UNUSED(ok);

    ui->labelScreenShotPicture->setAlignment( Qt::AlignCenter );
    ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width()-6,
                                                          ui->labelScreenShotPicture->height()-6,
                                                          Qt::KeepAspectRatio,
                                                          Qt::SmoothTransformation ) );
    show();
    emit signal_finish_screenshot( true );
}


void MainWindow::slot_show_Screenshoot()
{
    qDebug() << "[vokoscreen] show picture with standard system displayer";
    QDir dir( ui->lineEditPicturePath->text() );
    QStringList filters;
    filters << "vokoscreen*";
    QStringList pictureFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    QString string;
    string.append( "file://" );
    string.append( ui->lineEditPicturePath->text() );
    string.append( QDir::separator() );
    string.append( pictureFileList.at( 0 ) );
    bool b = QDesktopServices::openUrl( QUrl( string, QUrl::TolerantMode ) );
    if ( b == false )
    {
        QDialog *newDialog = new QDialog;
        Ui_NoPlayerDialog myUiDialog;
        myUiDialog.setupUi( newDialog );
        newDialog->setModal( true );
        newDialog->setWindowTitle( "vokoscreen" );
        newDialog->show();
    }
}


void MainWindow::slot_Screenshot_count_changed( int newCount )
{
    Q_UNUSED(newCount);
    ui->comboBoxScreenshotScreen->clear();
    QDesktopWidget *desk = QApplication::desktop();

    QScreen *screen = QGuiApplication::primaryScreen();

    for ( int i = 1; i < desk->screenCount()+1; i++ )
    {
        QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).left() * screen->devicePixelRatio() );
        QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).top() * screen->devicePixelRatio() );
        QString ScreenGeometryWidth = QString::number( desk->screenGeometry( i-1 ).width() * screen->devicePixelRatio() );
        QString ScreenGeometryHeight = QString::number( desk->screenGeometry( i-1 ).height() * screen->devicePixelRatio() );
        QString stringText = tr( "Display" ) + " " + QString::number( i ) + ":  " + ScreenGeometryWidth + " x " + ScreenGeometryHeight;
        QString stringData = "x=" + ScreenGeometryX + " " +
                             "y=" + ScreenGeometryY + " " +
                             "with=" + ScreenGeometryWidth + " " +
                             "height=" + ScreenGeometryHeight;
        ui->comboBoxScreenshotScreen->addItem( stringText, stringData );
    }

    ui->comboBoxScreenshotScreen->addItem( tr( "All Displays" ), -1 );
}


void MainWindow::slot_newPicturePath()
{
    QString dir = QFileDialog::getExistingDirectory( this,
                                                     "",
                                                     QStandardPaths::writableLocation( QStandardPaths::HomeLocation ),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dir > "" )
    {
        ui->lineEditPicturePath->setText( dir );
    }
}


void MainWindow::slot_pictureFileSystemWatcherSetNewPath()
{
    if ( !pictureFileSystemWatcher->directories().isEmpty() )
    {
       pictureFileSystemWatcher->removePaths( pictureFileSystemWatcher->directories() );
    }
    pictureFileSystemWatcher->addPath( ui->lineEditPicturePath->text() );
}


void MainWindow::slot_pictureFileSystemWatcherSetButtons()
{
  QDir dir( ui->lineEditPicturePath->text() );
  QStringList filters;
  filters << "vokoscreen*";
  QStringList List = dir.entryList( filters, QDir::Files, QDir::Time );

  if ( List.isEmpty() || ( ui->pushButtonScreenshotShot->isEnabled() == false ) )
  {
    ui->pushButtonScreenshotShow->setEnabled( false );
  }
  else
  {
    ui->pushButtonScreenshotShow->setEnabled( true );
  }
}

// ***************************** End Screenshot *****************************************************


void MainWindow::slot_audioHelp()
{
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/3.0/audio.html", QUrl::TolerantMode ) );
}


void MainWindow::slot_availableHelp()
{
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/3.0/available.html", QUrl::TolerantMode ) );
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


void MainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    emit signal_close();
    emit signal_close_webcam( false );
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if ( !pixmap.isNull() )
    {
        ui->labelScreenShotPicture->setPixmap( pixmap.scaled( ui->labelScreenShotPicture->width()-20,
                                                              ui->labelScreenShotPicture->height()-20,
                                                              Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation ) );
    }
}


void MainWindow::makeAndSetValidIcon( int index )
{
  QIcon myIcon = ui->tabWidget->tabIcon( index );
  QSize size = ui->tabWidget->iconSize();
  QPixmap workPixmap( myIcon.pixmap( size ) );
  ui->tabWidget->setTabIcon( index, QIcon( workPixmap ) );
}


void MainWindow::makeValidIconForSideBar( int index )
{
  QIcon myIcon = ui->tabWidgetSideBar->tabIcon( index );
  QSize size = ui->tabWidgetSideBar->iconSize();
  QPixmap workPixmap( myIcon.pixmap( size ) );
  QTransform trans;
  trans.rotate(90);
  workPixmap = workPixmap.transformed(trans);
  ui->tabWidgetSideBar->setTabIcon( index, QIcon( workPixmap ) );
}

/*
 * Setzt neues Icon um aufzuzeigen das Audio abgeschaltet ist
 */
void MainWindow::slot_audioIconOnOff( bool state )
{
  if ( state == Qt::Unchecked )
  {
    QIcon myIcon = ui->tabWidget->tabIcon( 1 );
    QSize size = ui->tabWidget->iconSize();
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
    ui->tabWidget->setTabIcon( 1, QIcon( workPixmap ) );
  }
  else{
    ui->tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
    makeAndSetValidIcon( 1 );
  }
}


void MainWindow::slot_clearVerticalLayoutAudioDevices( bool value )
{
    Q_UNUSED(value);
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
       ui->verticalLayoutAudioDevices->removeWidget( listQCheckBox.at(i) );
       delete listQCheckBox.at(i);
    }
}


void MainWindow::slot_getPulsesDevices( bool value )
{
    Q_UNUSED(value);
    QStringList list = get_all_Audio_devices();
    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            checkboxAudioDevice->setAccessibleName( QString( list.at(i) ).section( " ::: ", 0, 0 ) );
            ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count()-i-1, checkboxAudioDevice );
        }
    }
    else
    {
        ui->radioButtonPulse->setEnabled( false );
        ui->radioButtonAlsa->click();
    }
}


void MainWindow::slot_getAlsaDevices( bool value )
{
    Q_UNUSED(value);
    QStringList pulseDeviceStringList;
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices( QAudio::AudioInput ) )
    {
        if ( deviceInfo.deviceName().contains("alsa") == false)
        {
            pulseDeviceStringList << deviceInfo.deviceName();
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( deviceInfo.deviceName() );
            checkboxAudioDevice->setAccessibleName( deviceInfo.deviceName() );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice" + deviceInfo.deviceName() );
            ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count()-1, checkboxAudioDevice );
        }
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
        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer
                   << "xid=" + QString::number( vkWinInfo->getWinID() );

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
                   << "startx=" + QString::number( regionChoise->getXRecordArea() )
                   << "starty=" + QString::number( regionChoise->getYRecordArea() )
                   << "endx="   + QString::number( regionChoise->getXRecordArea() + regionChoise->getWidth()-1 )
                   << "endy="   + QString::number( regionChoise->getYRecordArea() + regionChoise->getHeight()-1 );
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
                   << "x=" + QString::number( regionChoise->getXRecordArea() )
                   << "y=" + QString::number( regionChoise->getYRecordArea() )
                   << "width=" + QString::number( regionChoise->getWidth() )
                   << "height=" + QString::number( regionChoise->getHeight() );
        value = stringList.join( " " );
    }

    return value;
}
#endif


QString MainWindow::VK_getCapsFilter()
{
   QStringList stringList;
   stringList << "caps=video/x-raw"
              << QString("framerate=") + ui->spinBoxFrames->text() + "/1";
   return QString( "capsfilter" ) + " " + stringList.join( "," );
}


// Check format, video and audoicodec on tab availability
void MainWindow::VK_gst_Elements_available()
{
    for ( int i = 0; i < globalFormatsList.count(); i++ )
    {
        QHBoxLayout *HBoxLayout_for_Format_and_Encoder_Layouts = nullptr;
        QVBoxLayout *VBoxLayout_for_Encoder = nullptr;
        QStringList listElements = QString( globalFormatsList.at(i) ).split( "," );
        for ( int x = 0; x < listElements.count(); x++ )
        {
            bool available;
            const gchar *element = QString( listElements.at( x ) ).section( ":", 1, 1 ).toLatin1();
            GstElementFactory *factory = gst_element_factory_find( element );
            if ( !factory )
            {
                //ui->verticalLayoutAvailableNotInstalled->insertWidget( ui->verticalLayoutAvailableNotInstalled->count()-2, label );
                available = false;
            }
            else
            {
                available = true;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "muxer" )
            {
                HBoxLayout_for_Format_and_Encoder_Layouts = new QHBoxLayout();

                QHBoxLayout *HBoxLayout_for_Picture_and_Format = new QHBoxLayout();
                QVBoxLayout *VBoxLayout_for_Format = new QVBoxLayout();
                VBoxLayout_for_Format->addItem( HBoxLayout_for_Picture_and_Format );

                VBoxLayout_for_Encoder = new QVBoxLayout();

                HBoxLayout_for_Format_and_Encoder_Layouts->addItem( VBoxLayout_for_Format);
                HBoxLayout_for_Format_and_Encoder_Layouts->addItem( VBoxLayout_for_Encoder);

                QLabel *labelFormat = new QLabel();
                labelFormat->show();
                labelFormat->setText( QString( listElements.at( x ) ).section( ":", 2, 2 ).prepend( " " ) );
                HBoxLayout_for_Picture_and_Format->addWidget( labelFormat );
                VBoxLayout_for_Format->addItem( new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding ) );

                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = labelPicture->style()->standardIcon( QStyle::SP_MessageBoxCritical );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                HBoxLayout_for_Picture_and_Format->insertWidget( 0, labelPicture );
                HBoxLayout_for_Picture_and_Format->addItem( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );

            }

            if ( ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "videocodec" ) or
                 ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "audiocodec" ) )
            {
                QHBoxLayout *HBoxLayout_for_Picture_and_Encoder = new QHBoxLayout();
                VBoxLayout_for_Encoder->addItem( HBoxLayout_for_Picture_and_Encoder );
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = labelPicture->style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = labelPicture->style()->standardIcon( QStyle::SP_MessageBoxCritical );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                HBoxLayout_for_Picture_and_Encoder->addWidget( labelPicture );

                QLabel *labelEncoder = new QLabel();
                labelEncoder->show();
                labelEncoder->setText( QString( listElements.at(x) ).section( ":", 2, 2 ).prepend( " " ) );
                HBoxLayout_for_Picture_and_Encoder->addWidget( labelEncoder );
                HBoxLayout_for_Picture_and_Encoder->addItem( new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
            }
        }
        QFrame *line = new QFrame();
        line->setObjectName( QStringLiteral("line") );
        line->setFrameShape( QFrame::HLine );
        line->setFrameShadow( QFrame::Sunken );
        ui->verticalLayoutAvailableInstalled->insertWidget( ui->verticalLayoutAvailableInstalled->count() -1, line );
        ui->verticalLayoutAvailableInstalled->insertLayout( ui->verticalLayoutAvailableInstalled->count() -2, HBoxLayout_for_Format_and_Encoder_Layouts );
    }
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
                                   );

    videoFormatsList.clear();
    videoFormatsList.append( MKV_QStringList.join( ","  ) );
    videoFormatsList.append( WEBM_QStringList.join( ","  ) );
    videoFormatsList.append( AVI_QStringList.join( "," ) );

    globalFormatsList << videoFormatsList;

    /*
        videoFormatsList.append( "mp4mux:mp4" );
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
        value = "x264enc speed-preset=veryfast pass=quant threads=0";
    }

    if ( encoder == "vp8enc" )
    {
        value = "vp8enc min_quantizer=20 max_quantizer=20 cpu-used=4 deadline=1000000 threads=4";
    }

    return value;
}


void MainWindow::slot_preStart()
{
    if ( ( ui->spinBoxCountDown->value() > 0 ) and ( ui->radioButtonWindow->isChecked() == true )  )
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

    if ( ui->spinBoxCountDown->value() > 0 )
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

    if ( ui->radioButtonWindow->isChecked() == true )
    {
        disconnect( vkWinInfo, 0, 0, 0 );
        connect( vkWinInfo, SIGNAL( signal_showCursor( bool ) ), ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_showCursor( bool ) ), ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ),     ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ),     ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ),     this,                SLOT( slot_Start() ) );
        vkWinInfo->slot_start();
        return;
    }


    if ( ui->radioButtonArea->isChecked() == true )
    {
       regionChoise->recordMode( true );
       QTest::qSleep(100);
       regionChoise->repaint();
       regionChoise->update();
    }
    slot_Start();
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


// Audiosystem is Pulse, Alsa, etc.
QString MainWindow::VK_get_AudioSystem()
{
    QString audioSystem;
    if ( ui->radioButtonPulse->isChecked() )
    {
        audioSystem = ui->radioButtonPulse->accessibleName();
    }
    if ( ui->radioButtonAlsa->isChecked() )
    {
        audioSystem = ui->radioButtonAlsa->accessibleName();
    }
    return audioSystem;
}


QString MainWindow::VK_getMuxer()
{
    QString value = ui->comboBoxFormat->currentData().toString();
    if ( ( ui->checkBoxAudioOnOff->isChecked() == true ) and ( !VK_get_AudioDevice().isEmpty() ) and ( ui->comboBoxAudioCodec->count() > 0  ) )
    {
        value = "mux. " + ui->comboBoxFormat->currentData().toString() + " name=mux";
    }

    return value;
}


void MainWindow::slot_Start()
{
    QString filename = "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
    QString path = ui->lineEditVideoPath->text();

    QStringList VK_PipelineList;
    VK_PipelineList << VK_getXimagesrc();
    VK_PipelineList << VK_getCapsFilter();
    VK_PipelineList << "queue flush-on-eos=true";
    VK_PipelineList << "videoconvert";
    VK_PipelineList << "videorate";
    VK_PipelineList << Vk_get_Videocodec_Encoder();

    if ( ( ui->checkBoxAudioOnOff->isChecked() == true ) and ( !VK_get_AudioDevice().isEmpty() ) and ( ui->comboBoxAudioCodec->count() > 0  ) )
    {
        VK_PipelineList << QString( "mux. ").append( VK_get_AudioSystem() ).append( " device=" ).append( VK_get_AudioDevice() );
        VK_PipelineList << "audioconvert";
        VK_PipelineList << "audiorate";
        VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
        VK_PipelineList << "queue flush-on-eos=true";
    }

    VK_PipelineList << VK_getMuxer();
    VK_PipelineList << "filesink location=" + path + "/" + filename;

    QString VK_Pipeline = VK_PipelineList.join( VK_Gstr_Pipe );
    qDebug() << "[vokoscreen] Start record with:" << VK_Pipeline;
    pipeline = gst_parse_launch( VK_Pipeline.toLatin1(), &error );

    // Start playing
    GstStateChangeReturn ret;
    ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
    if ( ret == GST_STATE_CHANGE_FAILURE )
    {
        g_printerr("[vokoscreen] Unable to set the pipeline to the playing state.\n");
        gst_object_unref( pipeline );
        return;
    }
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
    if ( ui->radioButtonArea->isChecked() == true )
    {
        regionChoise->recordMode( false );
        regionChoise->repaint();
        regionChoise->update();
    }
}


void MainWindow::slot_Stop()
{
    {
        // wait for EOS
        bool a = gst_element_send_event (pipeline, gst_event_new_eos());
        Q_UNUSED(a);

        GstClockTime timeout = 10 * GST_SECOND;
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
}


void MainWindow::slot_Pause()
{
    if ( ui->pushButtonStart->isEnabled() == false )
    {
        qDebug() << "[vokoscreen] Pause was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
        Q_UNUSED(ret);
    }
}


void MainWindow::slot_Continue()
{
    if ( ( ui->pushButtonStart->isEnabled() == false ) and ( ui->pushButtonContinue->isEnabled() == true ) )
    {
        qDebug() << "[vokoscreen] Continue was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        Q_UNUSED(ret);
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

void MainWindow::slot_screenCountChanged( int newCount )
{
    Q_UNUSED(newCount);
    ui->comboBoxScreen->clear();
    QDesktopWidget *desk = QApplication::desktop();
    qDebug() << "[vokoscreen]" << "---Begin search Screen---";
    qDebug() << "[vokoscreen]" << "Number of screens:" << desk->screenCount();
    qDebug() << "[vokoscreen] Primary screen is: Display" << desk->primaryScreen()+1;
    qDebug() << "[vokoscreen] VirtualDesktop:" << desk->isVirtualDesktop();

    //QList < QScreen *> screens = QGuiApplication::screens();
    QScreen *screen = QGuiApplication::primaryScreen();
    qDebug() << "[vokoscreen] DevicePixelRatio:" << screen->devicePixelRatio() << " (Normal displays is 1, Retina display is 2)";

    for ( int i = 1; i < desk->screenCount()+1; i++ )
    {
        QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).left() * screen->devicePixelRatio() );
        QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).top() * screen->devicePixelRatio() );
        QString ScreenGeometryWidth = QString::number( desk->screenGeometry( i-1 ).width() * screen->devicePixelRatio() );
        QString ScreenGeometryHeight = QString::number( desk->screenGeometry( i-1 ).height() * screen->devicePixelRatio() );
        QString stringText = tr( "Display" ) + " " + QString::number( i ) + ":  " + ScreenGeometryWidth + " x " + ScreenGeometryHeight;
        QString stringData = "x=" + ScreenGeometryX + " " +
                             "y=" + ScreenGeometryY + " " +
                             "with=" + ScreenGeometryWidth + " " +
                             "height=" + ScreenGeometryHeight;
        ui->comboBoxScreen->addItem( stringText, stringData );
        qDebug().noquote() << "[vokoscreen]" <<  ui->comboBoxScreen->itemText(i-1) << "     " << ui->comboBoxScreen->itemData(i-1).toString();
    }

    ui->comboBoxScreen->addItem( tr( "All Displays" ), -1 );
    qDebug() << "[vokoscreen]" << "---End search Screen---";
    qDebug( " " );
}
