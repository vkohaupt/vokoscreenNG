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

#include "QvkCameraController_wl.h"
#include "QvkCameraSingle_wl.h"
#include "global.h"

#include <QDebug>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QList>

#include "glib.h"
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>
#include "gst/video/videooverlay.h"

QvkCameraController_wl::QvkCameraController_wl( Ui_formMainWindow_wl *ui_surface )
{
    ui = ui_surface;
    test123();

    // Am Ende ein spaceritem einfügen
    QSpacerItem *spacerItem = new QSpacerItem( 100,100, QSizePolicy::Expanding, QSizePolicy::Expanding );
    ui->layoutAllCameras->addItem( spacerItem );
}


QvkCameraController_wl::~QvkCameraController_wl()
{
}


static gboolean my_bus_func( GstBus *bus, GstMessage *message, gpointer user_data )
{
    GstDevice *device;

    switch ( GST_MESSAGE_TYPE( message ) ) {
    case GST_MESSAGE_DEVICE_ADDED:
    {
        gst_message_parse_device_added( message, &device );

        GstStructure *structure = gst_device_get_properties( device );
        if ( structure != NULL ) {
            QString device_api = QString( gst_structure_get_string( structure, "device.api" ) );
            if ( device_api == "v4l2") {
                QString object_id = QString( gst_structure_get_string( structure, "object.serial" ) );
                QString camera_name = QString( gst_structure_get_string( structure, "api.v4l2.cap.card" ) );
                qDebug().noquote() << global::nameOutput << "[Camera added:]" << object_id << camera_name;
                // listDevices << object_id + ":::" + camera_name;
            }
        }
        gst_object_unref( device );
        break;
    }
    case GST_MESSAGE_DEVICE_REMOVED:
    {
        gst_message_parse_device_removed( message, &device );

        GstStructure *structure = gst_device_get_properties( device );
        if ( structure != NULL ) {
            QString device_api = QString( gst_structure_get_string( structure, "device.api" ) );
            if ( device_api == "v4l2") {
                QString object_id = QString( gst_structure_get_string( structure, "object.serial" ) );
                QString camera_name = QString( gst_structure_get_string( structure, "api.v4l2.cap.card" ) );
                qDebug().noquote() << global::nameOutput << "[Camera removed:]" << object_id << camera_name;
                // listDevices << object_id + ":::" + camera_name;
            }
        }
        gst_object_unref( device );
        break;
    }
    default:
        break;
    }
    return G_SOURCE_CONTINUE;
}


void QvkCameraController_wl::test123()
{
    GstDeviceMonitor *monitor;
    GstBus *bus;
    GstCaps *caps;

    monitor = gst_device_monitor_new();
    bus = gst_device_monitor_get_bus( monitor );
    gst_bus_add_watch( bus, my_bus_func, NULL );
    gst_object_unref( bus );

    caps = gst_caps_new_empty_simple( "video/x-raw" );
    gst_device_monitor_add_filter( monitor, "Video/Source", caps );
    gst_caps_unref( caps );

    gst_device_monitor_start( monitor );
}


QStringList QvkCameraController_wl::get_allCameraDevices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    GList *iterator;
    GList *list;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "video/x-raw" );
    gst_device_monitor_add_filter( monitor, "Video/Source", caps );
    gst_caps_unref( caps );
    bool isMonitorStart = gst_device_monitor_start( monitor );

    listDevices.clear();
    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {
        device = (GstDevice*)iterator->data;
        GstStructure *structure = gst_device_get_properties( device );
        if ( structure != NULL ) {
            QString device_api;
            device_api = QString( gst_structure_get_string( structure, "device.api" ) );
            if ( device_api == "v4l2") {
                QString object_id;
                // object_id = QString( gst_structure_get_string( structure, "object.id" ) );
                object_id = QString( gst_structure_get_string( structure, "object.serial" ) );

                QString camera_name;
                camera_name = QString( gst_structure_get_string( structure, "api.v4l2.cap.card" ) );
                qDebug().noquote() << global::nameOutput << "[Camera]" << object_id << camera_name;
                listDevices << object_id + ":::" + camera_name;
            }
        }
        gst_structure_free( structure );
    }

    if ( isMonitorStart == true ) {
        gst_device_monitor_stop( monitor );
    }

    return listDevices;
}

void QvkCameraController_wl::set_winId( WId value )
{
    m_winID = value;
}


WId QvkCameraController_wl::get_winId()
{
    return m_winID;
}
