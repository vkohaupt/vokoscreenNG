/* vokoscreenNG
 * Copyright (C) 2017-2021 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif

QvkHaloWindow::QvkHaloWindow( QWidget *parent )
{
    setParent( parent );
    setCursor( Qt::BlankCursor );

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

    show();
}


QvkHaloWindow::~QvkHaloWindow()
{
}


void QvkHaloWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPixmap pixmap( 100 * devicePixelRatioF(), 100 * devicePixelRatioF() );
    pixmap.fill( Qt::transparent );
    pixmap.setDevicePixelRatio( devicePixelRatioF() );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHints( QPainter::Antialiasing, true );

    qreal holeRadius = 3;
    qreal penWidth = diameter/2 - holeRadius;
    QPen pen;
    pen.setWidthF( penWidth );
    pen.setColor( color );
    painterPixmap.setPen( pen );
    painterPixmap.setOpacity( opacity );
    painterPixmap.drawEllipse( QPointF( diameter/2, diameter/2 ), holeRadius + penWidth/2, holeRadius + penWidth/2);

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkHaloWindow::slot_followMouse()
{
    move( QCursor::pos().x() - diameter/2, QCursor::pos().y() - diameter/2 );
}


void QvkHaloWindow::setDiameter( int value )
{
    diameter = value;
    resize( diameter, diameter );
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

