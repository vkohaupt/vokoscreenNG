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

#include "QvkScreenManagerWindows.h"
#include "global.h"

#include <QDebug>

QvkScreenManagerWindows::QvkScreenManagerWindows()
{
}


QvkScreenManagerWindows::~QvkScreenManagerWindows()
{
}


QStringList QvkScreenManagerWindows::get_all_Screen_devices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;
    QStringList stringList;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "video/x-raw" );
    gst_device_monitor_add_filter( monitor, "Source/Monitor", caps );
    bool isMonitorStart = gst_device_monitor_start( monitor );

    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {
        device = (GstDevice*)iterator->data;

        // Ab hier properties
        GstStructure *structure = gst_device_get_properties( device );

        QString device_name = QString( gst_structure_get_string( structure, "device.name" ) );
        device_name = device_name.replace( "\\", "").replace( ".", "" );

        guint64 value;
        gst_structure_get_uint64( structure, "device.hmonitor", &value );
        QString device_handle = QString::number( value );

        stringList << device_name + ":::" + device_handle;

        qDebug().noquote() << global::nameOutput << gst_structure_to_string( structure );
    }

    if ( isMonitorStart == true ) {
        gst_device_monitor_stop( monitor );
    }

    return stringList;
}
