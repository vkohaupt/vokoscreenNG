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

#include "QvkWatcherPlug.h"
#include "global.h"

#include <QDebug>

/*
 * QvkWatcherPlug monitoring only new or removed Audiodevices.
 * It has the same behavior as under Linux.
 */
QvkWatcherPlug::QvkWatcherPlug( Ui_formMainWindow *ui_mainwindow )
{
    global::lineEditAudioPlug = new QLineEdit;
    ui = ui_mainwindow;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 3000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_update() ) );
}


QvkWatcherPlug::~QvkWatcherPlug()
{}


void QvkWatcherPlug::start_monitor()
{
    timer->start();
}


void QvkWatcherPlug::slot_update()
{

}
