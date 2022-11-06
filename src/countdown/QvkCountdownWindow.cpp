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

#include "QvkCountdownWindow.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QBitmap>

#ifdef Q_OS_LINUX
#include <QX11Info>
#endif

QvkCountdownWindow::QvkCountdownWindow( QWidget *parent )
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

    // Is needed only for the translated text
    QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Abort, this);
    buttonBox->hide();
    QList<QPushButton *> list = buttonBox->findChildren<QPushButton *>();
    cancelText = list.at(0)->text();
    show();
}

QvkCountdownWindow::~QvkCountdownWindow()
{
}


void QvkCountdownWindow::paintEvent( QPaintEvent *event )
{
  (void)event;

    QPixmap pixmap( 300 * devicePixelRatioF(), 300 * devicePixelRatioF() );
    pixmap.fill( Qt::transparent );
    pixmap.setDevicePixelRatio( devicePixelRatioF() );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHints( QPainter::Antialiasing, true );

    QPen pen;
    QBrush brush;
    brush.setColor( Qt::darkGray );
    brush.setStyle( Qt::SolidPattern );
    pen.setWidth( 0 );
    pen.setColor( Qt::darkGray );
    painterPixmap.setBrush( brush );
    painterPixmap.setPen( pen );
    painterPixmap.setOpacity( 0.3 );
    painterPixmap.drawPie( 0, 0, 300, 300, 90*16, gradValue*16 );

    painterPixmap.setOpacity( 1.0 );
    pen.setColor( Qt::darkGray );
    pen.setWidth( 6 );
    painterPixmap.setPen( pen );
    brush.setStyle( Qt::NoBrush );
    painterPixmap.setBrush( brush );
    painterPixmap.drawEllipse( QPoint( width()/2, height()/2), 125-3, 125-3 );
    painterPixmap.drawEllipse( QPoint( width()/2, height()/2), 100, 100 );
    painterPixmap.drawLine( 0, height()/2, width(), height()/2 );
    painterPixmap.drawLine( width()/2, 0, width()/2, height() );
    
    int fontSize = 110;
    QFont font;
    font.setPointSize( fontSize );
    painterPixmap.setFont( font );
    painterPixmap.setPen( Qt::red );
    QFontMetrics fontMetrics( font );
    int fontWidth = fontMetrics.horizontalAdvance( QString::number( countValue ) );
    painterPixmap.drawText( width()/2-fontWidth/2, height()/2+fontSize/2, QString::number( countValue ) );

    //--------------------- Cancel button -------------------------------

    fontSize = 14;
    font.setPointSize( fontSize );
    font.setBold( true);
    QFontMetrics fontMetrics_1( font );
    fontWidth = fontMetrics_1.horizontalAdvance( cancelText );

    qreal x = width()/2 - (fontWidth+30)/2;
    qreal y = 220;
    qreal width = fontWidth + 30;
    qreal height = 30;

    brush.setColor( Qt::red );
    brush.setStyle( Qt::SolidPattern );
    painterPixmap.setBrush( brush );
    pen.setWidth( 2 );
    pen.setColor( Qt::black );
    painterPixmap.setPen( pen );
    rectCancel.setRect( x, y, width, height );
    painterPixmap.drawRoundedRect( rectCancel, 10, 10 );

    painterPixmap.setPen( Qt::white );
    painterPixmap.setFont( font );
    painterPixmap.drawText( rectCancel, Qt::AlignCenter, cancelText );

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

//    setMask( pixmap.mask() );
}


void QvkCountdownWindow::mousePressEvent( QMouseEvent *event )
{
    if ( rectCancel.contains( event->pos() ) )
    {
        hide();
        emit signal_countDownCancel( true );
    }
}

