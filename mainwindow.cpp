#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QScreen>
#include <QDesktopWidget>

#include <gst/gst.h>

// gstreamer-plugins-bad-orig-addon
// gstreamer-plugins-good-extra
// libgstinsertbin-1_0-0
// gst_parse_launch --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gstreamer-GstParse.html#gst-parse-launch-full
// ximagesrc        --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good-plugins/html/gst-plugins-good-plugins-ximagesrc.html
// videoconvert     --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-videoconvert.html
// x264enc          --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-ugly-plugins/html/gst-plugins-ugly-plugins-x264enc.html
// matroskamux      --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good/html/gst-plugins-good-plugins-matroskamux.html
// filesink         --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer-plugins/html/gstreamer-plugins-filesink.html

#include <gst/gst.h>
#include <gst/gstprotection.h>
#include <glib.h>

static gboolean my_bus_func (GstBus * bus, GstMessage * message, gpointer user_data)
{
   Q_UNUSED(bus);
   Q_UNUSED(user_data);
   GstDevice *device;
   gchar *name;

   switch (GST_MESSAGE_TYPE (message)) {
     case GST_MESSAGE_DEVICE_ADDED:{
       gst_message_parse_device_added (message, &device);
       name = gst_device_get_display_name (device);
       g_print("Device added: %s\n", name);
       g_free (name);
       gst_object_unref (device);
       break; }
     case GST_MESSAGE_DEVICE_REMOVED: {
       gst_message_parse_device_removed (message, &device);
       name = gst_device_get_display_name (device);
       gchar *classes = gst_device_get_device_class (device); // Volker
       g_print("Device removed: %s  %s\n", name, classes);       // Volker
       g_free (name);
       gst_object_unref (device);
       break; }
     default:
       break;
   }

   return G_SOURCE_CONTINUE;
}

GstDeviceMonitor *setup_raw_video_source_device_monitor() // hier darf kein void als Paramter stehen wie im Beispiel vermerkt
{
   GstDeviceMonitor *monitor;
   GstBus *bus;
   GstCaps *caps;

   monitor = gst_device_monitor_new ();

   bus = gst_device_monitor_get_bus (monitor);
   gst_bus_add_watch (bus, my_bus_func, NULL);
   gst_object_unref (bus);

   caps = gst_caps_new_empty_simple ("audio/x-raw");
   gst_device_monitor_add_filter (monitor, "Audio/Source", caps);
   gst_caps_unref (caps);

   gst_device_monitor_start (monitor);

   return monitor;
}


#include <QAudioDeviceInfo>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setup_raw_video_source_device_monitor();

    ui->setupUi(this);

    qDebug().noquote() << VK_GStreamer_Version();

    qDebug() << gst_version_string ();

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

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                      SLOT( VK_preStart() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonFullscreen, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonWindow,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonArea,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxScreen,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabAudio,              SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->tabCodec,              SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonContinue,    SLOT( setDisabled( bool ) ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( VK_Stop() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonFullscreen, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonWindow,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxScreen,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->tabAudio,              SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->tabCodec,              SLOT( setDisabled( bool ) ) );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), this,                   SLOT( VK_Pause() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonPause,    SLOT( hide() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( show() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonStop,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( setDisabled( bool ) ) );

    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this,                   SLOT( VK_Continue() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( hide() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonPause,    SLOT( show() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonStop,     SLOT( setDisabled( bool ) ) );
    ui->pushButtonContinue->hide();

    // Close
    connect( this, SIGNAL( signal_close() ),  ui->pushButtonContinue, SLOT( click() ) );
    connect( this, SIGNAL( signal_close() ),  ui->pushButtonStop, SLOT( click() ) );
    connect( this, SIGNAL( signal_close() ),  regionController, SLOT( close() ) );

    // Tab 1 Screen
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), regionController,   SLOT( show( bool ) ) );
    connect( ui->radioButtonArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreen, SLOT( setDisabled( bool ) ) );

    // Tab 2 Audio
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), this, SLOT( AudioIconOnOff( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->radioButtonPulse, SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->radioButtonAlsa, SLOT( setEnabled( bool ) ) );
    connect( ui->checkBoxAudioOnOff, SIGNAL( toggled( bool ) ), ui->scrollAreaAudioDevice, SLOT( setEnabled( bool ) ) );
    ui->checkBoxAudioOnOff->click();

    // Tab 3 Codec
    ui->pushButtonFramesDefault->setIcon ( QIcon::fromTheme( "edit-undo", QIcon( ":/pictures/undo.png" ) ) );
    connect( ui->pushButtonFramesDefault, SIGNAL( clicked( bool ) ), this, SLOT( setFramesStandard( bool ) ) );


    QDesktopWidget *desk = QApplication::desktop();
    connect( desk, SIGNAL( screenCountChanged(int) ), SLOT( myScreenCountChanged( int ) ) );
    connect( desk, SIGNAL( resized( int ) ),          SLOT( myScreenCountChanged( int ) ) );
    emit desk->screenCountChanged(0);


    QStringList pulseDeviceStringList;
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices( QAudio::AudioInput ) )
    {
        if ( deviceInfo.deviceName().contains("alsa") )
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

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    emit signal_close();
}


QString MainWindow::VK_GStreamer_Version()
{
    uint major, minor, micro, nano;
    QString nano_str;

    gst_version(&major, &minor, &micro, &nano);

    if (nano == 1)
      nano_str = "(CVS)";
    else if (nano == 2)
      nano_str = "(Prerelease)";
    else
      nano_str = "";

    QString gstrVersion = QString::number( major ).append( "." )
                 .append( QString::number( minor)).append( "." )
                 .append( QString::number( micro))
                 .append( nano_str );
    return gstrVersion;
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
void MainWindow::AudioIconOnOff( bool state )
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


void MainWindow::setFramesStandard( bool value )
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
   return QString( "capsfilter" ) + " " + stringList.join( "," );;
}

/*
void MainWindow::VK_Search_Video_Codecs()
{
    enum VideoCodecs{ x264enc, x265enc, mpeg2enc, theoraenc, vp8enc, vp9enc  }
}


void MainWindow::VK_Search_VideoFormats()
{
    enum VideoFormats{ matroskamux, webmmux, mp4mux, avimux, asfmux, flvmux }
}
*/

QString MainWindow::VK_getMuxer()
{
    GstElementFactory *factory = gst_element_factory_find( "matroskamux" );
    if ( !factory )
    {
      qDebug() << "Failed to find factory of type matroskamux";
    }
    QString value = "matroskamux";
    return value;
}


void MainWindow::VK_preStart()
{
    if ( ui->radioButtonWindow->isChecked() == true )
    {
      vkWinInfo = new QvkWinInfo();
      connect( vkWinInfo, SIGNAL( windowChanged() ), this, SLOT( VK_Start() ) );
      return;
    }

    VK_Start();
}


void MainWindow::VK_Start()
{
    GstElementFactory *factory = gst_element_factory_find( "ximagesrc" );
    if ( !factory )
    {
      qDebug() << "Failed to find factory of type ximagesrc";
      return;
    }

    QString filename = "vokoscreen-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + "mkv";
    QString path = QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
    QStringList VK_PipelineList;
    VK_PipelineList << VK_getXimagesrc()
                    << VK_getCapsFilter()
                    << "queue flush-on-eos=true"
                    << "videoconvert"
                    << "videorate" // Make a perfect stream? Relevant for screencasting?
                    << "x264enc speed-preset=veryfast pass=quant threads=0"
                    << VK_getMuxer();
/*
    if ( ui->checkBoxAudioOnOff->isChecked() == true )
    {

    }
*/

    VK_PipelineList.append( "filesink location=" + path + QDir::separator() + filename );


/*
    ! mux. pulsesrc device=alsa_input.usb-046d_0809_A6307261-02.analog-mono \
    ! audioconvert \
    ! voaacenc \
    ! queue flush-on-eos=true \
    ! mux. matroskamux name=mux
    ! filesink location=/home/vk/Videos/desktop.mkv
*/

    QString VK_Pipeline = VK_PipelineList.join( VK_Gstr_Pipe );
    qDebug() << "[vokoscreen] Start record with:" << VK_Pipeline;
    pipeline = gst_parse_launch( VK_Pipeline.toLatin1(), &error );

    // Start playing
    GstStateChangeReturn ret;
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr ("Unable to set the pipeline to the playing state.\n");
      gst_object_unref (pipeline);
      return;
    }
}


void MainWindow::VK_Stop()
{
    if ( ui->pushButtonStart->isEnabled() == false  )
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


void MainWindow::VK_Pause()
{
    if ( ui->pushButtonStart->isEnabled() == false )
    {
        qDebug() << "[vokoscreen] Pause was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
        Q_UNUSED(ret);
    }
}


void MainWindow::VK_Continue()
{
    if ( ( ui->pushButtonStart->isEnabled() == false ) and ( ui->pushButtonContinue->isEnabled() == true ) )
    {
        qDebug() << "[vokoscreen] Continue was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        Q_UNUSED(ret);
    }
}


void MainWindow::myScreenCountChanged(int newCount )
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
