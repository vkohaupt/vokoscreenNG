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

#include "QvkPlayerGstDiscover.h"
#include "global.h"

#include <string.h>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>

#include <QDebug>

QvkPlayerGstDiscover::QvkPlayerGstDiscover()
{
}


// Structure to contain all our information, so we can pass it around
typedef struct _CustomData {
    GstDiscoverer *discoverer;
    GMainLoop *loop;
} CustomData;

bool isAudio;
bool isVideo;

// Print a tag in a human-readable format (name: value)
void print_tag_foreach( const GstTagList *tags, const gchar *tag, gpointer user_data )
{
    Q_UNUSED(user_data)

    GValue val = G_VALUE_INIT;

    gst_tag_list_copy_value( &val, tags, tag );

    if ( QString( tag ) == "audio-codec" ) {
        isAudio = true;
    }

    if ( QString( tag ) == "video-codec" ) {
        isVideo = true;
    }

    g_value_unset( &val );
}


// This function is called every time the discoverer has information regarding
// one of the URIs we provided.
void on_discovered_cb (GstDiscoverer *discoverer, GstDiscovererInfo *info, GError *err, CustomData *data)
{
    Q_UNUSED(discoverer)
    Q_UNUSED(data)

    GstDiscovererResult result;
    const gchar *uri;
    const GstTagList *tags;

    uri = gst_discoverer_info_get_uri (info);

    result = gst_discoverer_info_get_result (info);
    switch (result) {
        case GST_DISCOVERER_URI_INVALID:
            g_print ("Invalid URI '%s'\n", uri);
            break;
        case GST_DISCOVERER_ERROR:
            g_print ("Discoverer error: %s\n", err->message);
            break;
        case GST_DISCOVERER_TIMEOUT:
            g_print ("Timeout\n");
            break;
        case GST_DISCOVERER_BUSY:
            g_print ("Busy\n");
            break;
        case GST_DISCOVERER_MISSING_PLUGINS:{
                const GstStructure *s;
                gchar *str;

                s = gst_discoverer_info_get_misc (info);
                str = gst_structure_to_string (s);

                g_print ("Missing plugins: %s\n", str);
                g_free (str);
                break;
            }
        case GST_DISCOVERER_OK:{
                break;
            }
    }

    if (result != GST_DISCOVERER_OK) {
        g_printerr ("This URI cannot be played\n");
        return;
    }

    tags = gst_discoverer_info_get_tags (info);
    if ( tags ) {
        gst_tag_list_foreach( tags, print_tag_foreach, GINT_TO_POINTER(1) );
    }
}

// This function is called when the discoverer has finished examining
// all the URIs we provided.
void on_finished_cb( GstDiscoverer *discoverer, CustomData *data )
{
    Q_UNUSED(discoverer)
    g_main_loop_quit( data->loop );
}


void QvkPlayerGstDiscover::discover_file( QString mediaFile )
{
    CustomData data;
    GError *err = NULL;

    isAudio = false;
    isVideo = false;

#ifdef Q_OS_LINUX
    mediaFile.prepend( "file://" );
#endif
#ifdef Q_OS_WIN
    mediaFile.prepend( "file:///" );
#endif

    QByteArray byteArray = mediaFile.toUtf8();
    const gchar *uri = byteArray.constData();
    qDebug().noquote().nospace() << global::nameOutput << "[Player][Discover] Uri: " << QString( uri );

    // Initialize custom data structure
    memset (&data, 0, sizeof (data));

    // Instantiate the Discoverer
    data.discoverer = gst_discoverer_new (5 * GST_SECOND, &err);
    if (!data.discoverer)
    {
        g_print ("Error creating discoverer instance: %s\n", err->message);
        g_clear_error (&err);
        //    return -1;
    }

    // Connect to the interesting signals
    g_signal_connect( data.discoverer, "discovered", G_CALLBACK( on_discovered_cb ), &data );
    g_signal_connect( data.discoverer, "finished", G_CALLBACK( on_finished_cb ), &data );

    // Start the discoverer process (nothing to do yet)
    gst_discoverer_start (data.discoverer);

    // Add a request to process asynchronously the URI passed through the command line
    if (!gst_discoverer_discover_uri_async (data.discoverer, uri))
    {
        g_print ("Failed to start discovering URI '%s'\n", uri);
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

    qDebug().noquote().nospace() << global::nameOutput << "[Player][Discover] Video: " << isVideo;
    qDebug().noquote().nospace() << global::nameOutput << "[Player][Discover] Audio: " << isAudio;
    emit signal_discover_quit( isVideo, isAudio );
}


