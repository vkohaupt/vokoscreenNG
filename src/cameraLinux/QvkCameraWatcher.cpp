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

#include "QvkCameraWatcher.h"
#include "global.h"

#include <QDebug>
#include <QCameraInfo>

/*
 * QvkWatcherPlug monitoring only new or removed Audiodevices.
 */

QvkCameraWatcher::QvkCameraWatcher()
{
}


QvkCameraWatcher::~QvkCameraWatcher()
{
}


QvkCameraWatcher::QvkCameraWatcher( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 3000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_update() ) );
    timer->start();
}


void QvkCameraWatcher::slot_update()
{
    QList<QCameraInfo> camerasInfoList = QCameraInfo::availableCameras();

    // Add new Device
    if ( camerasInfoList.count() > ui->comboBoxCamera->count() )
    {
        for ( int i = 0; i < camerasInfoList.count(); i++ )
        {
            if ( ui->comboBoxCamera->findData( camerasInfoList.at(i).deviceName() ) == -1 )
            {
                if ( ( camerasInfoList.at(i).description() > "" ) and ( !camerasInfoList.at(i).description().contains( "@device:pnp" ) ) )
                {
                    qDebug().noquote() << global::nameOutput << "[Camera] Added:" << camerasInfoList.at(i).description() << "Device:" << camerasInfoList.at(i).deviceName();
                    emit signal_addedCamera( camerasInfoList.at(i).description(), camerasInfoList.at(i).deviceName() );
                }
            }
        }
        return;
    }

    QStringList cameraInfoStringList;
    for ( int i = 0; i < camerasInfoList.count(); i++ )
    {
        cameraInfoStringList << camerasInfoList.at(i).deviceName();
    }

    int cameraCountCombobox = ui->comboBoxCamera->count();

    // Remove device
    if ( camerasInfoList.count() < cameraCountCombobox )
    {
        for ( int i = 1; i <= cameraCountCombobox; i++ )
        {
            if ( cameraInfoStringList.contains( QString( ui->comboBoxCamera->itemData(i-1).toString() ) ) == false )
            {
                qDebug().noquote() << global::nameOutput << "[Camera] Removed:" << ui->comboBoxCamera->itemText(i-1) << "Device:" << ui->comboBoxCamera->itemData(i-1).toString();
                emit signal_removedCamera( ui->comboBoxCamera->itemData(i-1).toString() );
                break;
            }
        }
    }
}
