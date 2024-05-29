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

    ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );

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

    QList<QLabel *> deleteLabel = ui->scrollAreaAudioDevice->findChildren<QLabel *>();
    for ( int x = 0; x < deleteLabel.count(); x ++ ) {
        delete deleteLabel.at(x);
    }

    // Add new Device
    QList<QVBoxLayout *> listVBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QVBoxLayout *>();
    if ( list.count() > listVBoxLayout.count() ) {
        for ( int i = 0; i < stringListAudio_Device.count(); i++ ) {
            if ( stringListCheckBox.contains( stringListAudio_Device.at(i) ) == false ) {
                QString name = list.at(i).section( ":::", 1, 1 );
                QString device = list.at(i).section( ":::", 0, 0 );
                qDebug().noquote() << global::nameOutput << "[PulseAudio] Added:" << name << "Device:" << device;

                // Freier Index(xx) 00, 01, 02, xx, 04, 05 usw. ermitteln und diesen Index dem neuen Layout, CheckBox und ProgressBar hinzufügen
                QList<QVBoxLayout *> listVBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QVBoxLayout *>();
                QString indexNumber;
                if ( listVBoxLayout.empty() ) {
                    indexNumber = "00";
                    qDebug().noquote() << global::nameOutput << "Index in List: List is empty" << "Count befor add:" << listVBoxLayout.count() << "New index:" << indexNumber ;
                } else {
                    QStringList indexStringList;
                    for ( int i = 0; i < listVBoxLayout.count(); i++ ) {
                        QVBoxLayout *vBoxLayout = listVBoxLayout.at(i);
                        indexStringList << vBoxLayout->objectName().right(2);
                    }
                    // Max 30 Audio Geräte
                    for ( int x = 1; x < 30; x++ ) {
                        if ( x < 10 ) {
                            indexNumber = "0" + QString::number(x);
                        } else {
                            indexNumber = QString::number(x);
                        }
                        if ( indexStringList.contains(indexNumber) == false ) {
                            break;
                        }
                    }
                    qDebug().noquote() << global::nameOutput << "Index in List" << indexStringList << "Count befor add:" << listVBoxLayout.count() << "New index:" << indexNumber ;
                }

                QVBoxLayout *vBoxLayout = new QVBoxLayout; // Für Checkbox und Progressbar
                vBoxLayout->setObjectName( "vBoxLayoutAudioDevice-" + indexNumber );
                vBoxLayout->setSpacing(0);
                ui->verticalLayoutAudioDevices->addLayout( vBoxLayout );

                // Checkbox hinzufügen
                QCheckBox *checkBox = new QCheckBox();
                connect( checkBox, SIGNAL( clicked(bool) ), this, SLOT( slot_audioDeviceSelected() ) );
                checkBox->setText( name );
                checkBox->setAccessibleName( device );
                checkBox->setObjectName( "checkboxAudioDevice-" + indexNumber );
                checkBox->setToolTip( tr ( "Select one or more devices" ) );
                checkBox->setIcon( QIcon( ":/pictures/screencast/microphone.png" ) );
                checkBox->setIconSize( QSize( 16, 16 ) );
                vBoxLayout->addWidget( checkBox );

                // levelmeter mit Widgets verbinden
                qDebug().noquote() << global::nameOutput << "[Audio] Found:" << QString( list.at(i) ).section( ":::", 1, 1 ) << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 );
                QvkLevelMeterController *vkLevelMeterController = new QvkLevelMeterController;
                vkLevelMeterController->add_ProgressBar( checkBox, vBoxLayout );

                qDebug().noquote() << global::nameOutput << "[Audio] add Widget" << vBoxLayout->objectName();
                qDebug().noquote() << global::nameOutput << "[Audio] add Widget" << checkBox->objectName() << checkBox->accessibleName();
                qDebug().noquote() << global::nameOutput << "[Audio] add Widget" << vkLevelMeterController->objectName();
                qDebug().noquote();
            }
        }

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

                        vkLevelMeterController->remove_ProgressBar( checkBox); //------------------------------------------------------------------------
                        vkLevelMeterController->deleteLater();
                    }
                }
                checkBox->deleteLater();

                QList<QVBoxLayout *> listBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QVBoxLayout *>();
                for ( int i = 0; i < listBoxLayout.count(); i++ ) {
                    QVBoxLayout *vBoxLayout = listBoxLayout.at(i);
                    if ( vBoxLayout->objectName().right(2) == number ) {
                        qDebug().noquote() << global::nameOutput << "[Audio] Remove Widget" << vBoxLayout->objectName();
                        qDebug().noquote();
                        vBoxLayout->deleteLater();
                    }
                }
            }
        }
    }

    if ( list.empty() and ( QvkPulseAudioServer::isAvailable() == false ) ) {
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
