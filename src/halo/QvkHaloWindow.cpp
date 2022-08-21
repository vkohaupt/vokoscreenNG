/* vokoscreenNG
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#include "QvkHaloWindow.h"

#include <QPainter>
#include <QDebug>
#include <QBitmap>
#include <QGuiApplication>
#include <QPaintEvent>

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif

QvkHaloWindow::QvkHaloWindow( QWidget *parent )
{
    Q_UNUSED(parent)

    setWindowFlags( Qt::WindowTransparentForInput | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );

#ifdef Q_OS_LINUX
    if ( QX11Info::isCompositingManagerRunning() == true )
    {
        setAttribute( Qt::WA_TranslucentBackground, true );
    } else
    {
        setAttribute( Qt::WA_TranslucentBackground, false );
    }
#endif

#ifdef Q_OS_WIN
    setAttribute( Qt::WA_TranslucentBackground, true );
#endif

    bool debug = false;

    globalCursorPos = QCursor::pos();
    if( debug == true ) { qDebug() << "QvkHaloWindow::paintEvent globalCursorPos:" << globalCursorPos; }

    screen = QGuiApplication::screenAt( globalCursorPos );
    if ( screen ) {
        if( debug == true ) { qDebug() << "QvkHaloWindow::paintEvent screen:" << screen; }
    } else {
        // screenAt found no screen. Without a return the Application is crashed
        return;
    }

    screenCursorPos = globalCursorPos - screen->geometry().topLeft(); // screenCursorPos beginnt bei 0 auf dem jeweiliegen Bildschirm
    if( debug == true ) { qDebug() << "QvkHaloWindow::paintEvent screenCursorPos:" << screenCursorPos; }
}


QvkHaloWindow::~QvkHaloWindow()
{
}


void QvkHaloWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    bool debug = false;

    globalCursorPos = QCursor::pos();
    if( debug == true ) { qDebug() << "QvkHaloWindow::paintEvent globalCursorPos:" << globalCursorPos; }

    screen = QGuiApplication::screenAt( globalCursorPos );
    if ( screen ) {
        if( debug == true ) { qDebug() << "QvkHaloWindow::paintEvent screen:" << screen; }
    } else {
        // screenAt found no screen. Without a return the Application is crashed
        return;
    }

    screenCursorPos = globalCursorPos - screen->geometry().topLeft(); // screenCursorPos beginnt bei 0 auf dem jeweiliegen Bildschirm
    if( debug == true ) { qDebug() << "QvkHaloWindow::paintEvent screenCursorPos:" << screenCursorPos; }

    if( debug == true ) { qDebug() << "----------"; }

#ifdef Q_OS_LINUX
    if ( QX11Info::isCompositingManagerRunning() == true )
    {
#endif
        QPainter painter;
        painter.begin( this );
        painter.setRenderHints( QPainter::Antialiasing, true );

        qreal penWidth = diameter/2 - holeRadius;
        QPen pen;
        pen.setWidthF( penWidth );
        pen.setColor( color );
        painter.setPen( pen );
        painter.setOpacity( opacity );

        qreal x1 = screenCursorPos.x();
        qreal y1 = screenCursorPos.y();
        qreal width =  holeRadius + penWidth/2;
        qreal height = holeRadius + penWidth/2;
        painter.drawEllipse( QPointF( x1, y1 ), width, height );

        painter.end();

#ifdef Q_OS_LINUX
    }
#endif

#ifdef Q_OS_LINUX
    if ( QX11Info::isCompositingManagerRunning() == false )
    {
        QPixmap pixmap( screen->size().width() * devicePixelRatioF(),
                        screen->size().height() * devicePixelRatioF() );
        pixmap.fill( Qt::transparent );
        pixmap.setDevicePixelRatio( devicePixelRatioF() );

        QPainter painterPixmap;
        painterPixmap.begin( &pixmap );
        painterPixmap.setRenderHints( QPainter::Antialiasing, true );

        qreal penWidth = diameter/2 - holeRadius;
        QPen pen;
        pen.setWidthF( penWidth );
        pen.setColor( color );
        painterPixmap.setPen( pen );
        painterPixmap.setOpacity( opacity );

        qreal x1 = screenCursorPos.x();
        qreal y1 = screenCursorPos.y();
        qreal width =  holeRadius + penWidth/2;
        qreal height = holeRadius + penWidth/2;
        painterPixmap.drawEllipse( QPointF( x1, y1 ), width, height );

        painterPixmap.end();

        QPainter painter;
        painter.begin( this );
        painter.drawPixmap( QPointF( 0, 0 ), pixmap );
        painter.end();

        clearMask();
        setMask( pixmap.mask() );
    }
#endif

}


void QvkHaloWindow::setDiameter( int value )
{
    diameter = value;
    repaint();
}


void QvkHaloWindow::setHoleRadius( int value )
{
    holeRadius = value;
    repaint();
}


void QvkHaloWindow::setOpacity( qreal value )
{
    opacity = value;
    repaint();
}


void QvkHaloWindow::setColor( QColor value )
{
    color = value;
    repaint();
}

