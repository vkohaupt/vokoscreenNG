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
#include <QComboBox>
#include <QList>
#include <QPainter>

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

    listStructure.clear();
    listDevices.clear();
    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next ) {

        QString stringStructure;
        gint device_width = 0;
        gint device_height = 0;

        device = (GstDevice*)iterator->data;

        // First we need the properties, and in this loop we read the caps
        GstStructure *structure = gst_device_get_properties( device );
        QString device_api = QString( gst_structure_get_string( structure, "device.api" ) );
        if ( device_api == "d3d11" ) {
            stringStructure.append( "PROPERTIES: " + QString( gst_structure_to_string( structure ) ) );
            listStructure.append( stringStructure );

            QString device_primary;
            device_primary = QString( gst_structure_get_string( structure, "device.primary" ) );

            QString device_name;
            device_name = QString( gst_structure_get_string( structure, "device.name" ) );
            device_name = device_name.replace( "\\", "").replace( ".", "" );

            guint64 value;
            gst_structure_get_uint64( structure, "device.hmonitor", &value );
            QString device_handle = QString::number( value );

            gint valueRight;
            gst_structure_get_int( structure, "display.coordinates.right", &valueRight );
            int right = valueRight;
            Q_UNUSED(right)

            gint valueLeft;
            gst_structure_get_int( structure, "display.coordinates.left", &valueLeft );
            int left = valueLeft;
            Q_UNUSED(left)

            gint valueTop;
            gst_structure_get_int( structure, "display.coordinates.top", &valueTop );
            int top = valueTop;
            Q_UNUSED(top)

            gint valueBottom;
            gst_structure_get_int( structure, "display.coordinates.bottom", &valueBottom );
            int bottom = valueBottom;
            Q_UNUSED(bottom)

            // Dies funktioniert auch
            // QString device_width = QString::number( right - left );
            // QString device_height = QString::number( bottom - top );

            gst_structure_free( structure );

            // Read the caps from the device
            GstCaps *Caps = gst_device_get_caps( device );
            GstStructure *structureCaps = gst_caps_get_structure( Caps, 0 );
            stringStructure = "CAPS: " + QString( gst_structure_to_string( structureCaps ) ).replace(  ";", "," );

            if ( gst_structure_has_field( structureCaps, "format" ) ) {
                const gchar *format = NULL;
                format = gst_structure_get_string( structureCaps, "format" );
                Q_UNUSED(format)
            }

            if ( gst_structure_has_field( structureCaps, "width" ) ) {
                gint width = 0;
                if ( gst_structure_get_int( structureCaps, "width", &width ) ) {
                    device_width = width;
                }
            }

            if ( gst_structure_has_field( structureCaps, "height" ) ) {
                gint height = 0;
                if ( gst_structure_get_int( structureCaps, "height", &height ) ) {
                    device_height = height;
                }
            }

            listDevices << device_name + ":::" +
                               device_handle + ":::" +
                               QString::number( device_width ) + ":::" +
                               QString::number( device_height );
        }
    }

    QToolButton *toolButton = parent->findChild<QToolButton *>("toolButton_screen_name");
    if ( toolButton != NULL ) {
        connect( toolButton, SIGNAL( toggled(bool) ), this, SLOT( slot_toolButton_toggled(bool) ) );
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
            QScreen *screen = screenList.at(i);
            label->move( screen->geometry().x(), screen->geometry().y() );

            QFont font;
            label->setStyleSheet( "QLabel { background-color : orange; color : black; }" );

            QPixmap pixmap( 400, 200 );
            pixmap.fill( Qt::transparent );
            const QRect rec = QRect( 0, 0, 400, 200 );
            QPainter *painter = new QPainter;
            painter->begin( &pixmap );
            painter->setPen( Qt::black );
            font.setPixelSize( 12 );
            painter->setFont( font );
            painter->drawText( rec, Qt::AlignTop | Qt::AlignHCenter, global::name + " " + global::version );

            font.setPixelSize( 50 );
            painter->setFont( font );
            // Screen name from gstreamer
            painter->drawText( rec, Qt::AlignCenter,  get_all_screen_devices().at(i).section( ":::", 0, 0) );
            painter->end();

            label->setPixmap( pixmap );


            label->show();
            labelList << label;
        }
    } else {
        for ( int i = 0; i < labelList.count(); i++ ) {
            QLabel *label = labelList.at(i);
            label->close();
            label->deleteLater();
        }
    }
}
