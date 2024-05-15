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

#include "QvkWASAPIGstreamer.h"

#include <QDebug>

QvkWASAPIGstreamer::QvkWASAPIGstreamer()
{
}


QvkWASAPIGstreamer::~QvkWASAPIGstreamer()
{
}

QStringList QvkWASAPIGstreamer::get_all_Audio_Source_devices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "audio/x-raw" );
    gst_device_monitor_add_filter( monitor, "Audio/Source", caps );

    listStructure.clear();
    listDevices.clear();
    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {

        QString stringStructure;
        device = (GstDevice*)iterator->data;

        // From here caps
        GstCaps *Caps = gst_device_get_caps( device );
        GstStructure *structureCaps = gst_caps_get_structure( Caps, 0 );
        stringStructure = "CAPS: " + QString( gst_structure_to_string( structureCaps ) );

        // From here properties
        GstStructure *structure = gst_device_get_properties( device );
        stringStructure.append( "PROPERTIES: " + QString( gst_structure_to_string( structure ) ) );
        listStructure.append( stringStructure );

        QString device_api = QString( gst_structure_get_string( structure, "device.api" ) );
        QString device_id = QString( gst_structure_get_string( structure, "device.id" ) );
        if ( ( device_api == "wasapi2" ) and ( device_id.contains( "MMDEVAPI" ) ) ) {
            QString device_id = QString( gst_structure_get_string( structure, "device.id" ) );
            QString device_description = QString( gst_structure_get_string( structure, "wasapi2.device.description" ) );
            gboolean boolValue;
            gst_structure_get_boolean( structure, "wasapi2.device.loopback", &boolValue ) ;
            QString loopback;
            if( boolValue ) {
                loopback = "Playback";
            } else {
                loopback = "Source";
            }
            listDevices << device_id + ":::" + device_description + ":::" + loopback + ":::" + device_api;
        }

        gst_structure_free( structure );
    }

    return listDevices;
}

