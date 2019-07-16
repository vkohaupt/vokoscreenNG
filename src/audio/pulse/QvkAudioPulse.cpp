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

#include "QvkAudioPulse.h"
#include "QvkWatcherPlug.h"
#include "global.h"

#include <QDebug>

QvkAudioPulse::QvkAudioPulse( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;
}


void QvkAudioPulse::init()
{
    getPulseDevices();

    // QvkWatcherPlug monitoring only new or removed Audiodevices from the PulseAudio server.
    // QvkWatcherPlug does not return any devices, if the PulseAudio server start or stop.
    QvkWatcherPlug *vkWatcherPlug = new QvkWatcherPlug();
    vkWatcherPlug->start_monitor();

    connect( global::lineEditAudioPlug, SIGNAL( textChanged( QString ) ), this, SLOT( slot_setDevice( QString ) ) );
}


QvkAudioPulse::~QvkAudioPulse()
{
}

void QvkAudioPulse::slot_setDevice( QString string )
{
    QString header = string.section( ":", 0, 0 );
    QString name   = string.section( ":", 1, 1 );
    QString device = string.section( ":", 2, 2 );

    if ( header == "[Audio-device-added]" )
    {
        QCheckBox *checkboxAudioDevice = new QCheckBox();
        connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        checkboxAudioDevice->setText( name );
        checkboxAudioDevice->setAccessibleName( device );
        QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( listAudioDevices.count() ) );
        ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count()-1, checkboxAudioDevice );
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
        slot_audioDeviceSelected();
    }
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
            connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            checkboxAudioDevice->setAccessibleName( QString( list.at(i) ).section( ":::", 0, 0 ) );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( i ) );
            checkboxAudioDevice->setToolTip( tr ( "Select one or more devices" ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            qDebug().noquote() << global::nameOutput << "PulseAudio device:" << list.at(i);
        }
        qDebug().noquote();

        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
        slot_audioDeviceSelected();
    }
    else
    {
        emit signal_haveAudioDeviceSelected( false );
    }
}


void QvkAudioPulse::slot_audioDeviceSelected()
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
