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
    setMouseTracking( true );

    showMaximized();

    timer = new QTimer();
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( timerInterval );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_durationButton() ) );
}


void QvkShowMessage_wl::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

    QPixmap pixmap( width(), height() );
    pixmap.fill( Qt::transparent );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHint( QPainter::Antialiasing, true );
    painterPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );

    // Begin Pixmap window. Hier wird alles gezeichnet und zum Schluß ins painterPixmap übertragen
    drawWindowWidth = 300;
    drawWindowHeight = 180;
    QPixmap windowPixmap( drawWindowWidth, drawWindowHeight );
    QPainter painterWindowPixmap;
    painterWindowPixmap.begin( &windowPixmap );
    painterWindowPixmap.setRenderHint( QPainter::Antialiasing, true );
    painterWindowPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );
    QPen pen;
    QBrush brush;
    brush.setColor( Qt::white );
    brush.setStyle( Qt::SolidPattern );
    pen.setWidth( 0 );
    pen.setColor( Qt::darkGray );
    painterWindowPixmap.setBrush( brush );
    painterWindowPixmap.setPen( pen );
    painterWindowPixmap.drawRect( 0, 0, drawWindowWidth, drawWindowHeight );

    // Titelzeile
    brush.setColor( Qt::lightGray );
    brush.setStyle( Qt::SolidPattern );
    int titelLineHeight = 24;
    painterWindowPixmap.fillRect( 0, 0, drawWindowWidth, titelLineHeight, brush );
    QPixmap logoPixmap( ":/pictures/logo/logo.png" );
    logoPixmap = logoPixmap.scaled( 22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterWindowPixmap.drawPixmap( 1, 1, logoPixmap );
    painterWindowPixmap.setPen( Qt::black );
    painterWindowPixmap.drawText( 1+30, 16, "Snapshot" );

    // CloseButton in Titelzeile
    QColor color;
    if ( isOverCloseButton == true ) {
        color = Qt::red;
    } else {
        color = Qt::white;
    }
    QPixmap pixmapCloseButton( titelLineHeight, titelLineHeight );
    pixmapCloseButton.fill( Qt::transparent );
    QPainter painterCloseButton;
    painterCloseButton.begin( &pixmapCloseButton );
    painterCloseButton.setRenderHint( QPainter::Antialiasing, true );
    painterCloseButton.setRenderHint( QPainter::SmoothPixmapTransform, true );
    pen.setColor( color );
    pen.setWidth( 2 );
    painterCloseButton.setPen( pen );
    painterCloseButton.translate( 12, 12 );
    painterCloseButton.rotate( 45 );
    painterCloseButton.drawLine( -6,  0, 6, 0 ); // Horizontal
    painterCloseButton.drawLine(  0, -6, 0, 6 ); // Vertikal
    painterWindowPixmap.drawPixmap( drawWindowWidth-titelLineHeight, 0, pixmapCloseButton );
    painterCloseButton.end();
    QPixmap statusPixmap( statusIcon );
    statusPixmap = statusPixmap.scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterWindowPixmap.drawPixmap( 10, (drawWindowHeight-titelLineHeight)/2 + titelLineHeight - 64/2, statusPixmap );

    QPixmap imagePixmap( image );
    imagePixmap = imagePixmap.scaled( 350, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterWindowPixmap.drawPixmap( 100, 70, imagePixmap );

    painterWindowPixmap.drawPixmap( drawWindowWidth-20, 30, pixmapDuration );

    painterWindowPixmap.end();
    // End Pixmap window.

    // Nun wird das fertige Fenster übertragen
    painterPixmap.drawPixmap( width()-drawWindowWidth-margin, height()-drawWindowHeight-margin, windowPixmap );
    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}


void QvkShowMessage_wl::mouseMoveEvent( QMouseEvent *event )
{
    if ( QRect( width()-margin-20, height()-margin-drawWindowHeight, 20, 20 ).contains( event->position().toPoint() ) == true ) {
        isOverCloseButton = true;
    } else {
        isOverCloseButton = false;
    }
}


void QvkShowMessage_wl::leaveEvent( QEvent *event )
{
    Q_UNUSED(event)
    isOverCloseButton = false;
}


void QvkShowMessage_wl::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    if ( isOverCloseButton == true ) {
        timer->stop();
        close();
    }
}


void QvkShowMessage_wl::showMessage( QString text )
{
    Q_UNUSED(text)
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
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
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
        timer->stop();
        close();
    }
}
