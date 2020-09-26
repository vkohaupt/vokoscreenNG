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

#include "QvkPulseAudioWatcher.h"
#include "QvkPulseAudioDevices.h"
#include "global.h"

#include <QDebug>
#include <QCheckBox>
#include <QLabel>
#include <QSpacerItem>
#include <QPainter>

/*
 * QvkWatcherPlug monitoring only new or removed Audiodevices.
 */
QvkPulseAudioWatcher::QvkPulseAudioWatcher( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 3000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_update() ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->labelAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->comboBoxAudioCodec, SLOT( setEnabled( bool ) ) );
}


QvkPulseAudioWatcher::~QvkPulseAudioWatcher()
{}


void QvkPulseAudioWatcher::start_monitor()
{
    timer->start();
}


void QvkPulseAudioWatcher::slot_update()
{
    ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft );

    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    QStringList list = QvkPulseAudioDevices::getAllDevices();

    QStringList stringListCheckBox;
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        stringListCheckBox.append( listCheckBox.at(i)->accessibleName() );
    }

    QStringList stringListAudio_Device;
    for ( int i = 0; i < list.count(); i++ )
    {
        stringListAudio_Device.append( list.at(i).section( ":::", 0, 0 ) );
    }

    QList<QLabel *> deleteLabel = ui->scrollAreaAudioDevice->findChildren<QLabel *>();
    for ( int x = 0; x < deleteLabel.count(); x ++ )
    {
        delete deleteLabel.at(x);
    }

    // Add new Device
    if ( list.count() > listCheckBox.count() )
    {
        // Remove SpacerItem
        for (int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i)
        {
            QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
            if ( layoutItem->spacerItem() )
            {
                ui->verticalLayoutAudioDevices->removeItem( layoutItem );
                delete layoutItem;
                --i;
            }
        }

        for ( int i = 0; i < stringListAudio_Device.count(); i++ )
        {
            if ( stringListCheckBox.contains( stringListAudio_Device.at(i) ) == false )
            {
                QString name = list.at(i).section( ":::", 1, 1 );
                QString device = list.at(i).section( ":::", 0, 0 );
                qDebug().noquote() << "[PulseAudio] Added:" << name << "Device:" << device;

                QCheckBox *checkboxAudioDevice = new QCheckBox();
                connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
                checkboxAudioDevice->setText( name );
                checkboxAudioDevice->setAccessibleName( device );
                QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
                checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( listAudioDevices.count() ) );
                checkboxAudioDevice->setToolTip( tr ( "Select one or more devices" ) );
                ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            }
        }

        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
        slot_audioDeviceSelected();
    }

    // Remove device
    if ( list.count() < listCheckBox.count() )
    {
        for ( int i = 0; i < listCheckBox.count(); i++ )
        {
            if ( stringListAudio_Device.contains( listCheckBox.at(i)->accessibleName() ) == false )
            {
                qDebug().noquote() << "[PulseAudio] Removed:" << listCheckBox.at(i)->text() << "Device:" << listCheckBox.at(i)->accessibleName();
                delete listCheckBox.at(i);
            }
        }
        slot_audioDeviceSelected();
    }

    if ( list.empty() )
    {
        // Remove SpacerItem
        for (int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i)
        {
            QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
            if ( layoutItem->spacerItem() )
            {
                ui->verticalLayoutAudioDevices->removeItem( layoutItem );
                delete layoutItem;
                --i;
            }
        }

        QList<QLabel *> deleteLabel = ui->verticalLayoutAudioDevices->findChildren<QLabel *>();
        for ( int x = 0; x < deleteLabel.count(); x ++ )
        {
            delete deleteLabel.at(x);
        }

        QLabel *label = new QLabel();
        label->setText( "No audio recording device found." );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter);
        ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count()-1, label );
        slot_audioDeviceSelected();
    }
}


void QvkPulseAudioWatcher::slot_audioDeviceSelected()
{
    audioIconOnOff( isAudioDeviceSelected() );
}


bool QvkPulseAudioWatcher::isAudioDeviceSelected()
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
    return value;
}


/*
 * Set a new icon with a red cross
 */
void QvkPulseAudioWatcher::audioIconOnOff( bool state )
{
    QIcon myIcon( ":/pictures/screencast/microphone.png" );
    if ( state == false  )
    {
        QSize size = ui->tabWidgetScreencast->iconSize();
        QPixmap workPixmap( myIcon.pixmap( size ) );
        QPainter painter;
        QPen pen;
        painter.begin( &workPixmap );
        pen.setColor( Qt::red );
        pen.setWidth( 2 );
        painter.setPen( pen );
        painter.drawLine ( 5, 5, size.width()-5, size.height()-5 );
        painter.drawLine ( 5, size.height()-5, size.width()-5, 5 );
        painter.end();
        int index = ui->tabWidgetScreencast->indexOf( ui->tabAudio );
        ui->tabWidgetScreencast->setTabIcon( index, workPixmap );
        emit signal_haveAudioDeviceSelected( false );
    } else {
        int index = ui->tabWidgetScreencast->indexOf( ui->tabAudio );
        ui->tabWidgetScreencast->setTabIcon( index, myIcon );
        emit signal_haveAudioDeviceSelected( true );
    }
}
