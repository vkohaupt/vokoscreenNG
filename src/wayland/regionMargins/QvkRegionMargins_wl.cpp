/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2025 Volker Kohaupt
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

#include "QvkRegionMargins_wl.h"

/*
 * QvkRegionMargins_wl determines four margins: left, top, right, and bottom,
 * which describe the size of the borders around a window on the desktop.
 */

QvkRegionMargins_wl::QvkRegionMargins_wl( QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *ui_mainwindow )
{
    mainWindow = vkMainWindow;
    ui = ui_mainwindow;
}


QvkRegionMargins_wl::~QvkRegionMargins_wl()
{
}


void QvkRegionMargins_wl::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

}


void QvkRegionMargins_wl::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event)
    screenWidth = width();
    screenHeight = height();
}
