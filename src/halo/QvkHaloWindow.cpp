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
    show();

    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_followMouse() ) );
}


void QvkHaloWindow::slot_followMouse()
{
    QCursor cursor;
    move( cursor.pos().x() - diameter/2, cursor.pos().y() - diameter/2 );
}


void QvkHaloWindow::init()
{
    diameter = 100;
    opacity = 0.5;
    color = Qt::yellow;

    QCursor cursor;
    int x = cursor.pos().x() - diameter/2;
    int y = cursor.pos().y() - diameter/2;

    resize( diameter, diameter );
    move( x / devicePixelRatioF() - ( diameter / 2 ), y / devicePixelRatioF() - ( diameter / 2 ) );

    QRegion window( 0,
                    0,
                    diameter,
                    diameter,
                    QRegion::Rectangle );

    QRegion mouseHole( diameter / 2 - 1,
                       diameter / 2 - 1,
                       2,
                       2,
                       QRegion::Rectangle );

    QRegion r1 = window.QRegion::subtracted( mouseHole );

    this->setMask( r1 );

    show();
    timer->start( 40 );

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
    pen.setColor( Qt::red );
    pen.setStyle( Qt::SolidLine );
    painterPixmap.setPen( pen );

    painterPixmap.drawLine( 0, pixmap.height()/2, pixmap.width(), pixmap.height()/2 );
    painterPixmap.drawLine( pixmap.width()/2, 0, pixmap.width()/2, pixmap.height() );

    QBrush brush;
    brush.setColor( color );
    brush.setStyle( Qt::SolidPattern );
    painterPixmap.setBrush( color );
    painterPixmap.setOpacity( opacity );
    painterPixmap.drawEllipse( 0, 0, diameter, diameter );

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();
}
