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

// gstreamer-plugins-bad-orig-addon
// gstreamer-plugins-good-extra
// libgstinsertbin-1_0-0

#include <gst/gst.h>

QString MainWindow::get_AudioDeviceString( GstDevice * device )
{
  static const char *const ignored_propnames[] = { "name", "parent", "direction", "template", "caps", NULL };
  GString *launch_line;
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


#include <QStyleFactory>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "[vokoscreen]" << gst_version_string ();

/*
    QStringList list;
    list.append( "/home/vk/Dokumente/vokoscreenGST/breeze-icons/" );
    QIcon::setThemeSearchPaths( list );

    ui->tabWidget->setTabIcon( 0, QIcon::fromTheme( "vi", QIcon( "/home/vk/Dokumente/vokoscreenGST/breeze-icons/icons/devices/64/camera-web.svg" ) ) );

    const QStringList styles = QStyleFactory::keys();
    for(QString s : styles)
    {
        qDebug() << s;
    }

    ui->pushButtonStart->setIcon( ui->pushButtonStart->style()->standardIcon( QStyle::SP_MediaVolume ) );
*/

    ui->tabWidget->setTabIcon( 0, QIcon::fromTheme( "video-display", QIcon( ":/pictures/monitor.png" ) ) );
    makeAndSetValidIcon( 0 );

    ui->tabWidget->setTabIcon( 1, QIcon::fromTheme( "audio-input-microphone", QIcon( ":/pictures/micro.png" ) ) );
    makeAndSetValidIcon( 1 );

    ui->tabWidget->setTabIcon( 2, QIcon::fromTheme( "applications-multimedia", QIcon( ":/pictures/videooptionen.png" ) ) );
    makeAndSetValidIcon( 2 );

    ui->tabWidget->setTabIcon( 3, QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/tools.png" ) ) );
    makeAndSetValidIcon( 3 );

    ui->tabWidget->setTabIcon( 4, QIcon::fromTheme( "camera-web", QIcon( ":/pictures/webcam.png" ) ) );
    makeAndSetValidIcon( 4 );

    regionController = new QvkRegionController();
    regionController->hide();

    vkWinInfo = new QvkWinInfo();

    vkCountdown = new QvkCountdown();

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
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabAudio,              SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabCodec,              SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabMisc,               SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStart() ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonFullscreen, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonWindow,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_comboBoxScreenSetDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->tabAudio,              SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->tabCodec,              SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->tabMisc,               SLOT( setDisabled( bool ) ) );
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

    // Close
    connect( this, SIGNAL( signal_close() ),  ui->pushButtonContinue, SLOT( click() ) );
    connect( this, SIGNAL( signal_close() ),  ui->pushButtonStop, SLOT( click() ) );
    connect( this, SIGNAL( signal_close() ),  regionController, SLOT( close() ) );

    // Tab 1 Screen
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), regionController,   SLOT( show( bool ) ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );

    // Tab 2 Audio
    ui->toolButtonAudioHelp->setIcon( ui->pushButtonStart->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );
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
    videoFileSystemWatcher = new QFileSystemWatcher( (QStringList)QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    connect( ui->PushButtonVideoPath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newVideoPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    connect( videoFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );

    VK_Supported_Formats_And_Codecs();
    VK_Check_is_Format_available();
    VK_set_available_Formats_in_Combox();


    QDesktopWidget *desk = QApplication::desktop();
    connect( desk, SIGNAL( screenCountChanged(int) ), SLOT( slot_screenCountChanged( int ) ) );
    connect( desk, SIGNAL( resized( int ) ),          SLOT( slot_screenCountChanged( int ) ) );
    emit desk->screenCountChanged(0);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slot_audioHelp()
{
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/3.0/audio.html", QUrl::TolerantMode ) );
}


void MainWindow::slot_comboBoxScreenSetDisabled( bool value )
{
    Q_UNUSED(value);
    if ( ui->radioButtonFullscreen->isChecked() == true )
    {
        ui->comboBoxScreen->setEnabled( true );
    }
    else
    {
        ui->comboBoxScreen->setEnabled( false );
    }
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
    videoFileSystemWatcher->removePaths( videoFileSystemWatcher->directories() );
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
}


void MainWindow::makeAndSetValidIcon( int index )
{
  QIcon myIcon = ui->tabWidget->tabIcon( index );
  QSize size = ui->tabWidget->iconSize();
  QPixmap workPixmap( myIcon.pixmap( size ) );
  ui->tabWidget->setTabIcon( index, QIcon( workPixmap ) );
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
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContents->findChildren<QCheckBox *>();
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


QString MainWindow::VK_getXimagesrc()
{
    QString showPointer = "true";
    if( ui->checkBoxMouseCursorOnOff->checkState() == Qt::Checked )
    {
        showPointer = "false";
    }

    if( ui->radioButtonFullscreen->isChecked() == true )
    {
        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer;

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
                   << "startx=" + QString::number( regionController->getX() )
                   << "starty=" + QString::number( regionController->getY() )
                   << "endx="   + QString::number( regionController->getX() + regionController->getWidth() )
                   << "endy="   + QString::number( regionController->getY() + regionController->getHeight() );
        QString value = stringList.join( " " );
        return value;
    }

    return "";
}


QString MainWindow::VK_getCapsFilter()
{
   QStringList stringList;
   stringList << "caps=video/x-raw"
              << QString("framerate=") + ui->spinBoxFrames->text() + "/1";
   return QString( "capsfilter" ) + " " + stringList.join( "," );
}


void MainWindow::VK_Supported_Formats_And_Codecs()
{
    QStringList MKV_QStringList = ( QStringList()
                                    << "muxer:matroskamux"
                                    << "suffix:mkv"
                                    << "videomimetype:video/x-matroska"
                                    << "audiomimetype:audio/x-matroska"
                                    << "videocodec:x264enc:x264"
                                    << "videocodec:vp8enc:vp8"
                                    << "audiocodec:vorbisenc:vorbis"
                                    << "audiocodec:flacenc:flac"
                                    << "audiocodec:opusenc:opus"
                                   );

    QStringList WEBM_QStringList = ( QStringList()
                                     << "muxer:webmmux"
                                     << "suffix:webm"
                                     << "videomimetype:video/webm"
                                     << "audiomimetype:audio/webm"
                                     << "videocodec:vp8enc:vp8"
                                     << "audiocodec:vorbisenc:vorbis"
                                     << "audiocodec:opusenc:opus"
                                   );

    QStringList AVI_QStringList = ( QStringList()
                                     << "muxer:avimux"
                                     << "suffix:avi"
                                     << "videomimetype:video/x-msvideo"
                                     << "audiomimetype:audio/x-msvideo"
                                     << "videocodec:x264enc:x264"
                                     << "videocodec:vp8enc:vp8"
                                   );

    videoFormatsList.clear();
    videoFormatsList.append( MKV_QStringList.join( ","  ) );
    videoFormatsList.append( WEBM_QStringList.join( ","  ) );
    videoFormatsList.append( AVI_QStringList.join( "," ) );


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
        const gchar *muxer = QString( listKey.at( 0 ) ).section( ":", 1 ).toLatin1();
        GstElementFactory *factory = gst_element_factory_find( muxer );
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
        QStringList listKeySuffix = listKeys.filter( "suffix");
        QStringList listKeyMuxer = listKeys.filter( "muxer" );

        QMimeDatabase mimeDatabase;
        QStringList listKeyVideoMimetype = listKeys.filter( "videomimetype" );
        QMimeType mimetype = mimeDatabase.mimeTypeForName( QString( listKeyVideoMimetype.at( 0 ) ).section( ":", 1 ) );
        QIcon icon = QIcon::fromTheme( mimetype.iconName(), QIcon( ":/pictures/videooptionen.png" ) );

        ui->comboBoxFormat->addItem( QIcon::fromTheme( mimetype.iconName(), QIcon( ":/pictures/videooptionen.png" ) ),
                                     QString( listKeySuffix.at( 0 ) ).section( ":", 1 ),
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 1 ) );
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
        connect( vkWinInfo,   SIGNAL( windowChanged( bool ) ),          this,                SLOT( slot_startCounter( bool ) ) );
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
        connect( vkWinInfo, SIGNAL( windowChanged( bool ) ),     ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( windowChanged( bool ) ),     ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( windowChanged( bool ) ),     this,                SLOT( slot_Start() ) );
        vkWinInfo->slot_start();
        return;
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
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContents->findChildren<QCheckBox *>();
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
    VK_PipelineList << "filesink location=" + path + QDir::separator() + filename;

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
    string.append( "file://" );
    string.append( ui->lineEditVideoPath->text() );
    string.append( QDir::separator() );
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
        QString ScreenGeometryX1 = QString::number( desk->screenGeometry( i-1 ).left() );
        QString ScreenGeometryY1 = QString::number( desk->screenGeometry( i-1 ).top() );
        QString ScreenGeometryX = QString::number( desk->screenGeometry( i-1 ).width() * screen->devicePixelRatio() ); // devicePixelRatio() for Retina Displays
        QString ScreenGeometryY = QString::number( desk->screenGeometry( i-1 ).height() * screen->devicePixelRatio() );
        ui->comboBoxScreen->addItem( tr( "Display" ) + " " + QString::number( i ) + ":  " + ScreenGeometryX + " x " + ScreenGeometryY, i-1 );
        qDebug().noquote() << "[vokoscreen]" << "Display " + QString::number( i ) + ":  " + ScreenGeometryX + " x " + ScreenGeometryY;
    }
    ui->comboBoxScreen->addItem( tr( "All Displays" ), -1 );
    qDebug() << "[vokoscreen]" << "---End search Screen---";
    qDebug( " " );
}
