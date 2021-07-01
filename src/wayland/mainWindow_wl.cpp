#include "mainWindow_wl.h"
#include "global.h"
#include "QvkLogController.h"
#include "QvkScreenManager.h"

#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QThread>
#include <QMimeDatabase>
#include <QMessageBox>

QvkMainWindow_wl::QvkMainWindow_wl( QWidget *parent, Qt::WindowFlags f )
    : QMainWindow(parent, f)
    , ui(new Ui::formMainWindow_wl)
{
    ui->setupUi( this );

    QvkLogController *vklogController = new QvkLogController();
    connect( vklogController, SIGNAL( signal_newLogText( QString ) ), ui->textBrowserLog, SLOT( append( QString ) ) );

    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );

    ui->tabWidgetScreencast->setCurrentIndex( 0 );
    ui->tabWidgetSideBar->setCurrentIndex( 0 );

    set_SpezialSlider();
    set_Connects();
    set_supported_Formats_And_Codecs(); // This is the base for format, video and audiocodec
    check_all_Elements_available();
    check_all_Formats_available();
    set_available_Formats_in_ComboBox();

    vkRegionChoise = new QvkRegionChoise_wl( ui );
//    connect( ui->radioButtonScreencastArea,     SIGNAL( toggled( bool ) ), vkRegionChoise, SLOT( setVisible( bool ) ) );
    connect( ui->radioButtonScreencastArea,     SIGNAL( toggled( bool ) ), vkRegionChoise, SLOT( slot_show( bool ) ) );
    connect( ui->toolButtonScreencastAreaReset, SIGNAL( clicked( bool ) ), vkRegionChoise, SLOT( slot_areaReset() ) );

    QvkScreenManager *screenManager = new QvkScreenManager();
    // Area
    connect( screenManager, SIGNAL( signal_clear_widget() ),                          ui->comboBoxScreencastScreenArea, SLOT( clear() ) );
    connect( screenManager, SIGNAL( signal_screen_count_changed( QString, QString) ), this,                             SLOT( slot_screenCountChangedArea( QString, QString ) ) );
//    connect( ui->comboBoxScreencastScreenArea, SIGNAL( currentIndexChanged( int) ),   vkRegionChoise,                   SLOT( slot_init() ) );
    screenManager->init();

    ui->tabWidgetScreencast->removeTab(1);
    ui->frame_information->hide();
    ui->pushButtonPause->hide();
    ui->pushButtonContinue->hide();
    ui->pushButtonPlay->hide();
    ui->pushButtonScreencastOpenfolder->hide();
}


QvkMainWindow_wl::~QvkMainWindow_wl()
{
}


void QvkMainWindow_wl::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    ui->pushButtonStop->click();
    vkRegionChoise->close();
}


void QvkMainWindow_wl::slot_screenCountChangedArea( QString stringText, QString stringData )
{
    ui->comboBoxScreencastScreenArea->addItem( stringText, stringData );
}


void QvkMainWindow_wl::set_Connects()
{
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,  SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                 SLOT( slot_start() ) );

    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), this,                 SLOT( slot_stop() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStop,   SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStart,  SLOT( setDisabled( bool ) ) );

    connect( portal_wl, SIGNAL( signal_portal_fd_path( QString, QString ) ), this, SLOT( slot_start_gst( QString, QString ) ) );

    connect( ui->toolButtonFramesReset, SIGNAL( clicked( bool ) ), this,           SLOT( slot_frames_Reset() ) );

    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this,    SLOT( slot_set_available_VideoCodecs_in_ComboBox( QString ) ) );
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


void QvkMainWindow_wl::slot_start()
{
    qDebug().noquote() << global::nameOutput << "Start record";

    int value = 1;
    if ( ui->radioButtonScreencastFullscreen->isChecked() )
    {
        value = 1;
    }

    if ( ui->radioButtonScreencastWindow->isChecked() )
    {
        value = 2;
    }

    if ( ui->radioButtonScreencastArea->isChecked() )
    {
        value = 1;
    }

    portal_wl->requestScreenSharing( value );
}


QString QvkMainWindow_wl::get_Area_Videocrop()
{
    QString value = "";
    vkRegionChoise->recordMode( true );

    int desktop_menue_height = 0;

    if ( ui->checkBox_menue_top->isChecked() == true  )
    {
        desktop_menue_height = vkRegionChoise->get_desktop_menue_height();
    }

    int width = ui->comboBoxScreencastScreenArea->currentData().toString().section( " ", 2, 2).section( "=", 1, 1 ).toInt();
    int height = ui->comboBoxScreencastScreenArea->currentData().toString().section( " ", 3, 3).section( "=", 1, 1 ).toInt();

    QString top = QString::number( vkRegionChoise->getYRecordArea() + desktop_menue_height );
    QString right = QString::number( width - ( vkRegionChoise->getWidth() + vkRegionChoise->getXRecordArea() ) );
    QString bottom = QString::number( height - ( vkRegionChoise->getHeight() + vkRegionChoise->getYRecordArea() ) - desktop_menue_height );
    QString left = QString::number( vkRegionChoise->getXRecordArea() );
    value = "videocrop top=" + top + " " + "right=" + right + " " + "bottom=" + bottom + " " + "left=" + left;
    return value;
}


void QvkMainWindow_wl::slot_start_gst( QString vk_fd, QString vk_path )
{
    ui->pushButtonStop->setEnabled( true );

    QStringList stringList;
    stringList << QString( "pipewiresrc fd=" ).append( vk_fd ).append( " path=" ).append( vk_path ).append( " do-timestamp=true" );
    stringList << "videoconvert";
    stringList << "videorate";
    if ( ui->radioButtonScreencastArea->isChecked() ) { stringList << get_Area_Videocrop(); }
    stringList << "video/x-raw, framerate=" + QString::number( sliderFrames->value() ) + "/1";
    stringList << get_Videocodec_Encoder();
    stringList << "matroskamux name=mux";

    QString newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
    stringList << "filesink location=\"" + QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) + "/" + newVideoFilename + "\"";

    QString launch = stringList.join( " ! " );
    qDebug().noquote() << global::nameOutput << launch;

    pipeline = gst_parse_launch( launch.toUtf8(), nullptr );
    gst_element_set_state( pipeline, GST_STATE_PLAYING );
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
       vkRegionChoise->recordMode( false );
    }
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


void QvkMainWindow_wl::slot_frames_Reset()
{
    sliderFrames->setValue( 25 );
}


void QvkMainWindow_wl::messageBox( QString value )
{
    QMessageBox *messageBox = new QMessageBox();
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    messageBox->setWindowIcon( icon );
    messageBox->setWindowTitle( global::name + " " + global::version );
    messageBox->setIcon( QMessageBox::Critical );
    messageBox->setText( tr( "Wayland desktop session detected" ) );
    messageBox->setInformativeText( ( "Please install the package\n" + value ) );
    messageBox->exec();

    exit(0);
}


void QvkMainWindow_wl::check_all_Elements_available()
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

    for ( int i = 0; i < list.count(); i++ )
    {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
            if ( list.at(i) == "pipewiresrc" )
            {
               messageBox( "gstreamer-plugin-pipewire" );
            }
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
void QvkMainWindow_wl::set_supported_Formats_And_Codecs()
{
    QStringList MKV;
                MKV << "muxer:matroskamux:mkv";
                MKV << "videomimetype:video/x-matroska";
                MKV << "audiomimetype:audio/x-matroska";
                MKV << "videocodec:openh264enc:H.264";
                MKV << "videocodec:x264enc:x264";
                MKV << "videocodec:vp8enc:VP8";
                MKV << "audiocodec:vorbisenc:vorbis";
                MKV << "audiocodec:flacenc:flac";
                MKV << "audiocodec:opusenc:opus";
                MKV << "audiocodec:lamemp3enc:mp3";

    QStringList WEBM;
                WEBM << "muxer:webmmux:webm";
                WEBM << "videomimetype:video/webm";
                WEBM << "audiomimetype:audio/webm";
                WEBM << "videocodec:vp8enc:VP8";
                WEBM << "audiocodec:vorbisenc:vorbis";
                WEBM << "audiocodec:opusenc:opus";

    QStringList AVI;
                AVI << "muxer:avimux:avi";
                AVI << "videomimetype:video/x-msvideo";
                AVI << "audiomimetype:audio/x-msvideo";
                AVI << "videocodec:openh264enc:H.264";
                AVI << "videocodec:x264enc:x264";
                AVI << "videocodec:vp8enc:VP8";
                AVI << "audiocodec:lamemp3enc:mp3";

    QStringList MP4;
                MP4 << "muxer:mp4mux:mp4";
                MP4 << "videomimetype:video/mp4";
                MP4 << "audiomimetype:audio/mpeg";
                MP4 << "videocodec:openh264enc:H.264";
                MP4 << "videocodec:x264enc:x264";
                MP4 << "audiocodec:lamemp3enc:mp3";
                MP4 << "audiocodec:opusenc:opus";

    // https://de.wikipedia.org/wiki/QuickTime
    QStringList MOV;
                MOV << "muxer:qtmux:mov";
                MOV << "videomimetype:video/mp4";
                MOV << "audiomimetype:audio/mpeg";
                MOV << "videocodec:openh264enc:H.264";
                MOV << "videocodec:x264enc:x264";
                MOV << "videocodec:vp8enc:VP8";
                MOV << "audiocodec:lamemp3enc:mp3";

    videoFormatsList.clear();
    videoFormatsList.append( MKV.join( "," ) );
    videoFormatsList.append( WEBM.join( "," ) );
    videoFormatsList.append( AVI.join( "," ) );
    videoFormatsList.append( MP4.join( ",") );
    videoFormatsList.append( MOV.join( ",") );
}


void QvkMainWindow_wl::check_all_Formats_available()
{
    qDebug().noquote() << global::nameOutput << "--- GStreamer muxer ---";

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
    qDebug();
}


void QvkMainWindow_wl::set_available_Formats_in_ComboBox()
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


void QvkMainWindow_wl::slot_set_available_VideoCodecs_in_ComboBox( QString suffix )
{
    ui->comboBoxVideoCodec->clear();

    qDebug().noquote() << global::nameOutput << "--- GStreamer video encoder ---";

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
    qDebug();
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
