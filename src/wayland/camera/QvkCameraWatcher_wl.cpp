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

#include "QvkCameraWatcher_wl.h"
#include "global.h"

#include <QDebug>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QList>

QvkCameraWatcher_wl::QvkCameraWatcher_wl()
{

    camerasDevices.clear();
    qDebug() << "1111111111111111111111111111111" << camerasDevices.count();

    QMediaDevices *mediaDevices = new QMediaDevices;
    connect( mediaDevices, &QMediaDevices::videoInputsChanged, this, [=](){

        QList<QCameraDevice> devices = QMediaDevices::videoInputs();
        // Camera wurde hinzugefügt
        if(devices.count() > camerasDevices.count() ){
            camerasDevices.clear();
            camerasDevices.append(devices);
            qDebug() << "22222222222222222222222222222" << camerasDevices.count();
        }

    });

    // Trigger a videoInputsChanged
    emit mediaDevices->videoInputsChanged();

    // object_id + ":::" + camera_name + ":::" + "added" or removed

}


QvkCameraWatcher_wl::~QvkCameraWatcher_wl()
{
}


