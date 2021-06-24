#include "mainWindow_wl.h"
#include "global.h"
#include "QvkLogController.h"

#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QThread>

QvkMainWindow_wl::QvkMainWindow_wl( QWidget *parent, Qt::WindowFlags f )
    : QMainWindow(parent, f)
    , ui(new Ui::formMainWindow_wl)
{
    ui->setupUi( this );

    QvkLogController *vklogController = new QvkLogController();
    connect( vklogController, SIGNAL( signal_newLogText( QString ) ), this, SLOT( slot_textToGuiLog( QString ) ) );

    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );

    ui->tabWidget->removeTab(1);
    ui->frame_information->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonContinue->hide();
    ui->pushButtonPlay->hide();
    ui->pushButtonScreencastOpenfolder->hide();

    setSpezialSlider();
    setConnects();
}


QvkMainWindow_wl::~QvkMainWindow_wl()
{
}


void QvkMainWindow_wl::slot_textToGuiLog( QString value )
{
    ui->textBrowserLog->append( value );
}


void QvkMainWindow_wl::setConnects()
{

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStop, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( slot_start() ) );

    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), this, SLOT( slot_stop() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStop, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStart, SLOT( setDisabled( bool ) ) );

    connect( portal_wl, SIGNAL( signal_fd_path( QString, QString ) ), this, SLOT( slot_start_gst( QString, QString ) ) );

    connect( ui->toolButtonFramesReset, SIGNAL( clicked( bool ) ), this, SLOT( slot_framesReset() ) );
}


QString QvkMainWindow_wl::Vk_get_Videocodec_Encoder()
{
    QString value;
//    QString encoder = "openh264enc"; //ui->comboBoxVideoCodec->currentData().toString();
    QString encoder = "x264enc"; //ui->comboBoxVideoCodec->currentData().toString();

    if ( encoder == "openh264enc" )
    {
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

    if ( encoder == "x264enc" )
    {
        QStringList list;
        list << encoder;
        list << "qp-min=17"; // + QString::number( sliderOpenh264->value() );
        list << "qp-max=17"; // + QString::number( sliderOpenh264->value() );
        list << "speed-preset=superfast";
        list << "threads=" + QString::number( QThread::idealThreadCount() );
        value = list.join( " " );
        value.append( " ! video/x-h264, profile=baseline" );
    }

    return value;
}


void QvkMainWindow_wl::slot_start()
{
    qDebug().noquote() << global::nameOutput << "start";
    portal_wl->requestScreenSharing();
}


void QvkMainWindow_wl::slot_start_gst( QString vk_fd, QString vk_path )
{
    QStringList pipeline;
    pipeline << QString( "pipewiresrc fd=" ).append( vk_fd ).append( " path=" ).append( vk_path ).append( " do-timestamp=true" );
    pipeline << "videoconvert";
    pipeline << "videorate";
    pipeline << "video/x-raw, framerate=" + QString::number( sliderFrames->value() ) + "/1";
    pipeline << Vk_get_Videocodec_Encoder();
    pipeline << "matroskamux name=mux";

    QString newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + ".mkv";// + ui->comboBoxFormat->currentText();
    pipeline << "filesink location=\"" + QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) + "/" + newVideoFilename + "\"";

    QString launch = pipeline.join( " ! " );
    qDebug() << global::nameOutput << launch;

    vk_gstElement = gst_parse_launch( launch.toUtf8(), nullptr );
    gst_element_set_state( vk_gstElement, GST_STATE_PLAYING );
}


void QvkMainWindow_wl::slot_stop()
{
    // send EOS to pipeline
    gst_element_send_event( vk_gstElement, gst_event_new_eos() );

    // wait for the EOS to traverse the pipeline and is reported to the bus
    GstBus *bus = gst_element_get_bus( vk_gstElement );
    gst_bus_timed_pop_filtered( bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS );

    gst_element_set_state( vk_gstElement, GST_STATE_NULL );

    qDebug() << global::nameOutput << "Stop record";
}


void QvkMainWindow_wl::setSpezialSlider()
{
    sliderFrames = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_slider_frames->insertWidget( 0, sliderFrames );
    sliderFrames->setObjectName( "sliderFrames" );
    sliderFrames->setTracking( true );
    sliderFrames->setMinimum( 10 );
    sliderFrames->setMaximum( 144 );
    sliderFrames->setValue( 25 );
    sliderFrames->show();
}


void QvkMainWindow_wl::slot_framesReset()
{
    sliderFrames->setValue( 25 );
}
