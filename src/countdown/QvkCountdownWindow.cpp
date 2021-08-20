/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "QvkCountdownWindow.h"

#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif

QvkCountdownWindow::QvkCountdownWindow(  QWidget *parent )
{
    setParent( parent );

#ifdef Q_OS_LINUX
    if ( QX11Info::isCompositingManagerRunning() == true )
    {
        setAttribute( Qt::WA_TranslucentBackground, true );
    } else
    {
        setAttribute( Qt::WA_TranslucentBackground, false );
    }
#endif

#ifdef Q_OS_WIN
    setAttribute( Qt::WA_TranslucentBackground, true );
#endif

    show();
}

QvkCountdownWindow::~QvkCountdownWindow()
{
}


void QvkCountdownWindow::paintEvent( QPaintEvent *event )
{
  (void)event;
  painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );
    QPen pen;
    QBrush brush;
    
    brush.setColor( Qt::darkGray );
    brush.setStyle( Qt::SolidPattern );
    pen.setWidth( 0 );
    pen.setColor( Qt::darkGray );
    painter.setBrush( brush );
    painter.setPen( pen );
    painter.setOpacity( 0.3 );
    painter.drawPie( 0, 0, 300, 300, 90*16, gradValue*16 );

    painter.setOpacity( 1.0 );
    pen.setColor( Qt::darkGray );
    pen.setWidth( 6 );
    painter.setPen( pen );
    brush.setStyle( Qt::NoBrush );
    painter.setBrush( brush );
    painter.drawEllipse( QPoint( width()/2, height()/2), 125-3, 125-3 );
    painter.drawEllipse( QPoint( width()/2, height()/2), 100, 100 );
    painter.drawLine( 0, height()/2, width(), height()/2 );
    painter.drawLine( width()/2, 0, width()/2, height() );
    
    int fontSize = 110;
    QFont font;
    font.setPointSize( fontSize );
    painter.setFont( font );
    painter.setPen( Qt::red );
    QFontMetrics fontMetrics( font );
    int fontWidth = fontMetrics.width( QString::number( countValue ) );
    painter.drawText( width()/2-fontWidth/2, height()/2+fontSize/2, QString::number( countValue ) );

//--------------------- Cancel button -------------------------------

    fontSize = 14;
    font.setPointSize( fontSize );
    font.setBold( true);
    QFontMetrics fontMetrics_1( font );
    fontWidth = fontMetrics_1.width( cancelText );

    qreal x = width()/2 - (fontWidth+30)/2;
    qreal y = 220;
    qreal width = fontWidth + 30;
    qreal height = 30;

    brush.setColor( Qt::red );
    brush.setStyle( Qt::SolidPattern );
    painter.setBrush( brush );
    pen.setWidth( 2 );
    pen.setColor( Qt::black );
    painter.setPen( pen );
    rectCancel.setRect( x, y, width, height );
    painter.drawRoundedRect( rectCancel, 10, 10 );

    painter.setPen( Qt::white );
    painter.setFont( font );
    painter.drawText( rectCancel, Qt::AlignCenter, cancelText );

  painter.end();
}

/*
void QvkCountdownWindow::mousePressEvent( QMouseEvent *event )
{
    if ( rectCancel.contains( event->pos() ) )
    {
        hide();
        timer->stop();
        animationTimer->stop();
        emit signal_countDownCancel( true );
    }
}
*/
