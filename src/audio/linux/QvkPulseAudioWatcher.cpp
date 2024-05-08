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
    timer->stop();
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
    QList<QVBoxLayout *> listVBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QVBoxLayout *>();
    if ( list.count() > listVBoxLayout.count() ) {

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
                QString name = list.at(i).section( ":::", 1, 1 );
                QString device = list.at(i).section( ":::", 0, 0 );
                qDebug().noquote() << global::nameOutput << "[PulseAudio] Added:" << name << "Device:" << device;

                // Freie Nummer(xx) 00, 01, 02, xx, 04, 05 usw. ermitteln und diese Nummer dem neuen Layout hinzufügen
                QList<QVBoxLayout *> listVBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QVBoxLayout *>();
                int countLayouts = listVBoxLayout.last()->objectName().last(2).toInt();
                countLayouts++;
                QString prefixNumber;
                if ( countLayouts < 10 ) {
                    prefixNumber = "0" + QString::number( countLayouts );
                } else {
                    prefixNumber = QString::number( countLayouts );
                }
                QVBoxLayout *hBoxLayout = new QVBoxLayout; // Für Checkbox und Progressbar
                hBoxLayout->setObjectName( "vBoxLayoutAudioDevice-" + prefixNumber );
                hBoxLayout->setSpacing(0);
                ui->verticalLayoutAudioDevices->addLayout( hBoxLayout );

                // Checkbox hinzufügen
                QCheckBox *checkBox = new QCheckBox();
                connect( checkBox, SIGNAL( clicked(bool) ), this, SLOT( slot_audioDeviceSelected() ) );
                checkBox->setText( name );
                checkBox->setAccessibleName( device );
                checkBox->setObjectName( "checkboxAudioDevice-" + prefixNumber );
                checkBox->setToolTip( tr ( "Select one or more devices" ) );
                hBoxLayout->addWidget( checkBox );

                // levelmeter mit Widgets verbinden
                qDebug().noquote() << global::nameOutput << "[Audio] Found:" << QString( list.at(i) ).section( ":::", 1, 1 ) << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 );
                QvkLevelMeterController *vkLevelMeterController = new QvkLevelMeterController;
                vkLevelMeterController->add_ProgressBar( checkBox, hBoxLayout );
            }
        }

        QSpacerItem *verticalSpacerAudioDevices = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
        ui->verticalLayoutAudioDevices->addSpacerItem( verticalSpacerAudioDevices );

        slot_audioDeviceSelected();
    }

    // Remove device
    if ( list.count() < listCheckBox.count() ) {
        QString number; // Ist gleich 00, 01, 02, 03 usw.
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            number = checkBox->objectName().right(2);
            if ( stringListAudio_Device.contains( checkBox->accessibleName() ) == false ) {
                QList<QvkLevelMeterController *> listProgressBar = ui->scrollAreaAudioDevice->findChildren<QvkLevelMeterController *>();
                for ( int i = 0; i < listProgressBar.count(); i++ ) {
                    QvkLevelMeterController *vkLevelMeterController = listProgressBar.at(i);
                    if ( vkLevelMeterController->objectName().right(2) == number ) {

                        qDebug().noquote() << global::nameOutput << "[Audio] Remove Widget" << checkBox->objectName() << checkBox->accessibleName();
                        qDebug().noquote() << global::nameOutput << "[Audio] Remove Widget" << vkLevelMeterController->objectName();

                        vkLevelMeterController->remove_ProgressBar();
                        vkLevelMeterController->deleteLater();
                    }
                }
                checkBox->deleteLater();

                QList<QVBoxLayout *> listBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QVBoxLayout *>();
                for ( int i = 0; i < listBoxLayout.count(); i++ ) {
                    QVBoxLayout *vBoxLayout = listBoxLayout.at(i);
                    if ( vBoxLayout->objectName().right(2) == number ) {
                        qDebug().noquote() << global::nameOutput << "[Audio] Remove Widget" << vBoxLayout->objectName();
                        vBoxLayout->deleteLater();
                    }
                }
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

    timer->start();
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
