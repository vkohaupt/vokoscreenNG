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

#include "QvkCameraWatcherLinux.h"
#include "global.h"

#include <QCameraInfo>

QvkCameraWatcher::QvkCameraWatcher()
{
}


void QvkCameraWatcher::cameraWatcherInit()
{
    cameraCount = 0;
    oldDeviceList.clear();
    QStringList deviceList;
    QString path = "/dev/";

    QFileSystemWatcher * fileSystemWatcher = new QFileSystemWatcher();
    fileSystemWatcher->addPath( path );
    connect( fileSystemWatcher, SIGNAL( directoryChanged( QString ) ), this, SLOT( myfileSystemWatcher( QString ) ) );

    QDir dir( path );
    QStringList filters;
    filters << "video*";
    deviceList = dir.entryList( filters, QDir::System, QDir::Time );
    deviceList.sort();

    oldDeviceList = deviceList;
    cameraCount = deviceList.count();

    for ( int x = 0; x < deviceList.count(); x++ )
    {
        QCameraInfo cameraInfo( QString( path + deviceList.at(x) ).toLatin1() );
        emit signal_addedCamera( cameraInfo.description() , path + deviceList.at(x) );
    }
}


QvkCameraWatcher::~QvkCameraWatcher()
{
}


/*
 * Add or remove plugin or plugout devices
 */
void QvkCameraWatcher::myfileSystemWatcher( QString path )
{
    QDir dir( path );
    QStringList filters;
    filters << "video*";
    QStringList newDeviceList = dir.entryList( filters, QDir::System, QDir::Time );
    newDeviceList.sort();

    // add plugin device
    if ( newDeviceList.count() > cameraCount )
    {
        QString addedDevice;
        for ( int x = 0; x < newDeviceList.count(); x++ )
        {
            if ( not oldDeviceList.contains( newDeviceList.at(x) ) )
            {
                addedDevice = newDeviceList.at(x);
            }
        }

        cameraCount = newDeviceList.count();

        // waiting for data from CameraInfo
        bool b = false;
        while ( b == false )
        {
            QCameraInfo cameraInfo( QString( path + addedDevice ).toLatin1() );
            if ( cameraInfo.isNull() == false )
            {
                b = true;
                emit signal_addedCamera( cameraInfo.description() , path + addedDevice );
            }
        }

        qDebug().noquote() << global::nameOutput << "added device" << addedDevice;
    }

    // remove plugout device
    if ( newDeviceList.count() < cameraCount )
    {
        QString removedDevice;
        for ( int x = 0; x < oldDeviceList.count(); x++ )
        {
            if ( not newDeviceList.contains( oldDeviceList.at(x) ) )
            {
                removedDevice = oldDeviceList.at(x);
            }
        }

        cameraCount = newDeviceList.count();
        emit signal_removedCamera( path + removedDevice );
        qDebug().noquote() << global::nameOutput << "removed device" << removedDevice;
    }

    if ( oldDeviceList.count() != newDeviceList.count() )
    {
        oldDeviceList = newDeviceList;
    }
}
