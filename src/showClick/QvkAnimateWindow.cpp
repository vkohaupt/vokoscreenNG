/* vokoShowClick
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

#include "QvkAnimateWindow.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QBitmap>


QvkAnimateWindow::QvkAnimateWindow( QWidget *parent )
{
  setParent( parent );
}


void QvkAnimateWindow::init( int x,
                             int y,
                             int showTime,
                             QString button,
                             int valueDiameter,
                             double valueOpacity,
                             QColor valueColor )

{
    setAttribute( Qt::WA_TranslucentBackground, true );
    resize( 110, 110 );
    move( x / devicePixelRatioF() - ( width() / 2 ), y / devicePixelRatioF() - ( height() / 2 ) );

    QRegion window( 0,
                    0,
                    width(),
                    height(),
                    QRegion::Rectangle );

    QRegion mouseHole( width() / 2 - 1,
                       height() / 2 - 1,
                       2,
                       2,
                       QRegion::Rectangle );

    QRegion r1 = window.QRegion::subtracted( mouseHole );

    this->setMask( r1 );

    mouseButton = button;
    diameter = valueDiameter;
    opacity = valueOpacity / 100;
    color = valueColor;

    QTimer::singleShot( showTime, this, SLOT( close() ) );

    show();
}


QvkAnimateWindow::~QvkAnimateWindow()
{
}


void QvkAnimateWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPixmap pixmap( height() * devicePixelRatioF(), width() * devicePixelRatioF() );
    pixmap.fill( Qt::transparent );
    pixmap.setDevicePixelRatio( devicePixelRatioF() );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHints( QPainter::Antialiasing, true );

    qreal penWith = 6.0;
    QPen pen;
    pen.setWidthF( penWith );
    pen.setColor( color );
    pen.setStyle( Qt::SolidLine );
    painterPixmap.setPen( pen );
    painterPixmap.setBrush( Qt::NoBrush );
    painterPixmap.setOpacity( opacity );
    painterPixmap.drawEllipse( width()/2-diameter/2, height()/2-diameter/2, diameter, diameter );

    // Paint pressed Button
    pen.setStyle( Qt::SolidLine );
    pen.setWidthF( 3.0 );
    painterPixmap.setPen( pen );
    painterPixmap.setOpacity( opacity );
    if ( mouseButton == "LeftButton" )
    {
        QRectF rectF( width()/2 - diameter/2 + penWith, height()/2 - diameter/2 + penWith, diameter-2*penWith, diameter-2*penWith );
        int startAngle = 90 * 16;
        int spanAngle = 180 * 16;
        painterPixmap.drawArc( rectF, startAngle, spanAngle );
    }

    if ( mouseButton == "RightButton" )
    {
        QRectF rectF( width()/2 - diameter/2 + penWith, height()/2 - diameter/2 + penWith, diameter-2*penWith, diameter-2*penWith );
        int startAngle = -90 * 16;
        int spanAngle = 180 * 16;
        painterPixmap.drawArc( rectF, startAngle, spanAngle );
    }

    if ( mouseButton == "MiddleButton" )
    {
        QLineF line( width()/2, height()/2 - diameter/2 + penWith , width()/2, height()/2 + diameter/2 - penWith );
        painterPixmap.drawLine( line );
    }

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();
}
