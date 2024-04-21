/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2023 Volker Kohaupt
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
#include "global.h"

#include <QDebug>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QList>

QvkCameraController_wl::QvkCameraController_wl( Ui_formMainWindow_wl *ui_surface )
{
    ui = ui_surface;

    connect( ui->checkBoxCameraOnOff, SIGNAL( clicked(bool) ), this, SLOT( slot_checkBoxCameraOnOff(bool) ) );
}


QvkCameraController_wl::~QvkCameraController_wl()
{
}

void QvkCameraController_wl::get_allCameraDevices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;

    QStringList listStructure;
    QStringList listDevices;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "video/x-raw" );
    gst_device_monitor_add_filter( monitor, "Video/Source", caps );
    bool isMonitorStart = gst_device_monitor_start( monitor );

    listStructure.clear();
    listDevices.clear();
    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {

        QString stringStructure;
        device = (GstDevice*)iterator->data;

        // From here properties
        GstStructure *structure = gst_device_get_properties( device );
        if ( structure != NULL ) {
            stringStructure.append( "PROPERTIES: " + QString( gst_structure_to_string( structure ) ) );
            listStructure.append( stringStructure );

            QString object_id;
            object_id = QString( gst_structure_get_string( structure, "object.id" ) );

            QString camera_name;
            camera_name = QString( gst_structure_get_string( structure, "api.v4l2.cap.card" ) );

            qDebug() << "---------------------------------------" << object_id << camera_name;
            qDebug() << listStructure;

            gst_structure_free( structure );
        }
    }

    if ( isMonitorStart == true ) {
        gst_device_monitor_stop( monitor );
    }
}


void QvkCameraController_wl::slot_checkBoxCameraOnOff( bool bo )
{
    if ( bo == true ) {
        get_allCameraDevices();
//        QString launch = "pipewiresrc path=59 ! video/x-raw, width=160, height=120, framerate=10/1, format=I420 ! videoconvert ! xvimagesink";
        QString launch = "pipewiresrc path=59 ! videoconvert ! waylandsink";
        pipelineCamera = gst_parse_launch( launch.toUtf8(), nullptr );
        gst_element_set_state( pipelineCamera, GST_STATE_PLAYING );
        qDebug().noquote() << global::nameOutput << "[Camera] Start with:" << launch;
    } else {
        gst_element_set_state( pipelineCamera, GST_STATE_NULL );
        gst_object_unref ( pipelineCamera );
        qDebug().noquote() << global::nameOutput << "[Camera] stop";
    }
}
