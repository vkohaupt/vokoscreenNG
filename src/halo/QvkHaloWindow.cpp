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


QvkHaloWindow::QvkHaloWindow( QWidget *parent )
{
    setParent( parent );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setCursor( Qt::BlankCursor );
    setDiameter( diameter );
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

    qreal penWith = 1.0;
    QPen pen;
    pen.setWidthF( penWith );
    pen.setColor( color );
    pen.setStyle( Qt::SolidLine );
    painterPixmap.setPen( pen );

    QBrush brush;
    brush.setColor( color );
    brush.setStyle( Qt::SolidPattern );
    painterPixmap.setBrush( color );
    painterPixmap.setOpacity( opacity );
    painterPixmap.drawEllipse( 1, 1, diameter-2, diameter-2 );

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();
}


void QvkHaloWindow::slot_followMouse()
{
    QCursor cursor;
    move( cursor.pos().x() - diameter/2, cursor.pos().y() - diameter/2 );
}


void QvkHaloWindow::setDiameter( int value )
{
    diameter = value;
    resize( diameter, diameter );

    QRegion window( 0,
                    0,
                    diameter,
                    diameter,
                    QRegion::Rectangle );

    QRegion mouseHole( diameter / 2 - 2,
                       diameter / 2 - 2,
                       4,
                       4,
                       QRegion::Rectangle );

    QRegion r1 = window.QRegion::subtracted( mouseHole );

    this->setMask( r1 );
}


void QvkHaloWindow::setOpacit( double value )
{
    opacity = value;
    repaint();
}


void QvkHaloWindow::setColor( QColor value )
{
   color = value;
   repaint();
}

