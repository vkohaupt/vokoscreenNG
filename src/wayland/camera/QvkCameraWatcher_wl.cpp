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
#include <QStringList>

QvkCameraWatcher_wl::QvkCameraWatcher_wl()
{
    QMediaDevices *mediaDevices = new QMediaDevices;
    connect( mediaDevices, &QMediaDevices::videoInputsChanged, this, [=](){

        QList<QCameraDevice> devices = QMediaDevices::videoInputs();

        // Camera wurde hinzugefügt
        if(devices.count() > stringListDevices.count()){
            for(int i = 0; i < devices.count(); i++){
                QString id = devices.at(i).id();
                QString description = devices.at(i).description();
                if(!stringListDevices.contains(id)){
                    stringListDevices.append(id);
                    qDebug().noquote() << global::nameOutput << "[Camera] Added:" << description << "Device:" << id;
                }
            }
        }

        if(devices.count() < stringListDevices.count() ){
            qDebug().noquote() << global::nameOutput << "[Camera] Removed:";// << description << "Device:" << id;
        }

        qDebug().noquote();
    });
    emit mediaDevices->videoInputsChanged();

    // object_id + ":::" + camera_name + ":::" + "added" or removed
}


QvkCameraWatcher_wl::~QvkCameraWatcher_wl()
{
}
