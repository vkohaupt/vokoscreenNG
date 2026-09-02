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
#include <QScreen>
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QRect>
#include <QRectF>
#include <QFont>

QvkShowMessage_wl::QvkShowMessage_wl()
{
    setAttribute( Qt::WA_TranslucentBackground, true );
    setWindowFlags( Qt::FramelessWindowHint );
    setMouseTracking( true );

    showMaximized();

    timer = new QTimer();
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( timerInterval );
    connect(timer, &QTimer::timeout, this, [=](){slot_durationButton();});
    degreeStep = 360 / timeOut * timerInterval;
    timer->start();
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
    drawWindowHeight = 130 + titelLineHeight ; // Inhalt Fenster + titelLineHeight
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
    painterWindowPixmap.fillRect( 0, 0, drawWindowWidth, titelLineHeight, brush );
    QPixmap logoPixmap( ":/pictures/logo/logo.png" );
    logoPixmap = logoPixmap.scaled( 22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterWindowPixmap.drawPixmap( 1, 1, logoPixmap );
    painterWindowPixmap.setPen( Qt::black );
    painterWindowPixmap.drawText( 1+30, 16, windowTitle );

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

    // Url für Ordner Bilder
    QPixmap pixmapUrl( drawWindowWidth - 60, 20 );
    pixmapUrl.fill( Qt::transparent );
    pixmapUrlSize = pixmapUrl.size();
    QPainter painterUrl;
    painterUrl.begin( &pixmapUrl );
    {
        int fontSize = 11;
        QFont font;
        font.setPointSize( fontSize );
        font.setUnderline( true );
        painterUrl.setFont( font );
        painterUrl.setPen( Qt::blue );
        QString folder = tr( "Folder" );
        QFontMetrics fontMetrics( font );
        int textWidth = fontMetrics.horizontalAdvance( folder );
        painterUrl.drawText( ( pixmapUrlSize.width()-textWidth )/2, 16, folder );
    }
    painterUrl.end();
    painterWindowPixmap.drawPixmap( 30, 30, pixmapUrl );

    // Text
    int widthText = 200;
    int heightText = 80;
    QPixmap pixmapText(widthText, heightText);
    pixmapText.fill(Qt::transparent);
    QPainter painterText;
    {
        painterText.begin(&pixmapText);
        int fontSize = 11;
        QFont font;
        font.setPointSize(fontSize);
        painterText.setFont(font);
        painterText.setPen(Qt::black);
        painterText.drawText(pixmapText.rect(), Qt::AlignCenter, text);
    }
    painterText.end();
    painterWindowPixmap.drawPixmap(80, 60, pixmapText);


    QPixmap statusPixmap( statusIcon );
    int statusPixmapSize = 48;
    statusPixmap = statusPixmap.scaled( statusPixmapSize, statusPixmapSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    painterWindowPixmap.drawPixmap( 20, (drawWindowHeight-titelLineHeight)/2 + titelLineHeight - statusPixmapSize/2, statusPixmap );

    QPixmap imagePixmap(image);
    if (imagePixmap.isNull() == false){
        imagePixmap = imagePixmap.scaled(300, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painterWindowPixmap.drawPixmap(100, (drawWindowHeight-titelLineHeight)/2 + titelLineHeight - statusPixmapSize/2, imagePixmap);
    }

    painterWindowPixmap.drawPixmap( drawWindowWidth-pixmapDuration.size().width()-6, titelLineHeight+6, pixmapDuration );

    painterWindowPixmap.end();
    // End Pixmap window.

    // Nun wird das fertige Fenster übertragen
    painterPixmap.drawPixmap( width()-drawWindowWidth-marginScreenEdge, height()-drawWindowHeight-marginScreenEdge, windowPixmap );
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
    // Closebutton
    if ( QRect( width()-marginScreenEdge-20, height()-marginScreenEdge-drawWindowHeight, 20, 20 ).contains( event->position().toPoint() ) == true ) {
        isOverCloseButton = true;
    } else {
        isOverCloseButton = false;
    }

    // Path to Folder
    int m_x = width()-marginScreenEdge-drawWindowWidth+(drawWindowWidth-pixmapUrlSize.width())/2;
    int m_y = height()-marginScreenEdge-drawWindowHeight+titelLineHeight+6;
    int m_with = pixmapUrlSize.width();
    int m_height = pixmapUrlSize.height();
    if ( QRect( m_x, m_y, m_with, m_height ).contains( event->position().toPoint() ) == true ) {
        isOverUrl = true;
        QCursor cursor( Qt::PointingHandCursor );
        setCursor( cursor );
    } else {
        isOverUrl = false;
        unsetCursor();
    }
}


void QvkShowMessage_wl::leaveEvent( QEvent *event )
{
    Q_UNUSED(event)
    isOverCloseButton = false;
    isOverUrl = false;
}


void QvkShowMessage_wl::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    if ( isOverCloseButton == true ) {
        timer->stop();
        close();
    }

    if ( isOverUrl == true ) {
        const QString path = "file:///" + folderPath;
        QDesktopServices::openUrl( QUrl( path, QUrl::TolerantMode) );
    }
}


void QvkShowMessage_wl::set_folderPath(QString path)
{
    folderPath = path;
}


void QvkShowMessage_wl::set_text(QString m_text)
{
    text = m_text;
}


void QvkShowMessage_wl::set_StatusIcon( QString m_statusIcon )
{
    statusIcon = m_statusIcon;
}


void QvkShowMessage_wl::set_Image( QString m_image )
{
    image = m_image;
}


void QvkShowMessage_wl::set_WindowTitle( QString title )
{
    windowTitle = title;
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
