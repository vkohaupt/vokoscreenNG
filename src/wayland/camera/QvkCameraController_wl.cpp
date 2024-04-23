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

    // Add Devices
    QStringList listDevices = get_allCameraDevices();
    for ( int i = 0; i < listDevices.count(); i++ ) {
        QvkCameraSingle_wl *vkCameraSingle_wl = new QvkCameraSingle_wl( ui, listDevices.at(i) );
    }

    // Am Ende ein spaceritem einfügen
    QSpacerItem *spacerItem = new QSpacerItem( 100,100, QSizePolicy::Expanding, QSizePolicy::Expanding );
    ui->layoutAllCameras->addItem( spacerItem );
}


QvkCameraController_wl::~QvkCameraController_wl()
{
}

QStringList QvkCameraController_wl::get_allCameraDevices()
{
    GstDeviceMonitor *monitor = Q_NULLPTR;
    GstCaps *caps = Q_NULLPTR;
    GstDevice *device = Q_NULLPTR;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "video/x-raw" );
    gst_device_monitor_add_filter( monitor, "Video/Source", caps );
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
            gst_structure_free( structure );
        }
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
