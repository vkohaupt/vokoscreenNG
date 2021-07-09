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

#include "QvkHaloCircleWidget.h"

QvkHaloCircleWidget::QvkHaloCircleWidget( QWidget *parent ) : QWidget( parent )
{
    parentWidget = parent;

    color = Qt::yellow;
    diameter = 50;
    opacity = 0.7;
}


QvkHaloCircleWidget::~QvkHaloCircleWidget()
{
}


void QvkHaloCircleWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);


    QPainter painter;
    painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );

    if ( color == Qt::white )
    {
        painter.fillRect( 0, 0, width(), height(), QColor( 245, 245, 245, 0 ) );
    }
    else
    {
        painter.fillRect( 0, 0, width(), height(), Qt::white );
    }


    painter.end();

/*
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
    painter.end(); */
}


void QvkHaloCircleWidget::setColor( QColor value )
{
    color = value;
    repaint();
}


QColor QvkHaloCircleWidget::getColor()
{
    return color;
}


void QvkHaloCircleWidget::setDiameter( int value )
{
    diameter = value;
    repaint();
}


int QvkHaloCircleWidget::getDiameter()
{
    return diameter;
}


void QvkHaloCircleWidget::setOpacity( double value )
{
    opacity = value;;
    repaint();
}


double QvkHaloCircleWidget::getOpacity()
{
    return opacity;
}
