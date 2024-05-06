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

#include "QvkPulseAudioWatcher.h"
#include "QvkPulseAudioDevices.h"
#include "QvkPulseAudioServer.h"
#include "global.h"

#include <QDebug>
#include <QCheckBox>
#include <QLabel>
#include <QSpacerItem>
#include <QPainter>
#include <QProgressBar>
#include "QvkLevelMeterController.h"


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
    connect( this, SIGNAL( signal_haveAudioDeviceSelected(bool) ), ui->labelAudioCodec,    SLOT( setEnabled(bool) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected(bool) ), ui->comboBoxAudioCodec, SLOT( setEnabled(bool) ) );
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
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listCheckBox.at(i);
        stringListCheckBox.append( checkBox->accessibleName() );
    }

    QStringList stringListAudio_Device;
    for ( int i = 0; i < list.count(); i++ ) {
        stringListAudio_Device.append( list.at(i).section( ":::", 0, 0 ) );
    }

    // Add new Device
    if ( list.count() > listCheckBox.count() ) {
        // Remove SpacerItem
        for (int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i) {
            QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
            if ( layoutItem->spacerItem() ) {
                ui->verticalLayoutAudioDevices->removeItem( layoutItem );
                delete layoutItem;
                --i;
            }
        }

        for ( int i = 0; i < stringListAudio_Device.count(); i++ ) {
            if ( stringListCheckBox.contains( stringListAudio_Device.at(i) ) == false ) {
                QString name = list.at(i).section( ":::", 1, 1 ).left(45);
                QString device = list.at(i).section( ":::", 0, 0 );
                qDebug().noquote() << global::nameOutput << "[PulseAudio] Added:" << name << "Device:" << device;

                // Anzahl der Layouts ermitteln und ein Layout hinzufügen
                QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
                int countLayouts = listAudioDevices.count();
                // Führende Null voranstellen
                QString prefixNumber;
                if ( countLayouts < 10 ) {
                    prefixNumber = "0" + QString::number( countLayouts );
                } else {
                    prefixNumber = QString::number( countLayouts );
                }
                QHBoxLayout *hBoxLayout = new QHBoxLayout; // Für Checkbox und Progressbar
                hBoxLayout->setObjectName( "hBoxLayoutAudioDevice-" + prefixNumber );
                ui->verticalLayoutAudioDevices->addLayout( hBoxLayout );

                // Checkbox hinzufügen
                QCheckBox *checkBox = new QCheckBox();
                connect( checkBox, SIGNAL( clicked(bool) ), this, SLOT( slot_audioDeviceSelected() ) );
                checkBox->setText( name );
                checkBox->setAccessibleName( device );
                checkBox->setObjectName( "checkboxAudioDevice-" + prefixNumber );
                checkBox->setToolTip( tr ( "Select one or more devices" ) );
                hBoxLayout->addWidget( checkBox );

                // Progressbar hinzufügen
                QProgressBar *progressBar = new QProgressBar;
                progressBar->setObjectName( "progressBarAudioDevice-" + prefixNumber );
                progressBar->setFixedWidth(130);
                progressBar->setFixedHeight(8);
                progressBar->setTextVisible(false);
                progressBar->setMinimum(0);
                progressBar->setMaximum(10000);
                progressBar->setToolTip( name );
                hBoxLayout->addWidget( progressBar );

                // levelmeter mit Widgets verbinden
                qDebug().noquote() << global::nameOutput << "[Audio] Found:" << QString( list.at(i) ).section( ":::", 1, 1 ) << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 );
                QvkLevelMeterController *vkLevelMeterController = new QvkLevelMeterController;
                vkLevelMeterController->set_levelmeterOnProgressBar( checkBox, progressBar );
            }
        }

        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );
        slot_audioDeviceSelected();
    }

    // Remove device
    if ( list.count() < listCheckBox.count() ) {
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( stringListAudio_Device.contains( checkBox->accessibleName() ) == false ) {
                QList<QHBoxLayout *> listHBoxLayout = ui->scrollAreaAudioDevice->findChildren<QHBoxLayout *>();
                QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
                QList<QProgressBar *> listProgressBar = ui->scrollAreaAudioDevice->findChildren<QProgressBar *>();
                delete listCheckBox.at(i);
                delete listProgressBar.at(i);
                delete listHBoxLayout.at(i);
            }
        }
    }

    if ( list.empty() and ( QvkPulseAudioServer::isAvailable() == false ) ) {
        // Remove SpacerItem
        for (int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i) {
            QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
            if ( layoutItem->spacerItem() ) {
                ui->verticalLayoutAudioDevices->removeItem( layoutItem );
                delete layoutItem;
                --i;
            }
        }

        QList<QLabel *> deleteLabel = ui->verticalLayoutAudioDevices->findChildren<QLabel *>();
        for ( int x = 0; x < deleteLabel.count(); x ++ ) {
            delete deleteLabel.at(x);
        }

        QLabel *label = new QLabel();
        label->setText( "PulseAudio not found\n" );
        label->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->addWidget( label );

        QLabel *labelText = new QLabel();
        labelText->setText( "Please install or start Pulseaudio" );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->addWidget( labelText );

        slot_audioDeviceSelected();
    }

    if ( list.empty() and ( QvkPulseAudioServer::isAvailable() == true ) ) {
        // Remove SpacerItem
        for (int i = 0; i < ui->verticalLayoutAudioDevices->count(); ++i) {
            QLayoutItem *layoutItem = ui->verticalLayoutAudioDevices->itemAt(i);
            if ( layoutItem->spacerItem() ) {
                ui->verticalLayoutAudioDevices->removeItem( layoutItem );
                delete layoutItem;
                --i;
            }
        }

        QList<QLabel *> deleteLabel = ui->verticalLayoutAudioDevices->findChildren<QLabel *>();
        for ( int x = 0; x < deleteLabel.count(); x ++ ) {
            delete deleteLabel.at(x);
        }

        QLabel *label = new QLabel();
        label->setText( "PulseAudio\n" );
        label->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->addWidget( label );

        QLabel *labelText = new QLabel();
        labelText->setText( "No device found for audio recording." );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
        ui->verticalLayoutAudioDevices->addWidget( labelText );

        slot_audioDeviceSelected();
    }

}


void QvkPulseAudioWatcher::slot_audioDeviceSelected()
{
    isAudioDeviceSelected();
}


bool QvkPulseAudioWatcher::isAudioDeviceSelected()
{
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listCheckBox.at(i);
        if ( checkBox->checkState() == Qt::Checked ) {
            value = true;
            break;
        }
    }
    return value;
}
