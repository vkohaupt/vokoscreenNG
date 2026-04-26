/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
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

#include "global.h"
#include "QvkCameraWindow_wl.h"

#include <QDebug>
#include <QIcon>
#include <QSize>
#include <QPaintEvent>

QvkCameraWindow_wl::QvkCameraWindow_wl()
{
    setWindowTitle(QString(tr("Camera")));

    QIcon icon;
    icon.addFile(QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMouseTracking(true);
}

QvkCameraWindow_wl::~QvkCameraWindow_wl()
{
}


void QvkCameraWindow_wl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
}
