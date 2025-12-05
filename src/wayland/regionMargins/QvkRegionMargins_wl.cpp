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
#include "global.h"

#include <QIcon>
#include <QSize>
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
#include <QImage>
#include <QBrush>
#include <QTimer>
#include <QColor>

/*
 * QvkRegionMargins_wl determined four margins: left, top, right, and bottom,
 * which describe the size of the borders around a window on the desktop.
 */

QvkRegionMargins_wl::QvkRegionMargins_wl( Ui_formMainWindow_wl *ui_mainwindow )
{
    ui = ui_mainwindow;

    setWindowTitle( QString( "AreaMargins" ) );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setMouseTracking( true );

    // Original vokoscreen color is "#3DAEE9"
    // We'll choose a similar but not the same color.
    color = "#5DAEE9";
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

    QBrush brush;
    brush.setColor( color );
    brush.setStyle(Qt::SolidPattern);
    painterPixmap.fillRect( 0, 0, width(), lineWidth, brush );
    painterPixmap.fillRect( width()-lineWidth, 0, lineWidth, height(), brush );
    painterPixmap.fillRect( 0, height()-lineWidth, width(), lineWidth, brush );
    painterPixmap.fillRect( 0, 0, lineWidth, height(), brush );
    painterPixmap.end();

/*
    QBrush brush;
    brush.setColor( color );
    brush.setStyle(Qt::SolidPattern);
    painterPixmap.fillRect( 0, 0, width(), height(), brush );

    QPen pen;
    pen.setColor( color );
    painterPixmap.setPen( pen );
    painterPixmap.drawLine( 0, 0, width()-1, 0 );
    painterPixmap.drawLine( width()-1, 0, width()-1, height()-1 );
    painterPixmap.drawLine( width()-1, height()-1, 0, height()-1 );
    painterPixmap.drawLine( 0, height()-1, 0, 0 );
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


void QvkRegionMargins_wl::slot_singleShot( bool bo )
{
    if ( bo == true ) {
        showMaximized();
        QTimer::singleShot(1000, [this]() { slot_snapshot(); });
    }
    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_singleShot()";
}


void QvkRegionMargins_wl::slot_snapshot()
{
        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface *i = new QDBusInterface("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", "org.freedesktop.portal.Screenshot", bus, NULL);

        QVariantMap options;
        options["interactive"] = false;
        QDBusReply<QDBusObjectPath> reply = i->call( "Screenshot", "", options );

        if( reply.isValid() ) {
            bus.connect( "", reply.value().path(), "org.freedesktop.portal.Request", "Response", this, SLOT( slot_handle_response_snapshot(uint,QVariantMap) ) );
            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_snapshot()" << reply.value().path();
        } else {
            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_snapshot() Something is wrong: " << reply.error();
        }
}


void QvkRegionMargins_wl::slot_handle_response_snapshot( uint responseCode, QVariantMap results )
{
    if ( responseCode == 0 ) {
        QUrl url( results["uri"].toString() );
        QFileInfo fileInfo( url.toLocalFile() );
        QString filePath_org = fileInfo.absoluteFilePath();

        if ( filePath_org > "" ) {
            /*
            * Hinweis für alle Ränder
            * Die Pixel werden von der Mitte zum Rand hin ausgwertet.
            */
            QImage image = QImage( filePath_org );
            for( int i = image.height()/2; i > 0; i-- ) {
                QColor pixelColor = image.pixelColor( image.width()/2, i );
                if ( pixelColor == color ) {
                    top = i + 1 - lineWidth;
                    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Top margin:" << top;
                    break;
                }
            }
            
            for( int i = image.width()/2; i < image.width(); i++ ) {
                QColor pixelColor = image.pixelColor( i, image.height()/2 );
                if ( pixelColor == color ) {
                    right = image.width() - i - lineWidth;
                    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Right margin:" << right;
                    break;
                }
            }
            
            for( int i = image.height()/2; i < image.height(); i++ ) {
                QColor pixelColor = image.pixelColor( image.width()/2, i );
                if ( pixelColor == color ) {
                    bottom = image.height() - i - lineWidth;
                    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Bottom margin:" << bottom;
                    break;
                }
            }
            
            for( int i = image.width()/2; i > 0; i-- ) {
                QColor pixelColor = image.pixelColor( i, image.height()/2 );
                if ( pixelColor == color ) {
                    left = i + 1 - lineWidth;
                    qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Left margin:" << left;
                    break;
                }
            }
            
            QFile file( filePath_org );
            file.remove();

            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_handle_response_snapshot()" << size();

            setVisible( false );

            emit signal_regionMargins();
        }
    } else {
        setVisible( false );
        qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] slot_handle_response_snapshot() Unable to take a screenshot" << results["uri"];
    }

}


int QvkRegionMargins_wl::get_top()
{
    return top;
}


int QvkRegionMargins_wl::get_right()
{
    return right;
}


int QvkRegionMargins_wl::get_bottom()
{
    return bottom;
}


int QvkRegionMargins_wl::get_left()
{
    return left;
}
