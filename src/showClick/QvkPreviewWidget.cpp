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

#include "QvkPreviewWidget.h"

QvkPreviewWidget::QvkPreviewWidget( QWidget *parent ) : QWidget( parent )
{
    parentWidget = parent;

    color = Qt::red;
    diameter = 50;
    opacity = 0.7;
}


QvkPreviewWidget::~QvkPreviewWidget()
{
}


void QvkPreviewWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPainter painter;
    painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );


    qreal penWith = 6;
    QPen pen;
    pen.setWidthF( penWith );
    pen.setColor( color );
    pen.setStyle( Qt::SolidLine );
    painter.setPen( pen );
    painter.setBrush( Qt::NoBrush );
    painter.setOpacity( opacity );
    resize( parentWidget->width(), parentWidget->height());
    painter.drawEllipse( parentWidget->width()/2-diameter/2, parentWidget->height()/2-diameter/2, diameter, diameter );

    pen.setWidthF( 3.0 );
    painter.setPen( pen );
    QRectF rectF( width()/2 - diameter/2 + 1.5*penWith, height()/2 - diameter/2 + 1.5*penWith, diameter-3*penWith, diameter-3*penWith );
    int startAngle = 90 * 16;
    int spanAngle = 180 * 16;
    painter.drawArc( rectF, startAngle, spanAngle );

    painter.end();
}


void QvkPreviewWidget::setColor( QColor value )
{
    color = value;
    repaint();
}


QColor QvkPreviewWidget::getColor()
{
    return color;
}


void QvkPreviewWidget::setDiameter( int value )
{
    diameter = value;
    repaint();
}


int QvkPreviewWidget::getDiameter()
{
    return diameter;
}


void QvkPreviewWidget::setOpacity( double value )
{
    opacity = value;;
    repaint();
}


double QvkPreviewWidget::getOpacity()
{
    return opacity;
}
