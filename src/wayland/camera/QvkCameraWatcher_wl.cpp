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
}


QvkCameraWatcher_wl::~QvkCameraWatcher_wl()
{
}

void QvkCameraWatcher_wl::init()
{
    // object_id + ":::" + camera_name + ":::" + "added" or removed

    QMediaDevices *mediaDevices = new QMediaDevices;
    connect( mediaDevices, &QMediaDevices::videoInputsChanged, this, [=](){

        QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

        // Camera wurde hinzugefügt
        if(cameras.count() > stringListDevices.count()){
            for(int i = 0; i < cameras.count(); i++){
                QString id = cameras.at(i).id();
                QString description = cameras.at(i).description();
                QString id_and_description = id + ":::" + description;
                if(!stringListDevices.contains(id_and_description)){
                    stringListDevices.append(id_and_description);
                    qDebug().noquote() << global::nameOutput << "[Camera] Added:" << description << "Device:" << id;
                    emit signal_cameraChanged( id + ":::" + description + ":::" + "added" );
                }
            }
            return;
        }

        // Camera wurde entfernt
        // ID und description werden von devices in die StringListen tempID und tmpDescription transferiert
        //
        QStringList tmp;
        for(int i = 0; i < cameras.count(); i++){
            tmp.append(cameras.at(i).id() + ":::" + cameras.at(i).description());
        }

        int index;
        if(tmp.count() < stringListDevices.count()){
            for(int i = 0; i < stringListDevices.count(); i++){
                QString id_and_description = stringListDevices.at(i);
                if(!tmp.contains(id_and_description)){
                    // id_and_description wird nicht gefunden. In dem Fall ist es das gesuchte Gerät.
                    QString id = id_and_description.section(":::", 0, 0);
                    QString description = id_and_description.section(":::", 1, 1);
                    qDebug().noquote() << global::nameOutput << "[Camera] Removed:" << description << "Device:" << id;
                    emit signal_cameraChanged( id + ":::" + description + ":::" + "removed" );
                    index = i;
                    break;
                }
            }
            if (!stringListDevices.empty()){
                stringListDevices.removeAt(index);
            }
        }

        qDebug().noquote();
    });

    emit mediaDevices->videoInputsChanged();
}
