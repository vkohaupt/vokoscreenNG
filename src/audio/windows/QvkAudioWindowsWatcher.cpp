/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2026 Volker Kohaupt
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

#include "QvkAudioWindowsWatcher.h"
#include "global.h"

#include <QString>
#include <QMetaObject>

QvkAudioWindowsWatcher::QvkAudioWindowsWatcher(Ui_formMainWindow *ui_mainwindow)
{
    ui = ui_mainwindow;
    startAudioWindowsMonitoring();
}


QvkAudioWindowsWatcher::~QvkAudioWindowsWatcher()
{}


GstBusSyncReply QvkAudioWindowsWatcher::my_AudioWindows_func(GstBus *bus, GstMessage *message, gpointer data)
{
    Q_UNUSED(bus)

    switch ( GST_MESSAGE_TYPE( message ) ) {
    case GST_MESSAGE_DEVICE_ADDED:
    {
        GstDevice *gstDevice;
        gst_message_parse_device_added( message, &gstDevice );
        GstStructure *structure = gst_device_get_properties( gstDevice );
        QString api = QString( gst_structure_get_string( structure, "device.api" ) );
        QString device_id = QString( gst_structure_get_string( structure, "device.id" ) );
        if ( ( api == "wasapi2" ) and ( device_id.contains( "}.{" ) ) ) {
            QString device = QString( gst_structure_get_string( structure, "device.id" ) );
            QString description = QString( gst_structure_get_string( structure, "wasapi2.device.description" ) );
            gboolean boolValue;
            gst_structure_get_boolean( structure, "wasapi2.device.loopback", &boolValue ) ;
            QString type;
            if( boolValue ) {
                type = "Playback";
            } else {
                type = "Source";
            }
            QString action = "[Audio-device-added]";

            QString string = device + ":::" +
                             description + ":::" +
                             type + ":::" +
                             api + ":::" +
                             action;
            QvkAudioWindowsWatcher *self = static_cast<QvkAudioWindowsWatcher*>(data);
            QMetaObject::invokeMethod(self, [self, string](){
                emit self->signal_audio_added_removed(string);
            }, Qt::QueuedConnection);

        }
        gst_structure_free( structure );
        gst_object_unref( gstDevice );
        break;
    }
    case GST_MESSAGE_DEVICE_REMOVED:
    {
        GstDevice *gstDevice;
        gst_message_parse_device_removed( message, &gstDevice );
        GstStructure *structure = gst_device_get_properties( gstDevice );
        QString device = QString( gst_structure_get_string( structure, "device.id" ) );
        QString description = QString( gst_structure_get_string( structure, "wasapi2.device.description" ) );
        gboolean boolValue;
        gst_structure_get_boolean( structure, "wasapi2.device.loopback", &boolValue ) ;
        QString type;
        if( boolValue ) {
            type = "Playback";
        } else {
            type = "Source";
        }
        QString api  = QString( gst_structure_get_string( structure, "device.api" ) );
        QString action = "[Audio-device-removed]";

        QString string = device + ":::" +
                         description + ":::" +
                         type + ":::" +
                         api + ":::" +
                         action;
        QvkAudioWindowsWatcher *self = static_cast<QvkAudioWindowsWatcher*>(data);
        QMetaObject::invokeMethod(self, [self, string](){
            emit self->signal_audio_added_removed(string);
        }, Qt::QueuedConnection);

        gst_structure_free(structure);
        gst_object_unref(gstDevice);
        break;
    }
    default:
        break;
    }

    return GST_BUS_PASS;
}


void QvkAudioWindowsWatcher::startAudioWindowsMonitoring()
{
    GstDeviceMonitor *monitor = gst_device_monitor_new();
    GstBus *bus = gst_device_monitor_get_bus(monitor);
    GstCaps *caps = gst_caps_new_empty_simple("audio/x-raw");
    gst_device_monitor_add_filter(monitor, "Audio/Source", caps);
    gst_caps_unref(caps);
    gst_bus_set_sync_handler(bus, (GstBusSyncHandler)my_AudioWindows_func, this, nullptr);
    gst_object_unref(bus);
    gst_device_monitor_start(monitor);
}
