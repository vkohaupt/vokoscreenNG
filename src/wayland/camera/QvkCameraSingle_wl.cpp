﻿/* vokoscreenNG - A desktop recorder
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

#include "ui_formMainWindow_wl.h"
#include "QvkCameraSingle_wl.h"
#include "global.h"

#include <QDebug>
#include <QRadioButton>
#include <QObject>
#include <QList>
#include <QImage>
#include <QIcon>
#include <QPainterPath>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRectF>

QvkCameraSingle_wl::QvkCameraSingle_wl( Ui_formMainWindow_wl *ui_surface, QString device )
{
    ui = ui_surface;

    device_id = device.section( ":::", 0, 0 );
    device_name = device.section( ":::", 1, 1 );

    QCheckBox *checkBoxCameraOnOff = new QCheckBox;
    checkBoxCameraOnOff->setText( device_name );
    ui->layoutAllCameras->addWidget( checkBoxCameraOnOff );
    connect( checkBoxCameraOnOff, SIGNAL( clicked(bool) ), this, SLOT( slot_checkBoxCameraOnOff(bool) ) );
}


QvkCameraSingle_wl::~QvkCameraSingle_wl()
{
}


#include <QGuiApplication>
//#include <qpa/qplatformnativeinterface.h>
//#include <gst/wayland/wayland.h>
void QvkCameraSingle_wl::slot_checkBoxCameraOnOff( bool bo )
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
        g_object_set( G_OBJECT( pipewiresrc ), "path", device_id, NULL );
        qDebug() << "--------------------------" << device_id;
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
