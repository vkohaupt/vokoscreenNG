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

#include "QvkAudioPipewireLevelMeter_wl.h"
#include "global.h"

#include <QDebug>
#include <QLineEdit>
#include <QTime>

#include <string.h>
#include <math.h>

#define GLIB_DISABLE_DEPRECATION_WARNINGS


QvkAudioPipewireLevelMeter_wl::QvkAudioPipewireLevelMeter_wl()
{
}


QvkAudioPipewireLevelMeter_wl::~QvkAudioPipewireLevelMeter_wl()
{
}


gboolean QvkAudioPipewireLevelMeter_wl::message_handler(GstBus *bus, GstMessage *message, gpointer index)
{
    Q_UNUSED(bus)
    // Dies wird zum testen benötigt
/*
    // https://gstreamer.freedesktop.org/documentation/gstreamer/gstmessage.html?gi-language=c
    printf("-------------------------%i\n", message->type);
    fflush(stdout);
*/
    if ( message->type == GST_MESSAGE_ELEMENT ) {
        const GstStructure *s = gst_message_get_structure( message );
        const gchar *name = gst_structure_get_name( s );

        if ( strcmp( name, "level" ) == 0 ) {
            gint channels;
            gdouble rms_dB;
            gdouble rms;
            const GValue *array_val;
            const GValue *value;
            GValueArray *rms_arr;

            // the values are packed into GValueArrays with the value per channel
            array_val = gst_structure_get_value( s, "rms" );
            rms_arr = (GValueArray *) g_value_get_boxed( array_val );

            // we can get the number of channels as the length of any of the value arrays
            channels = rms_arr->n_values;

            for ( gint i = 0; i < channels; ++i ) {
                value = rms_arr->values + i;
                rms_dB = g_value_get_double( value );

                // converting from dB to normal gives us a value between 0.0 and 1.0
                rms = pow( 10, rms_dB / 20 );

                for ( int x = 0; x < global::listChildren->count(); x++ ) {
                    QLineEdit *lineEdit = global::listChildren->at(x);
                    if (lineEdit->objectName().section("_", 1, 1) == QString::number((qint64)index)){
                        lineEdit->setText( QString::number(rms) );
                        // Dies wird zum testen benötigt
                        /*QTime current = QTime::currentTime();
                        QString currentTime = current.toString();
                        printf("%s %s  %f \n",
                               currentTime.toLatin1().data(),
                               QString::number((qint64)index).toLatin1().data(),
                               rms);
                        fflush(stdout);*/
                        break;
                    }
                }
            }
        }
    }

    return TRUE;
}


// Für den dritten Parameter index nehmen wir ebenfalls die deviceID da diese eindeutig ist.
void QvkAudioPipewireLevelMeter_wl::start(QString deviceID, QString myname, QString index)
{
    GstElement *audiosrc, *audioconvert, *level, *fakesink;
    GstCaps *caps;
    GstBus *bus;
    m_deviceID = deviceID;

    caps = gst_caps_from_string( "audio/x-raw,channels=2" );

    pipeline = gst_pipeline_new( nullptr );
    g_assert (pipeline);
    audiosrc = gst_element_factory_make( "pipewiresrc", nullptr );
    g_assert (audiosrc);
    audioconvert = gst_element_factory_make( "audioconvert", nullptr );
    g_assert (audioconvert);
    level = gst_element_factory_make( "level", nullptr );
    g_assert (level);
    fakesink = gst_element_factory_make( "fakesink", nullptr );
    g_assert (fakesink);

    gst_bin_add_many( GST_BIN( pipeline ), audiosrc, audioconvert, level, fakesink, nullptr );
    if ( !gst_element_link( audiosrc, audioconvert ) ) {
        g_error( "Failed to link audiosrc and audioconvert" );
    }
    if (!gst_element_link_filtered( audioconvert, level, caps ) ) {
        g_error( "Failed to link audioconvert and level" );
    }
    if ( !gst_element_link( level, fakesink ) ) {
        g_error( "Failed to link level and fakesink" );
    }

    g_object_set( G_OBJECT( audiosrc ), "target-object", deviceID.toUtf8().constData(), nullptr );

    QString m_name = myname;
    g_object_set( G_OBJECT( audiosrc ), "client-name", m_name.toUtf8().constData(), nullptr );

    // make sure we'll get messages
    g_object_set( G_OBJECT( level ), "post-messages", TRUE, nullptr );

    // run synced and not as fast as we can
    g_object_set( G_OBJECT( fakesink ), "sync", TRUE, nullptr );

    // Setzt den Intervall. Acht Nullen sind ca. 15Aufrufe/Sekunde
    //                      Sieben Nullen sind ca. 100Aufrufe/Sekunde
    g_object_set( G_OBJECT( level ), "interval", 10000000, nullptr );

    bus = gst_element_get_bus (pipeline);

    gint64 msg = index.toInt();
    gst_bus_set_sync_handler( bus, (GstBusSyncHandler)message_handler, (gpointer)msg, nullptr );
    gst_object_unref(bus);

    GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
    if (ret == GST_STATE_CHANGE_FAILURE){
        qDebug().noquote() << global::nameOutput
                           << "[Audio][levelmeter]"
                           << deviceID
                           << "GST_STATE_CHANGE_FAILURE Returncode ="
                           << ret;
        gst_object_unref( pipeline );
        return;
    } // 0
    if (ret == GST_STATE_CHANGE_SUCCESS){
        qDebug().noquote() << global::nameOutput
                           << "[Audio][LevelMeter]"
                           << deviceID
                           << "GST_STATE_CHANGE_SUCCESS Returncode ="
                           << ret;
    } // 1
    if (ret == GST_STATE_CHANGE_ASYNC){
        qDebug().noquote() << global::nameOutput
                           << "[Audio][LevelMeter]"
                           << deviceID
                           << "Start GST_STATE_CHANGE_ASYNC Returncode ="
                           << ret;
    } // 2
    if (ret == GST_STATE_CHANGE_NO_PREROLL){
        qDebug().noquote() << global::nameOutput
                           << "[Audio][LevelMeter]"
                           << deviceID
                           << "GST_STATE_CHANGE_NO_PREROLL Returncode ="
                           << ret;
    }// 3
    qDebug().noquote();

}


void QvkAudioPipewireLevelMeter_wl::stop()
{
    qDebug().noquote() << global::nameOutput << "[Audio][levelmeter]" << m_deviceID << "Stop is clicked";
    GstState state; GstState pending;
    gst_element_get_state(pipeline, &state, &pending, 1000000000);// 1 Millionen Nanosekunden sind eine Sekunde
    if (state == GST_STATE_PLAYING){
        qDebug().noquote() << global::nameOutput << "[Audio][levelmeter]" << m_deviceID << "State is PLAYING";
        qDebug().noquote() << global::nameOutput << "[Audio][levelmeter]" << m_deviceID << "State wants to switch to State_NULL";
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_element_get_state(pipeline, &state, &pending, 1000000000);
        if (state == GST_STATE_NULL){
            qDebug().noquote() << global::nameOutput << "[Audio][LevelMeter]" << m_deviceID << "State is State_NULL";
            qDebug().noquote() << global::nameOutput << "[Audio][LevelMeter]" << m_deviceID << "Stop";
        }
    }
    if (state == GST_STATE_PAUSED){
        qDebug().noquote() << global::nameOutput << "[Audio][LevelMeter]" << m_deviceID << "State is PAUSE";
        gst_element_set_state(pipeline, GST_STATE_NULL);
    }
}
