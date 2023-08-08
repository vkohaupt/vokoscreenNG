/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
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

#include "QvkBz2Decode.h"
#include "global.h"

#include <QDebug>
#include <QFile> //--------------------------------------------------------------------------------
#include <QFileInfo> //--------------------------------------------------------------------

QvkBz2Decode::QvkBz2Decode()
{
}

QvkBz2Decode::~QvkBz2Decode()
{}

QString bz2Path; //-------------------------------------------------------------------------
gboolean QvkBz2Decode::func( GstBus *bus, GstMessage *msg, gpointer data )
{
    Q_UNUSED(bus)

    GMainLoop *loop = (GMainLoop *) data;

    switch ( GST_MESSAGE_TYPE( msg ) )
    {
    case GST_MESSAGE_EOS:
        qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::func]" << "End of stream";
        g_main_loop_quit (loop);
        break;

    case GST_MESSAGE_ERROR:
    {
        gchar  *debug;
        GError *error;

        gst_message_parse_error( msg, &error, &debug );
        g_free( debug );

        g_printerr( "Error: %s\n", error->message );
        qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::func]" << error->message;
        g_error_free( error );

        // Wenn die .bz2 nicht ausgepackt werden kann, wird eine leere Datei openh264.txt angelegt.
        // Die openh264.txt wird im selben Ordner wie die .bz2 erstellt
        // Beim start von vokoscreen wird in der main.cpp übrprüft ob die Datei openh264.txt vorhanden ist.
        // Wenn die openh264.txt vorhanden ist wird die Datei openh264-6.dll gelöscht.
        // Anschließend wird die openh264.txt Datei gelöscht.

        // Datei anlegen
        QFile file( bz2Path + "/" + "openh264.txt" );
        file.open( QIODevice::ReadWrite );
        file.close();

        g_main_loop_quit( loop );
        break;
    }
    default:
        break;
    }

    return true;
}


void QvkBz2Decode::start_encoding( QString inputFile, QString outpuFile )
{
    // Pfad ermitteln für die anzulegende Datei openh264.txt
    QFileInfo fileInfo( inputFile );
    bz2Path = fileInfo.path();

    QByteArray byteArrayInputFile = inputFile.toUtf8();
    const gchar *inFile = byteArrayInputFile.constData();

    QByteArray byteArrayOutputFile = outpuFile.toUtf8();
    const gchar *outFile = byteArrayOutputFile.constData();

    GMainLoop *loop;
    GstElement *pipeline, *source, *decoder, *sink;
    GstBus *bus;
    guint bus_watch_id;

    loop = g_main_loop_new (NULL, FALSE);

    // Create gstreamer elements
    pipeline = gst_pipeline_new ("audio-player");
    source   = gst_element_factory_make( "filesrc",  "file-source" );
    decoder  = gst_element_factory_make( "bz2dec",   "bz2-decoder" );
    sink     = gst_element_factory_make( "filesink", "file-out" );

    if ( !pipeline || !source || !decoder || !sink )
    {
        qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::start_encoding]" << "One element could not be created. Exiting.";
        return;
    }

    // we set the input filename to the source element
    g_object_set( G_OBJECT( source ), "location", inFile, NULL );

    // we set the output filename to the sink element
    g_object_set( G_OBJECT( sink ), "location", outFile, NULL );

    // we add a message handler
    bus = gst_pipeline_get_bus( GST_PIPELINE( pipeline ) );
    bus_watch_id = gst_bus_add_watch( bus, QvkBz2Decode::func, loop );
    gst_object_unref( bus );

    // we add all elements into the pipeline
    gst_bin_add_many( GST_BIN (pipeline), source, decoder, sink, NULL );

    // we link the elements together
    gst_element_link_many( source, decoder, sink, NULL );

    // Set the pipeline to "playing" state
    qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::start_encoding]" << "Now decode file:" << inFile;;
    gst_element_set_state( pipeline, GST_STATE_PLAYING );

    // Start loop
    qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::start_encoding]" << "Running loop";
    g_main_loop_run( loop );

    // Out of the main loop, clean up nicely
    qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::start_encoding]" << "Returned from loop, Gstreamer go in state GST_STATE_NULL";
    gst_element_set_state( pipeline, GST_STATE_NULL );

    qDebug().noquote() << global::nameOutput << "[QvkBz2Decode::start_encoding]" << "Deleting pipeline";
    gst_object_unref( GST_OBJECT ( pipeline ) );
    g_source_remove( bus_watch_id );
    g_main_loop_unref( loop );

    emit signal_file_is_unzipped();
}

