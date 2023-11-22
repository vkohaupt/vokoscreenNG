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

#include "QvkCountdown_wl.h"

#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QIcon>
#include <QDebug>
#include <QMouseEvent>

QvkCountdown_wl::QvkCountdown_wl()
{
}


QvkCountdown_wl::~QvkCountdown_wl()
{
}


void QvkCountdown_wl::init()
{
    createCountdownWindow();

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_updateTimer() ) );

    animationTimer = new QTimer( this );
    animationTimer->setTimerType( Qt::PreciseTimer );
    connect( animationTimer, SIGNAL( timeout() ), this, SLOT( slot_updateAnimationTimer() ) );

    hide();
}


void QvkCountdown_wl::createCountdownWindow()
{
    vkCountdownWindow_wl = new QvkCountdownWindow_wl( this );
    vkCountdownWindow_wl->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );

    connect( vkCountdownWindow_wl, SIGNAL( signal_countDownCancel( bool ) ), this, SLOT( slot_cancel( bool ) ) );

    x = 0;
    y = 0;
    Width = 300;
    Height = 300;;

    vkCountdownWindow_wl->x = 0;
    vkCountdownWindow_wl->y = 0;
    vkCountdownWindow_wl->Width = 300;
    vkCountdownWindow_wl->Height = 300;;
}


void QvkCountdown_wl::startCountdown( int value )
{
    vkCountdownWindow_wl->setGeometry( x, y, Width, Height );
    vkCountdownWindow_wl->show();
    vkCountdownWindow_wl->countValue = value;
    vkCountdownWindow_wl->gradValue = 0;

    timer->start( 1000 );
    animationTimer->start( 40 );
    emit signal_countdownBegin( true );
}


void QvkCountdown_wl::slot_updateTimer()
{
    vkCountdownWindow_wl->gradValue = 0;
    vkCountdownWindow_wl->countValue--;

    if ( vkCountdownWindow_wl->countValue == 0 ) {
        vkCountdownWindow_wl->setGeometry( x, y, 1, 1 );
        vkCountdownWindow_wl->hide();
        timer->stop();
        animationTimer->stop();
        emit signal_countDownfinish( true );
    }
}


void QvkCountdown_wl::slot_updateAnimationTimer()
{
    vkCountdownWindow_wl->gradValue = vkCountdownWindow_wl->gradValue - 20;
    vkCountdownWindow_wl->repaint();
}


void QvkCountdown_wl::slot_cancel( bool value )
{
    timer->stop();
    animationTimer->stop();
    emit signal_countDownCancel( value);
}
