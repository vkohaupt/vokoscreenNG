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

#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QCheckBox>
#include <QLabel>

#include "QvkAudioController.h"
#include "QvkPulseAudioWatcher.h"
#include "QvkPulseAudioServer.h"
#include "QvkPulseAudioDevices.h"
#include "global.h"

QvkAudioController::QvkAudioController( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkAudioController::~QvkAudioController()
{
}


void QvkAudioController::init()
{
    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->labelAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->comboBoxAudioCodec, SLOT( setEnabled( bool ) ) );
    getAllDevices();
}


void QvkAudioController::getAllDevices()
{
    QStringList list;
    if ( QvkPulseAudioServer::isAvailable() )
    {
        list << QvkPulseAudioDevices::getAllDevices();
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
                qDebug().noquote() << global::nameOutput << "[Audio] Found:" << QString( list.at(i) ).section( ":::", 1, 1 ) << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 );
            }
            qDebug().noquote();

            QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
            ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );

            QvkPulseAudioWatcher *vkPulseAudioWatcher = new QvkPulseAudioWatcher( ui );
            vkPulseAudioWatcher->start_monitor();
        }
        else
        {
            QLabel *label = new QLabel();
            label->setText( "PulseAudio\n" );
            label->setAlignment( Qt::AlignCenter );
            ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
            ui->verticalLayoutAudioDevices->addWidget( label );

            QLabel *labelText = new QLabel();
            labelText->setText( "No device found for audio recording." );
            ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
            ui->verticalLayoutAudioDevices->addWidget( labelText );

            QvkPulseAudioWatcher *vkPulseAudioWatcher = new QvkPulseAudioWatcher( ui );
            vkPulseAudioWatcher->start_monitor();
        }
    } else {
        QLabel *label = new QLabel();
        label->setText( "PulseAudio not found\n" );
        label->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->addWidget( label );

        QLabel *labelText = new QLabel();
        labelText->setText( "Please install or start Pulseaudio" );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->addWidget( labelText );

        QvkPulseAudioWatcher *vkPulseAudioWatcher = new QvkPulseAudioWatcher( ui );
        vkPulseAudioWatcher->start_monitor();
    }

    slot_audioDeviceSelected();
}


void QvkAudioController::slot_audioDeviceSelected()
{
    audioIconOnOff( isAudioDeviceSelected() );
}


bool QvkAudioController::isAudioDeviceSelected()
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
void QvkAudioController::audioIconOnOff( bool state )
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

