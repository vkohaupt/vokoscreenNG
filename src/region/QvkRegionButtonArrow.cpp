/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
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

#include "QvkRegionButtonArrow.h"

#include <QPainterPath>

QvkRegionButtonArrow::QvkRegionButtonArrow()
{
}


QvkRegionButtonArrow::~QvkRegionButtonArrow()
{
}


QPixmap QvkRegionButtonArrow::getButton( QColor color )
{
    QPixmap pixmap( diameter+penWidth, diameter+penWidth );
    pixmap.fill( Qt::transparent );

    QPainter painter;
    painter.begin( &pixmap );
      painter.setRenderHints( QPainter::Antialiasing, true );
      QBrush brush;
        brush.setColor( color );
        brush.setStyle( Qt::SolidPattern );
      painter.setBrush( brush );
      QPen pen;
        pen.setColor( Qt::black );
        pen.setWidth( penWidth );
      painter.setPen( pen );
      painter.drawEllipse( penWidthHalf,
                           penWidthHalf,
                           diameter,
                           diameter );
    painter.end();

    return pixmap;
}


QPixmap QvkRegionButtonArrow::getArrow( degreeArrow degree, QColor colorSelected )
{
    QPixmap pixmap( diameter+penWidth, diameter+penWidth );
    pixmap.fill( Qt::transparent );

    QPainter painter;
    painter.begin( &pixmap );
        painter.setRenderHints( QPainter::Antialiasing, true );
        painter.translate((diameter+penWidth)/2, (diameter+penWidth)/2);
        painter.rotate( degree );
        QPen pen;
            pen.setCapStyle( Qt::RoundCap );
            pen.setColor( colorSelected );
            pen.setWidthF( penWidth );
            pen.setJoinStyle( Qt::RoundJoin );
        painter.setPen( pen );
        QBrush brush;
            brush.setColor( colorSelected );
            brush.setStyle( Qt::SolidPattern );
        painter.setBrush( brush );
        QPainterPath painterPath;
            painterPath.moveTo(  0, 0 );
            painterPath.lineTo(  0, -radius + penWidth );
            painterPath.lineTo( -3, -radius + penWidth + 7 );
            painterPath.lineTo(  3, -radius + penWidth + 7 );
            painterPath.lineTo(  0, -radius + penWidth );
        painter.drawPath( painterPath );
    painter.end();

    return pixmap;
}


QPixmap QvkRegionButtonArrow::getPixmapHandle( QColor color, QColor colorSelected ,degreeArrow degree )
{
    QPixmap pixmap( diameter+penWidth, diameter+penWidth );
    pixmap.fill( Qt::transparent );

    QPainter painter;
    painter.begin( &pixmap );
      painter.setRenderHints( QPainter::Antialiasing, true );
      painter.drawPixmap( 0, 0, getButton( color ) );
      painter.drawPixmap( 0, 0, getArrow( degree , colorSelected ) );
    painter.end();

    return pixmap;
}


int QvkRegionButtonArrow::getWithHalf()
{
   return ( diameter + penWidth ) / 2;
}
