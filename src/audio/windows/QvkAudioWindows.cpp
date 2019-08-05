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

#include "QvkAudioWindows.h"
#include "global.h"

#include <QAudioDeviceInfo>
#include <QDebug>

QvkAudioWindows::QvkAudioWindows( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 3000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_update() ) );
}


QvkAudioWindows::~QvkAudioWindows()
{
}


void QvkAudioWindows::init()
{
    getAllDevices();
}


void QvkAudioWindows::getAllDevices()
{
    QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices( QAudio::AudioInput );
    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
            checkboxAudioDevice->setText( list.at(i).deviceName() );
            checkboxAudioDevice->setAccessibleName( list.at(i).deviceName() );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( i ) );
            checkboxAudioDevice->setToolTip( tr ( "Select one or more devices" ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            qDebug().noquote() << global::nameOutput << "Audio device:" << list.at(i).deviceName();
        }
        qDebug();
        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );

        slot_audioDeviceSelected();
    }
    else
    {
        emit signal_haveAudioDeviceSelected( false );
    }
}


void QvkAudioWindows::slot_audioDeviceSelected()
{
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( listCheckBox.at(i)->checkState() == Qt::Checked )
        {
            value = true;
            break;
        }
    }
    emit signal_haveAudioDeviceSelected( value );
}


void QvkAudioWindows::slot_start( bool value )
{
    if ( value == true )
    {
        counter = 0;
        timer->start();
    }
    else
    {
        timer->stop();
    }
}


void QvkAudioWindows::slot_update()
{
    int count = QAudioDeviceInfo::availableDevices( QAudio::AudioInput ).count();
    if ( count != counter )
    {
        counter = count;
    }
}
