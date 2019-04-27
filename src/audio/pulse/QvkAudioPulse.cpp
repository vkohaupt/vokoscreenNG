/* vokoscreen - A desktop recorder
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

#include "QvkAudioPulse.h"
#include "QvkWatcherPlug.h"
#include "global.h"

#include <QDebug>
#include <QAudioDeviceInfo>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

QvkAudioPulse::QvkAudioPulse( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;
    connect( mainWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( slot_deletePlugFile() ) );
}


void QvkAudioPulse::slot_deletePlugFile()
{
    QFile file( QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + "/" + global::name + "AudioPlugFile.txt" );
    file.remove();
}


void QvkAudioPulse::init()
{
    getPulseDevices();

    // QvkWatcherPlug monitoring only new or removed Audiodevices from the PulseAudio server.
    // QvkWatcherPlug does not return any devices, if the PulseAudio server start or stop.
    QvkWatcherPlug *vkWatcherPlug = new QvkWatcherPlug();
    vkWatcherPlug->start_monitor();

    // Before watching the file from QvkWatcherPlug, the watched file must be present.
    // If the file not present or is removed, the fileSystemWatcher will not or not more monitoring.
    // We create a empty file. If the file exists, it will be overwritten.
    QString path = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + "/" + global::name + "AudioPlugFile.txt";
    QFile file( path );
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    file.close();
    fileSystemWatcher = new QFileSystemWatcher();
    fileSystemWatcher->addPath( path );
    connect( fileSystemWatcher, SIGNAL( fileChanged( QString ) ), this, SLOT( slot_myfileSystemWatcher( QString ) ) );
}


QvkAudioPulse::~QvkAudioPulse()
{
}


void QvkAudioPulse::slot_myfileSystemWatcher( QString string )
{
    QFile file( string );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        QString message = tr( "File can not be opened" );
        QMessageBox::information( nullptr, global::name + " error ", message + ": " + string );
        qDebug().noquote() << global::nameOutput << "File can not be opened:" << string;
    }

    QTextStream in( &file );

    while( !in.atEnd() )
    {
        QString header = in.readLine();
        QString name = in.readLine();
        QString device = in.readLine();

        if ( header == "[Audio-device-added]" )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( name );
            checkboxAudioDevice->setAccessibleName( device );
            ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count()-1, checkboxAudioDevice );
            checkboxAudioDevice->setAutoExclusive( true );
        }

        if ( header == "[Audio-device-removed]" )
        {
            QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
            for ( int i = 0; i < listAudioDevices.count(); i++ )
            {
                if ( listAudioDevices.at(i)->accessibleName() == device )
                {
                    delete listAudioDevices.at(i);
                }
            }
        }
    }

    file.close();
}


void QvkAudioPulse::getPulseDevices()
{
    QvkPulseGstr vkPulseGstr;
    QStringList list;
    list << vkPulseGstr.get_all_Audio_devices();

    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            checkboxAudioDevice->setAccessibleName( QString( list.at(i) ).section( ":::", 0, 0 ) );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( i ) );
            checkboxAudioDevice->setToolTip( tr ( "Select one or more devices" ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            checkboxAudioDevice->setAutoExclusive( false );

            qDebug().noquote() << global::nameOutput << "PulseAudio device:" << list.at(i);
        }
        qDebug().noquote();

        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
    }
    else
    {
        emit signal_noAudioDevicesAvalaible( false );
    }
}


void QvkAudioPulse::clearVerticalLayoutAudioDevices()
{
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
       ui->verticalLayoutAudioDevices->removeWidget( listQCheckBox.at(i) );
       delete listQCheckBox.at(i);
    }

    for ( int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i )
    {
        QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
        if ( layoutItem->spacerItem() )
        {
            ui->verticalLayoutAudioDevices->removeItem( layoutItem );
            delete layoutItem;
        }
    }
}

