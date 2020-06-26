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

#include "QvkWASAPIController.h"
#include "QvkWASAPIWatcher.h"
#include "global.h"

#include <QDebug>
#include <QPainter>
#include <QCheckBox>

QvkWASAPIController::QvkWASAPIController(Ui_formMainWindow *ui_mainwindow)
{
    ui = ui_mainwindow;
}


QvkWASAPIController::~QvkWASAPIController()
{
}

/*
 * Set a new icon with a red cross
 */
void QvkWASAPIController::slot_audioIconOnOff( bool state )
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
    } else {
        int index = ui->tabWidgetScreencast->indexOf( ui->tabAudio );
        ui->tabWidgetScreencast->setTabIcon( index, myIcon );
    }
}

void QvkWASAPIController::init()
{
    getAllDevices();

    // QvkWatcherPlug monitoring only new or removed Audiodevices from the PulseAudio server.
    // QvkWatcherPlug does not return any devices, if the PulseAudio server start or stop.
    QvkWASAPIWatcher *vkWASAPIWatcher = new QvkWASAPIWatcher();
    vkWASAPIWatcher->start_monitor();

    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->labelAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->comboBoxAudioCodec, SLOT( setEnabled( bool ) ) );

    connect( global::lineEditAudioPlug, SIGNAL( textChanged( QString ) ), this, SLOT( slot_pluggedInOutDevice( QString ) ) );
}


void QvkWASAPIController::getAllDevices()
{
    int count = ui->verticalLayoutAudioDevices->count();
    for ( int i = 0; i < count; ++i )
    {
        QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
        if ( layoutItem->spacerItem() )
        {
            ui->verticalLayoutAudioDevices->removeItem( layoutItem );
            delete layoutItem;
        }
    }

    QvkWASAPIGstreamer vkWASAPIGstreamer;
    QStringList list;
    list << vkWASAPIGstreamer.get_all_Audio_Source_devices();
    list << vkWASAPIGstreamer.get_all_Audio_Playback_devices();

    if ( !list.empty() )
    {
        for ( int i = 0; i < list.count(); i++ )
        {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            QString device = QString( list.at(i).section( ":::", 0, 0 ) );
            device.append( ":::" );
            device.append( QString( list.at(i).section( ":::", 2, 2 ) ) );
            checkboxAudioDevice->setAccessibleName( device );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( i ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            qDebug().noquote() << global::nameOutput << "[Audio] Found:" << QString( list.at(i) ).section( ":::", 1, 1 )
                                                                         << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 )
                                                                         << "Input/Output:" << QString( list.at(i) ).section( ":::", 2, 2 );

            connect( checkboxAudioDevice, SIGNAL(clicked(bool)),this,SLOT(slot_audioDeviceSelected() ) );
            connect( checkboxAudioDevice, SIGNAL(clicked(bool)),this,SLOT(slot_checkBox( bool ) ) );
        }
        qDebug().noquote();

        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
    }
    else
    {
        QLabel *label = new QLabel();
        label->setText( "No audio recording device found, please read the online help." );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter);
        ui->verticalLayoutAudioDevices->addWidget( label );
    }
}


void QvkWASAPIController::slot_checkBox( bool value )
{
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( listCheckBox.at(i)->underMouse() )
        {
            for ( int i = 0; i < listCheckBox.count(); i++ )
            {
                if ( listCheckBox.at(i)->isChecked() )
                {
                    listCheckBox.at(i)->setChecked( false );
                }
            }
            listCheckBox.at(i)->setChecked( value );
        }
    }
}


void QvkWASAPIController::slot_audioDeviceSelected()
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
    slot_audioIconOnOff( value );
    emit signal_haveAudioDeviceSelected( value );
}


void QvkWASAPIController::slot_pluggedInOutDevice( QString string )
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
        checkboxAudioDevice->setToolTip( tr ( "Select one or more devices" ) );
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

