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

#include "QvkHaloPreviewWidget.h"


QvkHaloPreviewWidget::QvkHaloPreviewWidget()
{
}


QvkHaloPreviewWidget::~QvkHaloPreviewWidget()
{
}


void QvkHaloPreviewWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPainter painter;
    painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );

    qreal penWidth = diameter/2 - holeRadius;
    QPen pen;
    pen.setWidthF( penWidth );
    pen.setColor( color );
    painter.setPen( pen );
    painter.setOpacity( opacity );
    painter.drawEllipse( QPointF( width()/2, height()/2 ), holeRadius + penWidth/2, holeRadius + penWidth/2);

    painter.end();
}


void QvkHaloPreviewWidget::setColor( QColor value )
{
    color = value;
    repaint();
}


QColor QvkHaloPreviewWidget::getColor()
{
    return color;
}


void QvkHaloPreviewWidget::setDiameter( int value )
{
    diameter = value;
    repaint();
}


int QvkHaloPreviewWidget::getDiameter()
{
    return diameter;
}


void QvkHaloPreviewWidget::setOpacity( qreal value )
{
    opacity = value;
    repaint();
}


qreal QvkHaloPreviewWidget::getOpacity()
{
    return opacity;
}


void QvkHaloPreviewWidget::setHole( int value )
{
    holeRadius = value;
}

