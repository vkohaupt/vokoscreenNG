/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "global.h"
#include "QvkConvert_mkv_repair_wl.h"
#include "QvkFileDialog.h"

#include <QDebug>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTimer>

QString convert_video_repair;
QString convert_audio_repair;

QvkConvert_mkv_repair_wl::QvkConvert_mkv_repair_wl( QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *vk_ui )
{
    Q_UNUSED(vkMainWindow)
    ui = vk_ui;
    global::lineEditConvertRepair = new QLineEdit;
    connect(global::lineEditConvertRepair, &QLineEdit::textChanged, this, [=](){slot_lineEdit_Convert_eos_repair();});
    connect(ui->toolButton_convert_dialog_mkv_repair, &QToolButton::clicked, this, [=](){slot_convert_openfiledialog_mkv_repair();});
    connect(ui->pushButton_convert_mkv_repair, &QPushButton::clicked, this, [=](){slot_convert_mkv_repair();});

    // Hintergrundfarbe für Widget setzen
    QPalette palette_1 = ui->pushButton_convert_mkv_repair->palette();
    palette_1.setColor( QPalette::Window, QColor( QColor( 239, 240, 241 ) ) );
    ui->widget_convert_mkv_repair->setAutoFillBackground( true );
    ui->widget_convert_mkv_repair->setPalette( palette_1 );

    // Hintergrundfarbe für label setzen
    QPalette palette_2 = ui->label_convert_mkv_repair->palette();
    palette_2.setColor( QPalette::Window, QColor( QColor( 239, 240, 241 ) ) );
    ui->label_convert_mkv_repair->setAutoFillBackground( true );
    ui->label_convert_mkv_repair->setPalette( palette_2 );

    // Hintergrundfarbe für Widget und Label in Variable für späteren gebrauch
    paletteConvertWidget = ui->pushButton_convert_mkv_repair->palette();
    paletteConvertLabel = ui->label_convert_mkv_repair->palette();

    connect(ui->toolButton_convert_dialog_mkv_repair, &QToolButton::clicked, this, [=](){slot_dicover_set_filePath();});

    timer = new QTimer;
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 200 );
    connect(timer, &QTimer::timeout, this, [=](){slot_timer();});
}


QvkConvert_mkv_repair_wl::~QvkConvert_mkv_repair_wl()
{
}


void QvkConvert_mkv_repair_wl::slot_timer()
{
    gint64 duration;
    gst_element_query_duration( pipeline, GST_FORMAT_TIME, &duration );

    gint64 currentTime;
    gst_element_query_position( pipeline, GST_FORMAT_TIME, &currentTime );

    qint64 prozent = 100 / ( duration / 1000 / 1000 / 1000 ) * ( currentTime / 1000 / 1000 / 1000 );

    ui->label_convert_mkv_repair->setText( QString::number( prozent ) + " %" );
}


void QvkConvert_mkv_repair_wl::slot_lineEdit_Convert_eos_repair()
{
    QPalette palette_1 = ui->pushButton_convert_mkv_repair->palette();
    palette_1.setColor( QPalette::Window, QColor( Qt::green ) );
    ui->widget_convert_mkv_repair->setAutoFillBackground( true );
    ui->widget_convert_mkv_repair->setPalette( palette_1 );

    QPalette palette_2 = ui->label_convert_mkv_repair->palette();
    palette_2.setColor( QPalette::Window, QColor( Qt::green ) );
    ui->label_convert_mkv_repair->setAutoFillBackground( true );
    ui->label_convert_mkv_repair->setPalette( palette_2 );
    ui->label_convert_mkv_repair->setText( "File was successfully repaired" );

    ui->toolButton_convert_dialog_mkv_repair->setDisabled( false );
    ui->pushButton_convert_mkv_repair->setDisabled( false );

    timer->stop();
    ui->label_convert_mkv_repair->setText( "100 %" );
}


void QvkConvert_mkv_repair_wl::slot_convert_openfiledialog_mkv_repair()
{
    //    QApplication::setDesktopSettingsAware( false );

    QString pathFile;
    QvkFileDialog vkFileDialog( this );
    QStringList list( { "video/x-matroska" } );
    vkFileDialog.setMimeTypeFilters( list );
    vkFileDialog.setModal( true );
    vkFileDialog.setVideoPath( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    if ( vkFileDialog.exec() == QDialog::Accepted ) {
        if ( !vkFileDialog.selectedFiles().empty() ) {
            pathFile = vkFileDialog.selectedFiles().at(0);
            ui->lineEdit_convert_mkv_repair->setText( pathFile );
            ui->pushButton_convert_mkv_repair->setEnabled( true );
            ui->label_convert_mkv_repair->setText( "Please start repair" );

            ui->widget_convert_mkv_repair->setAutoFillBackground( true );
            ui->widget_convert_mkv_repair->setPalette( paletteConvertWidget );

            ui->label_convert_mkv_repair->setAutoFillBackground( true );
            ui->label_convert_mkv_repair->setPalette( paletteConvertLabel );
        }
    }

    //    QApplication::setDesktopSettingsAware( true );
}

int counterConvertRepair = 0;
GstBusSyncReply QvkConvert_mkv_repair_wl::call_bus_message_convert_repair( GstBus *bus, GstMessage *message, gpointer user_data )
{
    Q_UNUSED(bus);
    Q_UNUSED(user_data)
    switch(GST_MESSAGE_TYPE (message))
    {
    case GST_MESSAGE_ERROR:
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_ERROR";
        break;
    case GST_MESSAGE_EOS: {
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_EOS";
        counterConvertRepair++;
        global::lineEditConvertRepair->setText( QString::number( counterConvertRepair ) );
        break;
    }
    case GST_MESSAGE_DURATION_CHANGED:
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_DURATION_CHANGED";
        break;
    case GST_MESSAGE_STEP_DONE:
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_STEP_DONE";
        break;
    case GST_MESSAGE_TAG:
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_TAG";
        break;
    case GST_MESSAGE_STATE_CHANGED:
        // qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_STATE_CHANGED";
        break;
    case GST_MESSAGE_STREAM_START:
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_STREAM_START";
        break;
    case GST_MESSAGE_APPLICATION:
    {
        qDebug().noquote() << global::nameOutput << "[Convert][Repair] GST_MESSAGE_APPLICATION";
        break;
    }
    default:
        break;
    }

    return GST_BUS_PASS;
}


void QvkConvert_mkv_repair_wl::slot_convert_mkv_repair()
{
    QString video_codec;
    if ( convert_video_repair.contains( "H264" ) ) {
        video_codec = "H264";
    }

    QString audio_codec;
    if ( convert_audio_repair.contains( "Vorbis" ) ) {
        audio_codec = "Vorbis";
    }
    if ( convert_audio_repair.contains( "MPEG" ) ) {
        audio_codec = "MPEG";
    }
    if ( convert_audio_repair.contains( "FLAC" ) ) {
        audio_codec = "FLAC";
    }
    if ( convert_audio_repair.contains( "Opus" ) ) {
        audio_codec = "Opus";
    }

    qDebug().noquote() << global::nameOutput << "[Convert][Repair] Detected video codec:" << video_codec;
    qDebug().noquote() << global::nameOutput << "[Convert][Repair] Detected audio codec:" << audio_codec;

    if ( video_codec == "H264" ) {
        ui->toolButton_convert_dialog_mkv_repair->setDisabled( true );
        ui->pushButton_convert_mkv_repair->setDisabled( true );

        // Hintergrundfarbe für Widget setzen
        QPalette palette_1 = ui->pushButton_convert_mkv_repair->palette();
        palette_1.setColor( QPalette::Window, QColor( QColor( 239, 240, 241 ) ) );
        ui->pushButton_convert_mkv_repair->setAutoFillBackground( true );
        ui->pushButton_convert_mkv_repair->setPalette( palette_1 );
        // Hintergrundfarbe für label setzen
        QPalette palette_2 = ui->label_convert_mkv_repair->palette();
        palette_2.setColor( QPalette::Window, QColor( QColor( 239, 240, 241 ) ) );
        ui->label_convert_mkv_repair->setAutoFillBackground( true );
        ui->label_convert_mkv_repair->setPalette( palette_2 );
        ui->label_convert_mkv_repair->setText( "Please wait" );

        GstElement *pipeline = nullptr;

        QString filePath = ui->lineEdit_convert_mkv_repair->text();
        QFileInfo fileInfo( filePath );
        QString path = fileInfo.path();


        // gst-launch-1.0 filesrc location=/home/vk/Videos/vokoscreenNG-kaputt-with-audio.mkv
        //     ! matroskademux
        //     ! h264parse
        //     ! queue
        //     ! matroskamux
        //     ! filesink location=test2.mkv
        QString VK_Pipeline;
        QString fileNameRepair = fileInfo.baseName() + "_REPAIR.mkv";
        VK_Pipeline = "filesrc location=" + filePath +
                " ! matroskademux" +
                " ! h264parse" +
                " ! queue" +
                " ! matroskamux" +
                " ! filesink location=" + path + "/" + fileNameRepair;

        qDebug().noquote() << global::nameOutput << VK_Pipeline;

        QByteArray byteArray = VK_Pipeline.toUtf8();
        const gchar *line = byteArray.constData();
        GError *error = Q_NULLPTR;
        pipeline = gst_parse_launch( line, &error );

        GstBus *bus = gst_pipeline_get_bus( GST_PIPELINE ( pipeline ) );
        gst_bus_set_sync_handler( bus, (GstBusSyncHandler)call_bus_message_convert_repair, this, NULL );
        gst_object_unref( bus );

        // Start playing
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "[Convert] repair was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "[Convert] repair was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "[Convert] repair was clicked" << "GST_STATE_CHANGE_ASYNC"   << "Returncode =" << ret;   } // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "[Convert] repair was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3
        if ( ret == GST_STATE_CHANGE_FAILURE )   {
            qDebug().noquote() << global::nameOutput << "[Convert][Repair] Unable to set the pipeline to the playing state.";
            gst_object_unref( pipeline );
            return;
        }
    } else {
        msgbox_mkv_repair();
    }
}


void QvkConvert_mkv_repair_wl::msgbox_mkv_repair() {
    QString text = "Only videos with H.264 video codec and audio MP3 or Opus can repair.";
    qDebug().noquote() << global::nameOutput << "[Convert][Repair]" << "Repair failed";
    qDebug().noquote() << global::nameOutput << "[Convert][Repair]" << text;

    QMessageBox msgBox( ui->centralwidget );
    msgBox.setModal( true );
    msgBox.setIcon( QMessageBox::Warning );
    QString space = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    msgBox.setText( space + "<b>Repair failed</b>" + space );
    msgBox.setInformativeText( text );
    msgBox.exec();
}

//----------------------------------------- Begin discover ----------------------------------------------------------------------------
// https://github.com/GStreamer/gst-docs/blob/master/examples/tutorials/basic-tutorial-9.c
#include <string.h>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>

// Structure to contain all our information, so we can pass it around
typedef struct _CustomData
{
    GstDiscoverer *discoverer;
    GMainLoop *loop;
} CustomData;

// Print a tag in a human-readable format (name: value)
static void print_tag_foreach( const GstTagList *tags, const gchar *tag, gpointer user_data )
{
    GValue val = { 0, 0 };
    gchar *str;
    gint depth = GPOINTER_TO_INT( user_data );

    gst_tag_list_copy_value( &val, tags, tag );

    if ( G_VALUE_HOLDS_STRING( &val ) ) {
        str = g_value_dup_string( &val );
    } else {
        str = gst_value_serialize( &val );
    }

    //  g_print( "%*s%s: %s\n", 2 * depth, " ", gst_tag_get_nick (tag), str ); // Übersetzung
    g_print( "[vokoscreenNG] %*s%s: %s\n", 2 * depth, " ", tag, str );  // English

    QString m_tag = tag;
    QString m_str = str;
    if ( m_tag == "video-codec" ) {
        convert_video_repair = m_str;
    }
    if ( m_tag == "audio-codec" ) {
        convert_audio_repair = m_str;
    }

    g_free( str );

    g_value_unset( &val );
}

// Print information regarding a stream
static void print_stream_info (GstDiscovererStreamInfo * info, gint depth)
{
    gchar *desc = NULL;
    GstCaps *caps;
    const GstTagList *tags;

    caps = gst_discoverer_stream_info_get_caps( info );

    if ( caps ) {
        if ( gst_caps_is_fixed( caps ) ) {
            desc = gst_pb_utils_get_codec_description( caps );
        } else {
            desc = gst_caps_to_string (caps);
        }
        gst_caps_unref( caps );
    }

    g_print( "[vokoscreenNG] %*s%s: %s\n", 2 * depth, " ",
             gst_discoverer_stream_info_get_stream_type_nick( info ),
             ( desc ? desc : "" ) );

    if ( desc ) {
        g_free( desc );
        desc = NULL;
    }

    tags = gst_discoverer_stream_info_get_tags( info );
    if ( tags ) {
        g_print( "[vokoscreenNG] %*sTags:\n", 2 * ( depth + 1 ), " " );
        gst_tag_list_foreach( tags, print_tag_foreach, GINT_TO_POINTER( depth + 2 ) );
    }
}

// Print information regarding a stream and its substreams, if any
static void print_topology (GstDiscovererStreamInfo * info, gint depth)
{
    GstDiscovererStreamInfo *next;

    if (!info)
        return;

    print_stream_info (info, depth);

    next = gst_discoverer_stream_info_get_next (info);
    if (next) {
        print_topology (next, depth + 1);
        gst_discoverer_stream_info_unref (next);
    } else if (GST_IS_DISCOVERER_CONTAINER_INFO (info)) {
        GList *tmp, *streams;

        streams =
                gst_discoverer_container_info_get_streams (GST_DISCOVERER_CONTAINER_INFO
                                                           (info));
        for (tmp = streams; tmp; tmp = tmp->next) {
            GstDiscovererStreamInfo *tmpinf = (GstDiscovererStreamInfo *) tmp->data;
            print_topology (tmpinf, depth + 1);
        }
        gst_discoverer_stream_info_list_free (streams);
    }
}

// This function is called every time the discoverer has information regarding
// one of the URIs we provided.
static void on_discovered_cb (GstDiscoverer *discoverer, GstDiscovererInfo *info, GError *err, CustomData *data)
{
    Q_UNUSED(discoverer)
    Q_UNUSED(data)
    GstDiscovererResult result;
    const gchar *uri;
    const GstTagList *tags;
    GstDiscovererStreamInfo *sinfo;

    uri = gst_discoverer_info_get_uri (info);
    result = gst_discoverer_info_get_result (info);
    switch (result) {
    case GST_DISCOVERER_URI_INVALID:
        g_print ("[vokoscreenNG][Repair] Invalid URI '%s'\n", uri);
        break;
    case GST_DISCOVERER_ERROR:
        g_print ("[vokoscreenNG][Repair] Discoverer error: %s\n", err->message);
        break;
    case GST_DISCOVERER_TIMEOUT:
        g_print ("[vokoscreenNG][Repair] Timeout\n");
        break;
    case GST_DISCOVERER_BUSY:
        g_print ("[vokoscreenNG][Repair] Busy\n");
        break;
    case GST_DISCOVERER_MISSING_PLUGINS:{
        const GstStructure *s;
        gchar *str;

        s = gst_discoverer_info_get_misc (info);
        str = gst_structure_to_string (s);

        g_print ("[vokoscreenNG][Repair] Missing plugins: %s\n", str);
        g_free (str);
        break;
    }
    case GST_DISCOVERER_OK:
        g_print ("[vokoscreenNG][Repair] Discovered '%s'\n", uri);
        break;
    }

    if (result != GST_DISCOVERER_OK) {
        g_printerr ("[vokoscreenNG][Repair] This URI cannot be played\n");
        return;
    }

    // If we got no error, show the retrieved information

    g_print( "\n[vokoscreenNG] Duration: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS( gst_discoverer_info_get_duration( info ) ) );

    tags = gst_discoverer_info_get_tags (info);
    if (tags) {
        g_print ("[vokoscreenNG] Tags:\n");
        gst_tag_list_foreach (tags, print_tag_foreach, GINT_TO_POINTER (1));
    }

    g_print ("[vokoscreenNG] Seekable: %s\n", (gst_discoverer_info_get_seekable (info) ? "yes" : "no"));

    g_print ("\n");

    sinfo = gst_discoverer_info_get_stream_info (info);
    if (!sinfo)
        return;

    g_print ("[vokoscreenNG] Stream information:\n");

    print_topology (sinfo, 1);

    gst_discoverer_stream_info_unref (sinfo);

    g_print ("\n");
}

// This function is called when the discoverer has finished examining all the URIs we provided.
static void on_finished_cb (GstDiscoverer * discoverer, CustomData * data)
{
    Q_UNUSED(discoverer)
    g_print( "[vokoscreenNG][Repair] Finished discovering\n\n" );
    g_main_loop_quit( data->loop );
}


void QvkConvert_mkv_repair_wl::slot_dicover_set_filePath()
{
    slot_dicover_start( "file://" + ui->lineEdit_convert_mkv_repair->text() );
}


void QvkConvert_mkv_repair_wl::slot_dicover_start( QString filePath )
{
    CustomData data;
    GError *err = NULL;

    QString file = filePath;
    QByteArray byteArray = file.toUtf8();
    gchar *uri = byteArray.data();

    // Initialize cumstom data structure
    memset (&data, 0, sizeof (data));

    // Instantiate the Discoverer
    data.discoverer = gst_discoverer_new (5 * GST_SECOND, &err);
    if (!data.discoverer) {
        g_print ("[vokoscreenNG][Repair] Error creating discoverer instance: %s\n", err->message);
        g_clear_error (&err);
        //    return -1;
    }

    // Connect to the interesting signals
    g_signal_connect (data.discoverer, "discovered", G_CALLBACK (on_discovered_cb), &data);
    g_signal_connect (data.discoverer, "finished", G_CALLBACK (on_finished_cb), &data);

    // Start the discoverer process (nothing to do yet)
    gst_discoverer_start (data.discoverer);

    // Add a request to process asynchronously the URI passed through the command line
    if (!gst_discoverer_discover_uri_async (data.discoverer, uri)) {
        g_print ("[vokoscreenNG][Repair] Failed to start discovering URI '%s'\n", uri);
        g_object_unref (data.discoverer);
        //    return -1;
    }

    // Create a GLib Main Loop and set it to run, so we can wait for the signals
    data.loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (data.loop);

    // Stop the discoverer process
    gst_discoverer_stop (data.discoverer);

    // Free resources
    g_object_unref (data.discoverer);
    g_main_loop_unref (data.loop);
}
