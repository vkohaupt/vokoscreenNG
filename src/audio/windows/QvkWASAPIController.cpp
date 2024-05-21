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

#include "QvkWASAPIController.h"
#include "global.h"
#include "QvkLevelMeterController.h"

#include <QDebug>
#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>
#include <QVBoxLayout>
#include <QIcon>
#include <QSize>
#include <QList>
#include <QAudioDevice>
#include <QMediaDevices>

QvkWASAPIController::QvkWASAPIController( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
    global::lineEditWASAPIWatcher = new QLineEdit;
    connect( this, SIGNAL( signal_haveAudioDeviceSelected(bool) ), ui->labelAudioCodec,    SLOT( setEnabled(bool) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected(bool) ), ui->comboBoxAudioCodec, SLOT( setEnabled(bool) ) );
    connect( global::lineEditWASAPIWatcher, SIGNAL( textChanged(QString) ), this, SLOT( slot_pluggedInOutDevice(QString) ) );
    vkWASAPIWatcher = new QvkWASAPIWatcher( ui );
    slot_audioDeviceSelected();
}


QvkWASAPIController::~QvkWASAPIController()
{
}


void QvkWASAPIController::slot_audioDeviceSelected()
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


void QvkWASAPIController::slot_pluggedInOutDevice( QString string )
{
    QList<QLabel *> listLabel = ui->scrollAreaAudioDevice->findChildren<QLabel *>();
    for ( int i = 0; i < listLabel.count(); i++ ) {
        ui->verticalLayoutAudioDevices->removeWidget( listLabel.at(i) );
        delete listLabel.at(i);
    }

    QString device = string.section( ":::", 0, 0 ); // DeviceID
    QString name   = string.section( ":::", 1, 1 ); // Name
    QString type   = string.section( ":::", 2, 2 ); // Microphone or speaker
    QString api    = string.section( ":::", 3, 3 ); // WASAPI2
    QString action = string.section( ":::", 4, 4 ); // Action: Added or removed
    Q_UNUSED(api)

    if ( action == "[Audio-device-added]" ) {
        QCheckBox *checkboxAudioDevice = new QCheckBox();
        connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        checkboxAudioDevice->setText( name );
        checkboxAudioDevice->setAccessibleName( string );
        QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( listAudioDevices.count() ) );

        if ( type == "Playback" ) {
            checkboxAudioDevice->setIconSize( QSize( 13, 13 ) );
            checkboxAudioDevice->setIcon( QIcon( ":/pictures/screencast/speaker.png" ) );
        }
        if ( type == "Source" ) {
            checkboxAudioDevice->setIconSize( QSize( 16, 16 ) );
            checkboxAudioDevice->setIcon( QIcon( ":/pictures/screencast/microphone.png" ) );
        }

        ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count(), checkboxAudioDevice );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );

        qDebug().noquote() << global::nameOutput << "[Audio-device-added]" << name << device;
    }

    if ( action == "[Audio-device-removed]" ) {
        QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        for ( int i = 0; i < listAudioDevices.count(); i++ ) {
            QCheckBox *checkBox = listAudioDevices.at(i);
            if ( checkBox->accessibleName().section( ":::", 0, 0 ) == device ) {
                delete checkBox;
                listAudioDevices.removeAt(i);
                qDebug().noquote() << global::nameOutput << "[Audio-device-removed]" << name << device;
            }
        }
        slot_audioDeviceSelected();

        if ( listAudioDevices.empty() ) {
            QLabel *label = new QLabel();
            label->setText( "WASAPI\n" );
            label->setAlignment( Qt::AlignCenter );
            ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
            ui->verticalLayoutAudioDevices->addWidget( label );

            QLabel *labelText = new QLabel();
            labelText->setText( "No device found for audio recording." );
            ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignCenter );
            ui->verticalLayoutAudioDevices->addWidget( labelText );
        }
    }
}
