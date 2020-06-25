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

#include "QvkDirectSoundWatcher.h"
#include "global.h"

#include <QDebug>
#include <QAudioDeviceInfo>

/*
 * QvkWatcherPlug monitoring only new or removed Audiodevices.
 * It has the same behavior as under Linux.
 */
QvkDirectSoundWatcher::QvkDirectSoundWatcher( Ui_formMainWindow *ui_mainwindow )
{
    global::lineEditAudioPlug = new QLineEdit;
    ui = ui_mainwindow;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 3000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_update() ) );
}


QvkDirectSoundWatcher::~QvkDirectSoundWatcher()
{}


void QvkDirectSoundWatcher::start_monitor()
{
    timer->start();
}


void QvkDirectSoundWatcher::slot_update()
{
    QList<QCheckBox *> listCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    QList<QAudioDeviceInfo> listAudioDeviceInfo = QAudioDeviceInfo::availableDevices( QAudio::AudioInput );

    QStringList stringListCheckBox;
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        stringListCheckBox.append( listCheckBox.at(i)->accessibleName() );
    }

    QStringList stringListAudioDeviceInfo;
    for ( int i = 0; i < listAudioDeviceInfo.count(); i++ )
    {
        stringListAudioDeviceInfo.append( listAudioDeviceInfo.at(i).deviceName() );
    }

    // Add new Device
    if ( listAudioDeviceInfo.count() > listCheckBox.count() )
    {
        for ( int i = 0; i < stringListAudioDeviceInfo.count(); i++ )
        {
            if ( stringListCheckBox.contains( stringListAudioDeviceInfo.at(i) ) == false )
            {
                QString name = listAudioDeviceInfo.at(i).deviceName();
                QString device = listAudioDeviceInfo.at(i).deviceName();
                qDebug().noquote() << global::nameOutput << "[Audio] Added:" << name << "Device:" << device;
                QString audioDevicePlug = "";
                audioDevicePlug.append( "[Audio-device-added]" );
                audioDevicePlug.append( ":");
                audioDevicePlug.append( name );
                audioDevicePlug.append( ":");
                audioDevicePlug.append( device );
                global::lineEditAudioPlug->setText( audioDevicePlug );
            }
        }
    }

    // Remove device
    if ( listAudioDeviceInfo.count() < listCheckBox.count() )
    {
        for ( int i = 0; i < stringListCheckBox.count(); i++ )
        {
            if ( stringListAudioDeviceInfo.contains( stringListCheckBox.at(i) ) == false )
            {
                QString name = stringListCheckBox.at(i);
                QString device = stringListCheckBox.at(i);
                qDebug().noquote() << global::nameOutput << "[Audio] Removed:" << name << "Device:" << device;
                QString audioDevicePlug = "";
                audioDevicePlug.append( "[Audio-device-removed]" );
                audioDevicePlug.append( ":");
                audioDevicePlug.append( name );
                audioDevicePlug.append( ":");
                audioDevicePlug.append( device );
                global::lineEditAudioPlug->setText( audioDevicePlug );
            }
        }
    }
}
