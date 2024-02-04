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
    drawWindowHeight = 180;

    showMaximized();

    timer = new QTimer();
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( timerInterval );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_durationButton() ) );
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

    // Titelzeile
    brush.setColor( Qt::lightGray );
    brush.setStyle( Qt::SolidPattern );
    int titelLineHeight = 24;
    painterPixmap.fillRect( width()-drawWindowWidth, height()-drawWindowHeight, drawWindowWidth, titelLineHeight, brush );
    QPixmap logoPixmap( ":/pictures/logo/logo.png" );
    logoPixmap = logoPixmap.scaled( 22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterPixmap.drawPixmap( width()-drawWindowWidth+1, height()-drawWindowHeight+1, logoPixmap );
    painterPixmap.setPen( Qt::black );
    painterPixmap.drawText( width()-drawWindowWidth+1+30, height()-drawWindowHeight+16, "Snapshot" );

    QPixmap statusPixmap( statusIcon );
    statusPixmap = statusPixmap.scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterPixmap.drawPixmap( width()-drawWindowWidth+10, height()-(drawWindowHeight-titelLineHeight)/2-64/2, statusPixmap );

    QPixmap imagePixmap( image );
    imagePixmap = imagePixmap.scaled( 350, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterPixmap.drawPixmap( width()-drawWindowWidth+100, height()-drawWindowHeight+70, imagePixmap );

    painterPixmap.drawPixmap( width()-20, height()-drawWindowHeight+25, pixmapDuration );

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkShowMessage_wl::showMessage( QString text )
{
    degreeStep = 360 / timeOut * timerInterval;
    timer->start();
    show();
}


void QvkShowMessage_wl::set_StatusIcon( QString m_statusIcon )
{
    statusIcon = m_statusIcon;
}


void QvkShowMessage_wl::set_Image( QString m_image )
{
    image = m_image;
}


void QvkShowMessage_wl::set_timeOut( qreal value )
{
    timeOut = value;
}


void QvkShowMessage_wl::slot_durationButton()
{
    if ( underMouse() == true ) {
        degree = degreeStep;
    }

    int h = 16;
    QPixmap pixmap( h+2, h+2 );
    pixmap.fill( Qt::transparent );

    QPainter painter;
    painter.begin( &pixmap );
    painter.setRenderHints( QPainter::Antialiasing, true );
    painter.setOpacity( 1.0 );

    QPen pen;
    pen.setColor( Qt::black );
    pen.setWidth( 1 );
    painter.setPen( pen );
    painter.drawEllipse( QRectF( 1, 1, h, h ) );

    pen.setColor( Qt::black );
    pen.setWidth( 1 );
    painter.setPen( pen );
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QString( "#3daee9" ) );
    painter.setBrush( brush );
    degree = degree - degreeStep;
    painter.drawPie( 1, 1, h, h, 90*16, degree*16 );
    painter.end();

    pixmapDuration = pixmap;
    repaint();

    if ( degree <= -360 ) {
        close();
    }
}
