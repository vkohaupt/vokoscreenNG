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

QvkHaloWindow::QvkHaloWindow( QWidget *parent )
{
    setParent( parent );
    setCursor( Qt::BlankCursor );
    setAttribute( Qt::WA_TranslucentBackground, true );
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
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkHaloWindow::setDiameter( int value )
{
    diameter = value;
    resize( diameter, diameter );
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

