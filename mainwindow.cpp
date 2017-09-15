#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <gst/gst.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug().noquote() << VK_GStreamer_Version();

    VK_Start();
}

MainWindow::~MainWindow()
{
    delete ui;
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


// gstreamer-plugins-bad-orig-addon
// gstreamer-plugins-good-extra
// libgstinsertbin-1_0-0
void MainWindow::VK_Start()
{
    GstElementFactory *factory = gst_element_factory_find( "ximagesrc" );
    if ( !factory )
    {
      qDebug() << "Failed to find factory of type ximagesrc";
      return;
    }

    GstElement *pipeline, *source, *videoconvert, *x264enc, *matroskamux, *filesink;

    /* Create the elements */
    source = gst_element_factory_make ("ximagesrc", "source1111");
    videoconvert = gst_element_factory_make ("videoconvert", "videoconvert");
    x264enc = gst_element_factory_make ("x264enc", "x264enc");
    matroskamux = gst_element_factory_make ("matroskamux", "matroskamux");
    filesink = gst_element_factory_make ("filesink", "filesink");

    filesink = gst_element_factory_make ("filesink",  "filesink");
    g_object_set (G_OBJECT (filesink), "location", "/home/vk/Videos/vokoscreen.mkv", NULL);


    // Create the empty pipeline
    pipeline = gst_pipeline_new ("test-pipeline");

    if (!pipeline || !source || !videoconvert || !x264enc || !matroskamux || !filesink)
    {
      g_printerr ("Not all elements could be created.\n");
      return;
    }

    // Build the pipeline
    gst_bin_add_many (GST_BIN (pipeline), source, videoconvert, x264enc, matroskamux, filesink, NULL);

    if (gst_element_link_many (source, videoconvert, x264enc, matroskamux, filesink, NULL ) != TRUE)
    {
      g_printerr ("Elements could not be linked.\n");
      gst_object_unref (pipeline);
      return;
    }

    // Start playing
    GstStateChangeReturn ret;
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      g_printerr ("Unable to set the pipeline to the playing state.\n");
      gst_object_unref (pipeline);
      return;
    }


    gst_object_unref (GST_OBJECT (factory));
}

