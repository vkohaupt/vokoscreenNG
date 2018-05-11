#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <gst/gst.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug().noquote() << VK_GStreamer_Version();

    qDebug() << gst_version_string ();

    regionController = new QvkRegionController();
    regionController->hide();

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( VK_Start() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), this, SLOT( VK_Stop() ) );
    connect( ui->radioButtonArea, SIGNAL( toggled( bool ) ), regionController, SLOT( show( bool ) ) );
    connect( this,                SIGNAL( close() ),         regionController, SLOT( close() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent * event )
{
    emit close();
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


QString MainWindow::VK_getXimagesrc()
{
   if( ui->radioButtonFullscreen->isChecked() == true )
   {
       QString value = "ximagesrc use-damage=false";
       return value;
   }

   if( ui->radioButtonWindow->isChecked() == true )
   {

   }

   if ( ui->radioButtonArea->isChecked() == true )
   {
       QStringList stringList;
       stringList << "ximagesrc"
                  << "use-damage=false"
                  << "startx=" + QString::number( regionController->getX() )
                  << "starty=" + QString::number( regionController->getY() )
                  << "endx="   + QString::number( regionController->getX() + regionController->getWidth() )
                  << "endy="   + QString::number( regionController->getY() + regionController->getHeight() );
       QString value = stringList.join( " " );
       return value;
   }
}


QString MainWindow::VK_getMuxer()
{
    QString value = "matroskamux";
    return value;
}


// gstreamer-plugins-bad-orig-addon
// gstreamer-plugins-good-extra
// libgstinsertbin-1_0-0
// gst_parse_launch --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/gstreamer-GstParse.html#gst-parse-launch-full
// ximagesrc        --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good-plugins/html/gst-plugins-good-plugins-ximagesrc.html
// ximagesrc        --> Element Pads --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good-plugins/html/gst-plugins-good-plugins-ximagesrc.html
// videoconvert     --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-videoconvert.html
// x264enc          --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-ugly-plugins/html/gst-plugins-ugly-plugins-x264enc.html
// matroskamux      --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-good/html/gst-plugins-good-plugins-matroskamux.html
// filesink         --> https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer-plugins/html/gstreamer-plugins-filesink.html

//use-damage=false
void MainWindow::VK_Start()
{
    GstElementFactory *factory = gst_element_factory_find( "ximagesrc" );
    if ( !factory )
    {
      qDebug() << "Failed to find factory of type ximagesrc";
      return;
    }

    QStringList VK_PipelineList;
    VK_PipelineList << VK_getXimagesrc()
                    << "video/x-raw, framerate=25/1"
                    << "videoconvert"
                    << "x264enc speed-preset=veryfast pass=quant threads=0"
                    << VK_getMuxer()
                    << "filesink location=/home/vk/Videos/desktop.mkv";

    QString VK_Pipeline = VK_PipelineList.join( VK_Gstr_Separator );
    qDebug() << VK_Pipeline;
    pipeline = gst_parse_launch( VK_Pipeline.toLatin1(), &error );


/*
    // Create the elements
    source = gst_element_factory_make ("ximagesrc", "source");
    g_object_set (G_OBJECT (source), "show-pointer", true, NULL);

    //https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/GstCaps.html
    //http://comments.gmane.org/gmane.comp.video.gstreamer.devel/53765
    GstCaps *caps = gst_caps_new_simple ("video/x-raw",
                                         "format", G_TYPE_STRING, "I420",
                                         "framerate", GST_TYPE_FRACTION, 25, 1,
                                         "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                         "width", G_TYPE_INT, 320,
                                         "height", G_TYPE_INT, 240,
                                         NULL);

    videoconvert = gst_element_factory_make ("videoconvert", "videoconvert");

    // Preset found with, gst-inspect-1.0 x264enc
    enum x264preset { None, ultrafast, superfast, veryfast, faster, medium, slow, slower, veryslow, placebo };
    VK_VideoEncoder = gst_element_factory_make ("x264enc", "x264enc");
    g_object_set (G_OBJECT (VK_VideoEncoder), "speed-preset", x264preset::medium , NULL);

    VK_VideoQueue = gst_element_factory_make ("queue2", "videoq");

    matroskamux = gst_element_factory_make ("matroskamux", "matroskamux");

    filesink = gst_element_factory_make ("filesink",  "filesink");
    g_object_set (G_OBJECT (filesink), "location", "/home/vk/Videos/vokoscreen.mkv", NULL);

    // Create the empty pipeline
    pipeline = gst_pipeline_new ("test-pipeline");

    if (!pipeline || !source || !caps || !videoconvert || !VK_VideoEncoder || !VK_VideoQueue || !matroskamux || !filesink)
    {
      g_printerr ("Not all elements could be created.\n");
      return;
    }

    // Build the pipeline
    gst_bin_add_many (GST_BIN (pipeline), source, videoconvert, VK_VideoEncoder, VK_VideoQueue, matroskamux, filesink, NULL);

    if (gst_element_link_many (source, videoconvert, VK_VideoEncoder, VK_VideoQueue, matroskamux, filesink, NULL ) != TRUE)
    {
      g_printerr ("Elements could not be linked.\n");
      gst_object_unref (pipeline);
      return;
    }
*/

    // https://gstreamer.freedesktop.org/documentation/application-development/basics/helloworld.html

    // Start playing
    GstStateChangeReturn ret;
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    qDebug() << ret;
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr ("Unable to set the pipeline to the playing state.\n");
      gst_object_unref (pipeline);
      return;
    }

    /* Iterate */
    //g_print ("Running...\n");
    //g_main_loop_run (loop);
}


void MainWindow::VK_Stop()
{
    // http://gstreamer-devel.narkive.com/AMLXdRKP/how-can-i-tell-if-all-elements-received-the-eos
    // wait for EOS
    bool a = gst_element_send_event (pipeline, gst_event_new_eos());
    qDebug() << a;
    GstClockTime timeout = 10 * GST_SECOND;
    GstMessage *msg;
    msg = gst_bus_timed_pop_filtered (GST_ELEMENT_BUS (pipeline), timeout, GST_MESSAGE_EOS );

    GstStateChangeReturn ret ;
    ret = gst_element_set_state (pipeline, GST_STATE_PAUSED);
    qDebug() << ret;
    ret = gst_element_set_state (pipeline, GST_STATE_READY);
    qDebug() << ret;
    ret = gst_element_set_state (pipeline, GST_STATE_NULL);
    qDebug() << ret;
    gst_object_unref (pipeline);
}

