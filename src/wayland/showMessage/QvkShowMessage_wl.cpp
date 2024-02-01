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

#include "QvkShowMessage_wl.h"

#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QDebug>
#include <QMouseEvent>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QPixmap>

QvkShowMessage_wl::QvkShowMessage_wl()
{
    setAttribute( Qt::WA_TranslucentBackground, true );
    setWindowFlags( Qt::FramelessWindowHint );

    // drawWindowWidth und drawWindowHeight sind die zu zeichnenden größen des Fenster
    drawWindowWidth = 300;
    drawWindowHeight = 130;

    showMaximized();
}


void QvkShowMessage_wl::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

    QPixmap pixmap( width() * devicePixelRatioF(), height() * devicePixelRatioF() );
    pixmap.fill( Qt::transparent );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHints( QPainter::Antialiasing, true );
    painterPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );

    QPen pen;
    QBrush brush;
    brush.setColor( Qt::white );
    brush.setStyle( Qt::SolidPattern );
    pen.setWidth( 0 );
    pen.setColor( Qt::darkGray );
    painterPixmap.setBrush( brush );
    painterPixmap.setPen( pen );
    painterPixmap.drawRect( width()-drawWindowWidth, height()-drawWindowHeight, drawWindowWidth, drawWindowHeight );

    QPixmap statusPixmap( statusIcon );
    painterPixmap.drawPixmap( width()-drawWindowWidth+10, height()-drawWindowHeight+64/2, 64, 64, statusPixmap );

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkShowMessage_wl::set_StatusIcon( QString m_statusIcon )
{
    statusIcon = m_statusIcon;
}
