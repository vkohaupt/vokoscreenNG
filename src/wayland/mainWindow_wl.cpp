#include "mainWindow_wl.h"
#include "global.h"
#include "QvkScreenManager.h"

#include <QStringList>
#include <QStandardPaths>
#include <QDateTime>
#include <QThread>
#include <QMimeDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QStyleFactory>
#include <QLibraryInfo>

QvkMainWindow_wl::QvkMainWindow_wl( QWidget *parent, Qt::WindowFlags f )
    : QMainWindow(parent, f)
    , ui(new Ui::formMainWindow_wl)
{
    ui->setupUi( this );

    vklogController = new QvkLogController();
    connect( vklogController, SIGNAL( signal_newLogText( QString ) ), ui->textBrowserLog, SLOT( append( QString ) ) );

    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );

    ui->tabWidgetScreencast->setCurrentIndex( 0 );
    ui->tabWidgetSideBar->setCurrentIndex( 0 );
    set_CornerWidget();
    set_system_info();
    set_SpezialSliders();
    set_Connects();
    check_all_Elements_available();
    vkContainerController = new QvkContainerController( this, ui );

    vkRegionChoise = new QvkRegionChoise_wl( ui );
    connect( ui->radioButtonScreencastArea,     SIGNAL( toggled( bool ) ), vkRegionChoise, SLOT( slot_show( bool ) ) );
    connect( ui->toolButtonScreencastAreaReset, SIGNAL( clicked( bool ) ), vkRegionChoise, SLOT( slot_areaReset() ) );

    QvkScreenManager *screenManager = new QvkScreenManager();
    // Area
    connect( screenManager, SIGNAL( signal_clear_widget() ),                          ui->comboBoxScreencastScreenArea, SLOT( clear() ) );
    connect( screenManager, SIGNAL( signal_screen_count_changed( QString, QString) ), this,                             SLOT( slot_screenCountChangedArea( QString, QString ) ) );
    screenManager->init();

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


void QvkMainWindow_wl::set_system_info()
{
    qDebug().noquote() << global::nameOutput << "Version:" << global::version;
    qDebug().noquote() << global::nameOutput << "Locale:" << QLocale::system().name();
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug().noquote() << global::nameOutput << "Log from:" << dateTime.toString( "yyyy-MM-dd hh:mm:ss" );
    QLocale locale;
    qDebug().noquote() << global::nameOutput << "Country:" << QLocale::countryToString( locale.country() );
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
    qDebug().noquote() << global::nameOutput << "Qt-PluginsPath:     " << QLibraryInfo::location( QLibraryInfo::PluginsPath );
    qDebug().noquote() << global::nameOutput << "Qt-TranslationsPath:" << QLibraryInfo::location( QLibraryInfo::TranslationsPath );
    qDebug().noquote() << global::nameOutput << "Qt-LibraryPath:     " << QLibraryInfo::location( QLibraryInfo::LibrariesPath );
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

    QPixmap pixmap( ":/pictures/wayland.png" );
    pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

    for ( int i = 0; i < list.count(); i++ )
    {
        QLabel *label = new QLabel();
        label->setPixmap( pixmap );
        label->setEnabled( false );
        list.at( i )->setCornerWidget( label, Qt::TopRightCorner );
    }
}


void QvkMainWindow_wl::slot_screenCountChangedArea( QString stringText, QString stringData )
{
    ui->comboBoxScreencastScreenArea->addItem( stringText, stringData );
}


void QvkMainWindow_wl::set_Connects()
{
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                 SLOT( slot_start() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,  SLOT( setEnabled( bool ) ) );

    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), this,                 SLOT( slot_stop() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStop,   SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->pushButtonStart,  SLOT( setDisabled( bool ) ) );

    connect( portal_wl, SIGNAL( signal_portal_fd_path( QString, QString ) ), this, SLOT( slot_start_gst( QString, QString ) ) );
    connect( portal_wl, SIGNAL( signal_portal_cancel( uint ) ), this,              SLOT( slot_portal_cancel( uint ) ) );

    connect( ui->toolButtonFramesReset, SIGNAL( clicked( bool ) ), this,           SLOT( slot_frames_Reset() ) );
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
    // https://flatpak.github.io/xdg-desktop-portal/portal-docs.html#gdbus-property-org-freedesktop-portal-ScreenCast.AvailableSourceTypes
    // Value 1 = MONITOR
    // Value 2 = WINDOW
    int sourceType = 1;
    if ( ui->radioButtonScreencastFullscreen->isChecked() )
    {
        qDebug().noquote() << global::nameOutput << "Start portal fullscreen";
        sourceType = 1;
    }

    if ( ui->radioButtonScreencastWindow->isChecked() )
    {
        qDebug().noquote() << global::nameOutput << "Start portal window";
        sourceType = 2;
    }

    if ( ui->radioButtonScreencastArea->isChecked() )
    {
        qDebug().noquote() << global::nameOutput << "Start portal area";
        sourceType = 1;

        if ( ui->checkBox_panel_top->isChecked() ) {
            qDebug().noquote() << global::nameOutput << "Panel is top";
        }

        if ( ui->checkBox_panel_right->isChecked() ) {
            qDebug().noquote() << global::nameOutput << "Panel is right";
        }

        if ( ui->checkBox_panel_bottom->isChecked() ) {
            qDebug().noquote() << global::nameOutput << "Panel is bottom";
        }

        if ( ui->checkBox_panel_left->isChecked() ) {
            qDebug().noquote() << global::nameOutput << "Panel is left";
        }
    }

    // https://flatpak.github.io/xdg-desktop-portal/portal-docs.html#gdbus-property-org-freedesktop-portal-ScreenCast.AvailableCursorModes
    // Value 1 = hidden cursor
    // Value 2 = record cursor
    int mousecursorONOff = 1;
    if ( ui->checkBoxMouseCursorOnOff->isChecked() == true )
    {
        qDebug().noquote() << global::nameOutput << "Mouse cursor is not recording";
        mousecursorONOff = 1;
    }

    if ( ui->checkBoxMouseCursorOnOff->isChecked() == false )
    {
        qDebug().noquote() << global::nameOutput << "Mouse cursor is recording";
        mousecursorONOff = 2;
    }

    portal_wl->requestScreenSharing( sourceType, mousecursorONOff );
}


QString QvkMainWindow_wl::get_Area_Videocrop()
{
    QString videocrop = "";
    vkRegionChoise->recordMode( true );

    QScreen *Screen = screen();
    int screenWidth = Screen->size().width();
    int screenHeight = Screen->size().height();

    if ( ui->checkBox_panel_top->isChecked() == true )
    {
        QString top = QString::number( vkRegionChoise->getYRecordArea() + vkRegionChoise->get_panel_height() );
        QString right = QString::number( screenWidth - ( vkRegionChoise->getWidthRecordArea() + vkRegionChoise->getXRecordArea() ) );
        QString bottom = QString::number( screenHeight - ( vkRegionChoise->getHeightRecordArea() + vkRegionChoise->getYRecordArea() ) - vkRegionChoise->get_panel_height() );
        QString left = QString::number( vkRegionChoise->getXRecordArea() );
        videocrop = "videocrop top=" + top + " " + "right=" + right + " " + "bottom=" + bottom + " " + "left=" + left;
    }

    if ( ui->checkBox_panel_bottom->isChecked() == true )
    {
        QString top = QString::number( vkRegionChoise->getYRecordArea() );
        QString right = QString::number( screenWidth - ( vkRegionChoise->getWidthRecordArea() + vkRegionChoise->getXRecordArea() ) );
        QString bottom = QString::number( screenHeight - ( vkRegionChoise->getHeightRecordArea() + vkRegionChoise->getYRecordArea() ) );
        QString left = QString::number( vkRegionChoise->getXRecordArea() );
        videocrop = "videocrop top=" + top + " " + "right=" + right + " " + "bottom=" + bottom + " " + "left=" + left;
    }

    qDebug().noquote() << global::nameOutput << "Area crop from the screen"
                                             << Screen->name() + ","
                                             << Screen->manufacturer() + ","
                                             << Screen->model() + ","
                                             << QString::number( Screen->size().width() ) + "/" + QString::number( Screen->size().height() );


    return videocrop;
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


void QvkMainWindow_wl::slot_portal_cancel( uint value )
{
    Q_UNUSED(value)
    ui->pushButtonStart->setEnabled( true );
    ui->pushButtonStop->setEnabled( false );
}


void QvkMainWindow_wl::set_SpezialSliders()
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
    messageBox->setText( ( "Wayland desktop session detected" ) );
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
