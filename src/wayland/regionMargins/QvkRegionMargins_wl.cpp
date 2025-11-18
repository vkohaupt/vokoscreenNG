/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2025 Volker Kohaupt
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

#include "QvkRegionMargins_wl.h"
#include "QvkShowMessage_wl.h"
#include "global.h"

#include <QIcon>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPalette>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QVariantMap>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QUrl>
#include <QFileInfo>
#include <QFile>
#include <QDateTime>
#include <QThread>

/*
 * QvkRegionMargins_wl determined four margins: left, top, right, and bottom,
 * which describe the size of the borders around a window on the desktop.
 */

QvkRegionMargins_wl::QvkRegionMargins_wl(Ui_formMainWindow_wl *ui_mainwindow )
{
    ui = ui_mainwindow;

    setWindowTitle( QString( tr( "AreaMargins") ) );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setMouseTracking( true );
}


QvkRegionMargins_wl::~QvkRegionMargins_wl()
{
}


void QvkRegionMargins_wl::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

    QImage image( screenWidth, screenHeight, QImage::Format_ARGB32_Premultiplied );
    image.fill( Qt::transparent );

    QPainter painterPixmap;
    painterPixmap.begin( &image );
    QPen pen;
    pen.setColor( Qt::red );
    painterPixmap.setPen( pen );
    painterPixmap.drawLine( 0, 0, width()-1, 0 );
    painterPixmap.drawLine( width()-1, 0, width()-1, height()-1 );
    painterPixmap.drawLine( width()-1, height()-1, 0, height()-1 );
    painterPixmap.drawLine( 0, height()-1, 0, 0 );
/*
    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    painterPixmap.fillRect( 0, 0, width()-1, height()-1, brush );
    painterPixmap.end();
*/
    pixmap = pixmap.fromImage( image );

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );

    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] paintEvent()";
}


void QvkRegionMargins_wl::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event)
    screenWidth = width();
    screenHeight = height();
    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] resizeEvent()";
}


void QvkRegionMargins_wl::slot_pushButton_singleShot( bool bo )
{
    if ( bo == true ) {
        QTimer::singleShot(1000, [this]() {
            slot_pushButton_snapshot(); });
    }
    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_pushButton_singleShot()";
}


void QvkRegionMargins_wl::slot_pushButton_snapshot()
{
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface *i = new QDBusInterface("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", "org.freedesktop.portal.Screenshot", bus, NULL);

        QVariantMap options;
        options["interactive"] = false;
        QDBusReply<QDBusObjectPath> reply = i->call( "Screenshot", "", options );

        if( reply.isValid() ) {
            bus.connect( "", reply.value().path(), "org.freedesktop.portal.Request", "Response", this, SLOT( slot_handle_response_snapshot(uint,QVariantMap) ) );
            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_pushButton_snapshot()" << reply.value().path();
        } else {
            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_pushButton_snapshot() Something is wrong: " << reply.error();
        }
}


void QvkRegionMargins_wl::slot_handle_response_snapshot( uint responseCode, QVariantMap results )
{
    int top = 0;
    int right = 0;
    int bottom = 0;
    int left = 0;

    if ( responseCode == 0 ) {
        QUrl url( results["uri"].toString() );
        QFileInfo fileInfo( url.toLocalFile() );
        QString filePath_org = fileInfo.absoluteFilePath();

        QImage image_1 = QImage( filePath_org );
        image_1.save("/home/vk/Bilder/testImage.tiff", "tiff", 100);

        QImage image_2 = QImage( "/home/vk/Bilder/testImage.tiff" );
        for( int i = 0; i < image_2.height(); i++ ) {
            QColor color = image_2.pixelColor( image_2.width()/2, i );
            if ( color == Qt::red ) {
                top = i;
                qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Top margin:" << top;
                break;
            }
        }

        for( int i = image_2.width()-1; i > 0; i-- ) {
             QColor color = image_2.pixelColor( i, image_2.height()/2 );
             if ( color == Qt::red ) {
                 right = image_2.width() - 1 - i;
                 qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] right margin:" << right;
                 break;
             }
         }

        for( int i = image_2.height()-1; i > 0; i-- ) {
             QColor color = image_2.pixelColor( image_2.width()/2, i );
             if ( color == Qt::red ) {
                 bottom = image_2.height() - 1 - i;
                 qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] bottom margin:" << bottom;
                 break;
             }
         }

        for( int i = 0; i < image_2.width(); i++ ) {
            QColor color = image_2.pixelColor( i, image_2.height()/2 );
            if ( color == Qt::red ) {
                left = i;
                qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Left margin:" << left;
                break;
            }
        }

        QFile file( filePath_org );
        file.remove();

        qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_handle_response_snapshot()";
    } else {
        qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_handle_response_snapshot() Unable to take a screenshot" << results["uri"];
    }
}
