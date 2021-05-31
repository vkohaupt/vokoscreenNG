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

#include "QvkPushButton.h"
#include <QPainter>
#include <QDebug>
#include <QBitmap>


QvkPushButton::QvkPushButton( QColor valueColor)
{
    color = valueColor;
}


QvkPushButton::~QvkPushButton()
{
}


void QvkPushButton::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    qreal penWidthFrame = 2;

    if ( mouseHover == false )
    {
        QPainter painter;
        painter.begin( this );
        painter.setRenderHints( QPainter::Antialiasing, true );

        // Area
        painter.fillRect( 0, 0, width(), height(), color );

        // Frame
        QPen penFrame;
        penFrame.setWidthF( 1.0 );
        penFrame.setColor( Qt::lightGray );
        penFrame.setStyle( Qt::SolidLine );
        penFrame.setCapStyle( Qt::FlatCap );
        penFrame.setJoinStyle( Qt::MiterJoin );
        painter.setPen( penFrame );
        painter.setBrush( Qt::NoBrush );
        painter.drawRect( penWidthFrame/2,
                          penWidthFrame/2,
                          width() - penWidthFrame,
                          height() - penWidthFrame );

        painter.end();
    }

    if ( mouseHover == true )
    {
        QPainter painter;
        painter.begin( this );
        painter.setRenderHints( QPainter::Antialiasing, true );

        // Area
        QBrush brush( color );
        painter.setBrush( brush );
        painter.setPen( Qt::NoPen );
        painter.drawRect( 0, 0, width(), height() );

        // Frame
        QPen penFrame;
        penFrame.setWidthF( penWidthFrame );
        penFrame.setColor( Qt::cyan );
        penFrame.setStyle( Qt::SolidLine );
        penFrame.setCapStyle( Qt::FlatCap );
        penFrame.setJoinStyle( Qt::MiterJoin );
        painter.setPen( penFrame );
        painter.drawRect( penWidthFrame/2,
                          penWidthFrame/2,
                          width() - penWidthFrame,
                          height() - penWidthFrame );

        painter.end();

    }

}


void QvkPushButton::enterEvent( QEvent *event )
{
    Q_UNUSED(event);
    mouseHover = true;
}


void QvkPushButton::leaveEvent( QEvent *event )
{
    Q_UNUSED(event);
    mouseHover = false;
}
