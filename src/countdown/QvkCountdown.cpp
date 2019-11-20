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

#include "QvkCountdown.h"
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QIcon>

QvkCountdown::QvkCountdown()
{
    QScreen *screen = QGuiApplication::primaryScreen();

    Width = 300;
    Height = 300;;
    x = ( screen->geometry().width() / 2 ) - ( Width / 2 );
    y = ( screen->geometry().height() / 2 ) -( Height / 2 );

    // Die Optionen Qt::Tool sollte nicht angewendet werden da bei Auswahl eines Fenster der Countdown nicht angezeigt wird.
    // Die Option Qt::ToolTip wäre eine möglichkeit, gefällt mir aber Optisch nicht da ein Rahmen angezeigt wird.
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute( Qt::WA_TranslucentBackground, true );
    setWindowTitle( QString( tr( "Countdown") ) );

    QIcon icon;
    icon.addFile( QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off );
    setWindowIcon( icon );

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_updateTimer() ) );

    animationTimer = new QTimer( this );
    animationTimer->setTimerType( Qt::PreciseTimer );
    connect( animationTimer, SIGNAL( timeout() ), this, SLOT( slot_updateAnimationTimer() ) );

    hide();
}


void QvkCountdown::startCountdown( int value )
{
    setGeometry( x, y, Width, Height );
    show();
    countValue = value;
    gradValue = 0;

    timer->start( 1000 );
    animationTimer->start( 25 );
    emit signal_countdownBegin( true );
}


QvkCountdown::~QvkCountdown()
{
}


void QvkCountdown::slot_updateTimer()
{
  gradValue = 0;
  countValue--;

  if ( countValue == 0 )
  {
    setGeometry( x, y, 1, 1 );
    hide();
    timer->stop();
    animationTimer->stop();
    emit signal_countDownfinish( true );
  }
}

void QvkCountdown::slot_updateAnimationTimer()
{
  gradValue = gradValue - 20;
  update();
}


void QvkCountdown::paintEvent( QPaintEvent *event )
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
  painter.end();
}
