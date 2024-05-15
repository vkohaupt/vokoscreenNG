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

#include "QvkWASAPIWatcher.h"
#include "QvkWASAPIGstreamer.h"
#include "global.h"

#include <QDebug>
#include <QCheckBox>
#include <QStringList>

QvkWASAPIWatcher::QvkWASAPIWatcher( Ui_formMainWindow *ui_mainwindow )
{
    lineEditWASAPIAudioPlug = new QLineEdit;

    ui = ui_mainwindow;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 3000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_update() ) );
}

QvkWASAPIWatcher::~QvkWASAPIWatcher()
{}


void QvkWASAPIWatcher::slot_update()
{
    QvkWASAPIGstreamer vkWASAPIGstreamer;
    QStringList listDevices;
    listDevices << vkWASAPIGstreamer.get_all_Audio_Source_devices();
    listDevices << vkWASAPIGstreamer.get_all_Audio_Playback_devices();

    QList<QCheckBox *> listCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    QStringList stringListCheckBox;
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listCheckBox.at(i);
        stringListCheckBox << checkBox->accessibleName();
    }

    // Add new Device
    if ( listDevices.count() > listCheckBox.count() ) {
        for ( int i = 0; i < listDevices.count(); i++ ) {
            QString device = QString( listDevices.at(i) ).section( ":::", 0, 0 )
                           + ":::"
                           + QString( listDevices.at(i) ).section( ":::", 2, 2 ) ;
            if ( stringListCheckBox.contains( device ) == false ) {
                QString name = QString( listDevices.at(i) ).section( ":::", 1, 1 );
                qDebug().noquote() << global::nameOutput << "[Audio WASAPI] Added:" << name << "Device:" << device;
                QString audioDevicePlug = "";
                audioDevicePlug.append( "[Audio-device-added]" );
                audioDevicePlug.append( "---");
                audioDevicePlug.append( name );
                audioDevicePlug.append( "---");
                audioDevicePlug.append( device );
                lineEditWASAPIAudioPlug->setText( audioDevicePlug );
            }
        }        
    }


    // listDevices   --> Device ::: Name ::: Typ
    // listDeviceTyp --> Device ::: Typ
    // listCheckbox  --> Device ::: Typ

    QStringList listDeviceTyp;
    for ( int i = 0; i < listDevices.count(); i++ ) {
        listDeviceTyp << QString( listDevices.at(i) ).section( ":::", 0, 0 )
                       + ":::"
                       + QString( listDevices.at(i) ).section( ":::", 2, 2 );
    }

    // Remove device
    if ( listDevices.count() < listCheckBox.count() ) {
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            QString string = QString( checkBox->accessibleName() );
            if ( listDeviceTyp.contains( string ) == false ) {
                QString name = checkBox->text();
                QString device = checkBox->accessibleName();
                qDebug().noquote() << global::nameOutput << "[Audio WASAPI] Removed:" << name << "Device:" << device;
                qDebug().noquote();
                QString audioDevicePlug = "";
                audioDevicePlug.append( "[Audio-device-removed]" );
                audioDevicePlug.append( "---");
                audioDevicePlug.append( name );
                audioDevicePlug.append( "---");
                audioDevicePlug.append( device );
                lineEditWASAPIAudioPlug->setText( audioDevicePlug );
            }
        }
    }
}
