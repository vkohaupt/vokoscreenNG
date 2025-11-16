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

/*
 * QvkRegionMargins_wl determined four margins: left, top, right, and bottom,
 * which describe the size of the borders around a window on the desktop.
 */

QvkRegionMargins_wl::QvkRegionMargins_wl( QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *ui_mainwindow )
{
    mainWindow = vkMainWindow;
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
    painterPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );
    QPen pen;
    pen.setColor( Qt::red );
    painterPixmap.setPen( pen );
    painterPixmap.drawLine( 0, 0, width()-1, 0 );
    painterPixmap.drawLine( width()-1, 0, width()-1, height()-1 );
    painterPixmap.drawLine( width()-1, height()-1, 0, height()-1 );
    painterPixmap.drawLine( 0, height()-1, 0, 0 );
    painterPixmap.end();

    pixmap = pixmap.fromImage( image );

    QPainter painter;
    painter.begin( this );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );

    qDebug() << "paintEvent";
}


void QvkRegionMargins_wl::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event)
    screenWidth = width();
    screenHeight = height();
    qDebug() << "resizeEvent";
}


void QvkRegionMargins_wl::slot_pushButton_snapshot( bool bo )
{
    if ( bo == true ) {
        setVisible( true );

        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusInterface *i = new QDBusInterface("org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", "org.freedesktop.portal.Screenshot", bus, NULL);

        QVariantMap options;
        options["interactive"] = false;
        QDBusReply<QDBusObjectPath> reply = i->call( "Screenshot", "", options );

        if( reply.isValid() ) {
            bus.connect( "", reply.value().path(), "org.freedesktop.portal.Request", "Response", this, SLOT( slot_handle_response_snapshot(uint,QVariantMap) ) );
            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl]" << reply.value().path();
        } else {
            qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Something is wrong: " << reply.error();
        }
    } else {
        setVisible( false );
    }
}


void QvkRegionMargins_wl::slot_handle_response_snapshot( uint responseCode, QVariantMap results )
{
    if ( responseCode == 0 ) {
        QUrl url( results["uri"].toString() );
        QFileInfo fileInfo( url.toLocalFile() );
        QString path_to_snapshot_folder = fileInfo.absolutePath();
        QString filePath_org = fileInfo.absoluteFilePath();

        QPixmap pixmap = QPixmap( filePath_org );
        QString filePath_new = path_to_snapshot_folder +
                "/" +
                global::name +
                "-" +
                QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss.zzz" ) +
                "." +
                ui->comboBoxSnapshotImageFormats->currentText().toUtf8();
        pixmap.save( filePath_new );

        QFile file( filePath_org );
        file.remove();

        QvkShowMessage_wl *vkShowMessage_wl = new QvkShowMessage_wl();
        vkShowMessage_wl->set_StatusIcon( ":/pictures/status/information.png" );
        vkShowMessage_wl->set_Image( filePath_new );
        vkShowMessage_wl->set_timeOut( 10000 );
        vkShowMessage_wl->showMessage( "" );
        vkShowMessage_wl->set_folderPath( path_to_snapshot_folder );

        qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Saved under:" << filePath_new;
    } else {
        qDebug().noquote() << global::nameOutput << "[QvkRegionMargins_wl] Unable to take a screenshot" << results["uri"];
    }
}

