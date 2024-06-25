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
#include <QProgressBar>

#include "QvkAudioController.h"
#include "QvkPulseAudioWatcher.h"
#include "QvkPulseAudioServer.h"
#include "QvkPulseAudioDevices.h"
#include "global.h"

QvkAudioController::QvkAudioController( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
    ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    global::listChildren = new QList<QLineEdit*>();
    ui->scrollAreaWidgetContentsAudioDevices->setStyleSheet( "background-color: white; " );
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

// systemctl --user stop pipewire.socket
// systemctl --user start pipewire.socket


void QvkAudioController::getAllDevices()
{
    QStringList list;
    if ( QvkPulseAudioServer::isAvailable() ) {
        list << QvkPulseAudioDevices::getAllDevices();
        if ( !list.empty() ) {
            for ( int i = 0; i < list.count(); i++ ) {

                // Führende Null voranstellen
                QString index;
                if ( i < 10 ) {
                    index = "0" + QString::number(i);
                } else {
                    index = QString::number(i);
                }

                QHBoxLayout *hBoxLayout = new QHBoxLayout; // Für Checkbox und Progressbar
                hBoxLayout->setSpacing(0);
                hBoxLayout->setObjectName( "vBoxLayoutAudioDevice-" + index );
                QCheckBox *checkBox = new QCheckBox();
                connect( checkBox, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
                checkBox->setAccessibleName( QString( list.at(i) ).section( ":::", 0, 0 ) );
                checkBox->setObjectName( "checkboxAudioDevice-" + index );
                checkBox->setToolTip( tr ( "Select one or more devices" ) );

                if ( QString( list.at(i) ).section( ":::", 0, 0 ).contains( "monitor" ) ) {
                    checkBox->setIconSize( QSize( 15, 15 ) );
                    checkBox->setIcon( QIcon( ":/pictures/screencast/speaker.png" ) );
                } else {
                    checkBox->setIconSize( QSize( 16, 16 ) );
                    checkBox->setIcon( QIcon( ":/pictures/screencast/microphone.png" ) );
                }

                hBoxLayout->addWidget( checkBox );
                ui->verticalLayoutAudioDevices->addLayout( hBoxLayout );

                qDebug().noquote() << global::nameOutput << "[Audio] Found:" << QString( list.at(i) ).section( ":::", 1, 1 ) << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 );

                vkLevelMeterController = new QvkLevelMeterController;
                vkLevelMeterController->add_ProgressBar( checkBox, hBoxLayout, QString( list.at(i) ).section( ":::", 1, 1 ) );
                vkLevelMeterController->set_Text( QString( list.at(i) ).section( ":::", 1, 1 ) );

            }
            qDebug().noquote();

            QvkPulseAudioWatcher *vkPulseAudioWatcher = new QvkPulseAudioWatcher( ui );
            vkPulseAudioWatcher->start_monitor();
        } else {
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
            ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
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
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listCheckBox.at(i);
        if ( checkBox->checkState() == Qt::Checked ) {
            value = true;
            break;
        }
    }
    emit signal_haveAudioDeviceSelected( value );
}
