#include "mainWindow_wl.h"
#include "global.h"
#include "QvkLogController.h"

#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QThread>
#include <QMimeDatabase>

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

    set_Connects();

    supported_Formats_And_Codecs();
    gst_Elements_available();
    check_is_Format_available();

    set_SpezialSlider();
    set_available_Formats_in_Combox();


    ui->tabWidget->removeTab(1);
    ui->frame_information->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonContinue->hide();
    ui->pushButtonPlay->hide();
    ui->pushButtonScreencastOpenfolder->hide();
}


QvkMainWindow_wl::~QvkMainWindow_wl()
{
}


void QvkMainWindow_wl::slot_textToGuiLog( QString value )
{
    ui->textBrowserLog->append( value );
}


void QvkMainWindow_wl::set_Connects()
{

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStop, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( slot_start() ) );

    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), this, SLOT( slot_stop() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStop, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStart, SLOT( setDisabled( bool ) ) );

    connect( portal_wl, SIGNAL( signal_fd_path( QString, QString ) ), this, SLOT( slot_start_gst( QString, QString ) ) );

    connect( ui->toolButtonFramesReset, SIGNAL( clicked( bool ) ), this, SLOT( slot_framesReset() ) );

    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_VideoCodecs_in_Combox( QString ) ) );
}


QString QvkMainWindow_wl::get_Videocodec_Encoder()
{
    QString value;
    QString encoder = ui->comboBoxVideoCodec->currentData().toString();

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

    if ( encoder == "vp8enc" )
    {
        QStringList list;
        list << "vp8enc";
        list << "min_quantizer=20"; // + QString::number( sliderVp8->value() );
        list << "max_quantizer=20";  // + QString::number( sliderVp8->value() );
        list << "cpu-used=" + QString::number( QThread::idealThreadCount() );
        list << "deadline=1000000";
        list << "threads=" + QString::number( QThread::idealThreadCount() );
        value = list.join( " " );
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
    pipeline << get_Videocodec_Encoder();
    pipeline << "matroskamux name=mux";

    QString newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
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


void QvkMainWindow_wl::set_SpezialSlider()
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


void QvkMainWindow_wl::gst_Elements_available()
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

    for ( int i = 0; i < list.count(); i++ )
    {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << list.at(i);
            gst_object_unref( factory );
        }
    }
    qDebug();
}

// This is the base for format, video and audiocodec
void QvkMainWindow_wl::supported_Formats_And_Codecs()
{
    QStringList MKV_QStringList = ( QStringList()
                                    << "muxer:matroskamux:mkv"
                                    << "videomimetype:video/x-matroska"
                                    << "audiomimetype:audio/x-matroska"
                                    << "videocodec:openh264enc:H.264"
                                    << "videocodec:x264enc:x264"
                                    << "videocodec:vp8enc:VP8"
                                    << "audiocodec:vorbisenc:vorbis"
                                    << "audiocodec:flacenc:flac"
                                    << "audiocodec:opusenc:opus"
                                    << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList WEBM_QStringList = ( QStringList()
                                     << "muxer:webmmux:webm"
                                     << "videomimetype:video/webm"
                                     << "audiomimetype:audio/webm"
                                     << "videocodec:vp8enc:VP8"
                                     << "audiocodec:vorbisenc:vorbis"
                                     << "audiocodec:opusenc:opus"
                                   );

    QStringList AVI_QStringList = ( QStringList()
                                     << "muxer:avimux:avi"
                                     << "videomimetype:video/x-msvideo"
                                     << "audiomimetype:audio/x-msvideo"
                                     << "videocodec:openh264enc:H.264"
                                     << "videocodec:x264enc:x264"
                                     << "videocodec:vp8enc:VP8"
                                     << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList MP4_QStringList = ( QStringList()
                                    << "muxer:mp4mux:mp4"
                                    << "videomimetype:video/mp4"
                                    << "audiomimetype:audio/mpeg"
                                    << "videocodec:openh264enc:H.264"
                                    << "videocodec:x264enc:x264"
                                    << "audiocodec:lamemp3enc:mp3"
                                    << "audiocodec:opusenc:opus"
                                    );

    // https://de.wikipedia.org/wiki/QuickTime
    QStringList MOV_QStringList = ( QStringList()
                                    << "muxer:qtmux:mov"
                                    << "videomimetype:video/mp4"
                                    << "audiomimetype:audio/mpeg"
                                    << "videocodec:openh264enc:H.264"
                                    << "videocodec:x264enc:x264"
                                    << "videocodec:vp8enc:VP8"
                                    << "audiocodec:lamemp3enc:mp3"
                                  );

    videoFormatsList.clear();
    videoFormatsList.append( MKV_QStringList.join( ","  ) );
    videoFormatsList.append( WEBM_QStringList.join( ","  ) );
    videoFormatsList.append( AVI_QStringList.join( "," ) );
    videoFormatsList.append( MP4_QStringList.join( ",") );
    videoFormatsList.append( MOV_QStringList.join( ",") );
}


void QvkMainWindow_wl::check_is_Format_available()
{
    qDebug().noquote() << global::nameOutput << "Symbols: + available, - not available";
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
            qDebug().noquote() << global::nameOutput << "-" << muxer;
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << muxer;
            tempList << videoFormatsList.at( x );
            gst_object_unref( factory );
        }
    }
    videoFormatsList.clear();
    videoFormatsList << tempList;
}


void QvkMainWindow_wl::set_available_Formats_in_Combox()
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
        QIcon icon = QIcon( ":/pictures/screencast/strip.png" );

        ui->comboBoxFormat->addItem( icon, // Picture
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ), // suffix
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 1, 1 ) ); // muxer
    }
}


void QvkMainWindow_wl::slot_set_available_VideoCodecs_in_Combox( QString suffix )
{
    ui->comboBoxVideoCodec->clear();

    QStringList listSuffix = videoFormatsList.filter( suffix );
    QString stringSuffix = listSuffix.at( 0 );
    QStringList listKeys = stringSuffix.split( "," );
    QStringList listKeyVideoCodec = listKeys.filter( "videocodec" );
    for ( int i = 0; i < listKeyVideoCodec.count(); i++ )
    {
        QString encoder = QString( listKeyVideoCodec.at( i ) ).section( ":", 1, 1 );

        QString name = QString( listKeyVideoCodec.at( i ) ).section( ":", 2, 2 );
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );

        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        }
        else
        {
            QString message = global::nameOutput + " + " + encoder;
            GstElement *source = gst_element_factory_create( factory, "source" );
            if ( !source )
            {
                message = global::nameOutput + " - " + encoder + " available but codec is missing";
            }
            else
            {
                ui->comboBoxVideoCodec->addItem( name, encoder );
                gst_object_unref( source );
            }

            qDebug().noquote() << message;
            gst_object_unref( factory );
        }
    }

    if ( ui->comboBoxVideoCodec->count() == 0 )
    {
        ui->pushButtonStart->setEnabled( false );
    }
    else
    {
        ui->pushButtonStart->setEnabled( true );
    }
}

/*
void QvkMainWindow::slot_set_available_AudioCodecs_in_Combox( QString suffix )
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
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << encoder;
            ui->comboBoxAudioCodec->addItem( name, encoder );
            gst_object_unref( factory );
        }
    }
    qDebug();
}
*/
