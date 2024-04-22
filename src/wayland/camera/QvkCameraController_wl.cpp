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
    get_allCameraDevices();
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

            QString camera_name;
            camera_name = QString( gst_structure_get_string( structure, "api.v4l2.cap.card" ) );

            QString object_id;
            object_id = QString( gst_structure_get_string( structure, "object.id" ) );

            listStructure << camera_name + ":::" + object_id;

            gst_structure_free( structure );
        }
    }

    if ( isMonitorStart == true ) {
        gst_device_monitor_stop( monitor );
    }
}

void QvkCameraController_wl::set_winId( WId value )
{
    m_winID = value;
}


WId QvkCameraController_wl::get_winId()
{
    return m_winID;
}


#include <QGuiApplication>
//#include <qpa/qplatformnativeinterface.h>
//#include <gst/wayland/wayland.h>
void QvkCameraController_wl::slot_checkBoxCameraOnOff( bool bo )
{
    if ( bo == true ) {
//        vkCameraWindow_wl = new QvkCameraWindow_wl;
//        vkCameraWindow_wl->show();
//        set_winId( vkCameraWindow_wl->winId() );

        pipeline  = gst_pipeline_new( "pipeline" );
        pipewiresrc   = gst_element_factory_make( "pipewiresrc", nullptr );
        videoconvert = gst_element_factory_make( "videoconvert", nullptr );
        waylandsink = gst_element_factory_make( "xvimagesink", nullptr );
//        waylandsink = gst_element_factory_make( "waylandsink", nullptr );

        gst_bin_add_many( GST_BIN( pipeline ), pipewiresrc, videoconvert, waylandsink, nullptr );
        gst_element_link( pipewiresrc, videoconvert  );
        gst_element_link( videoconvert, waylandsink );
//        gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( waylandsink ), (guintptr)vkCameraWindow_wl->winId() );
        g_object_set( G_OBJECT( pipewiresrc ), "path", "38", NULL );
        GstStateChangeReturn sret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        if ( sret == GST_STATE_CHANGE_FAILURE ) {
            gst_element_set_state ( pipeline, GST_STATE_NULL );
            gst_object_unref( pipeline );
        }
    } else {
        gst_element_set_state( pipeline, GST_STATE_NULL );
        gst_object_unref ( pipeline );
        qDebug().noquote() << global::nameOutput << "[Camera] stop";
    }
}
