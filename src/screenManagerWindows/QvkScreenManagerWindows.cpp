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
#include <QGuiApplication>
#include <QScreen>
#include <QToolButton>

QvkScreenManagerWindows::QvkScreenManagerWindows( QMainWindow *parent )
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "video/x-raw" );
    gst_device_monitor_add_filter( monitor, "Source/Monitor", caps );
    bool isMonitorStart = gst_device_monitor_start( monitor );

    listStructure.clear();
    listDevices.clear();
    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {
        device = (GstDevice*)iterator->data;

        // Ab hier properties
        GstStructure *structure = gst_device_get_properties( device );
        listStructure.append( gst_structure_to_string( structure ) );

        QString device_name = QString( gst_structure_get_string( structure, "device.name" ) );
        device_name = device_name.replace( "\\", "").replace( ".", "" );

        guint64 value;
        gst_structure_get_uint64( structure, "device.hmonitor", &value );
        QString device_handle = QString::number( value );

        gst_structure_free( structure );
        listDevices << device_name + ":::" + device_handle;
    }

    if ( isMonitorStart == true ) {
        gst_device_monitor_stop( monitor );
    }

    QToolButton *toolButton = parent->findChild<QToolButton *>("toolButton_screen_name");
    if ( toolButton != NULL ) {
        connect( toolButton, SIGNAL( toggled( bool ) ), this, SLOT( slot_toolButton_toggled( bool ) ) );
    }
}


QvkScreenManagerWindows::~QvkScreenManagerWindows()
{
}


QStringList QvkScreenManagerWindows::get_all_screen_devices()
{
    return listDevices;
}


QStringList QvkScreenManagerWindows::get_screen_structure()
{
    return listStructure;
}


void QvkScreenManagerWindows::slot_toolButton_toggled( bool checked )
{
    QList<QScreen *> screenList = QGuiApplication::screens();

    if ( checked == true ) {
        labelList.clear();
        for ( int i = 0; i < screenList.count(); i++ ) {
            QLabel *label = new QLabel;
            label->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
            label->resize( 400, 200 );
            label->move( screenList.at(i)->geometry().x(), screenList.at(i)->geometry().y() );

            QFont font;
            font.setPixelSize( 50 );
            label->setFont( font );
            label->setStyleSheet( "QLabel { background-color : orange; color : black; }" );

            label->setAlignment( Qt::AlignCenter );
            label->setText( screenList.at(i)->name().remove( "." ).remove( '\\' ) );
            label->show();
            labelList << label;
        }
    } else {
        for ( int i = 0; i < labelList.count(); i++ ) {
            labelList.at(i)->close();
            labelList.at(i)->deleteLater();
        }
    }
}
