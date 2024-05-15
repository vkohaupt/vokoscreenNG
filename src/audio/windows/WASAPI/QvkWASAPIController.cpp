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


void QvkWASAPIController::init()
{
    vkWASAPIWatcher = new QvkWASAPIWatcher( ui );

    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->labelAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->comboBoxAudioCodec, SLOT( setEnabled( bool ) ) );

    connect( vkWASAPIWatcher->lineEditWASAPIAudioPlug, SIGNAL( textChanged( QString ) ), this, SLOT( slot_pluggedInOutDevice( QString ) ) );
}


void QvkWASAPIController::getAllDevices()
{
    QList<QLabel *> listLabel = ui->scrollAreaAudioDevice->findChildren<QLabel *>();
    for ( int i = 0; i < listLabel.count(); i++ ) {
        ui->verticalLayoutAudioDevices->removeWidget( listLabel.at(i) );
        delete listLabel.at(i);
    }

    QvkWASAPIGstreamer vkWASAPIGstreamer;
    QStringList list;
    list << vkWASAPIGstreamer.get_all_Audio_Source_devices();

    if ( !list.empty() ) {
        for ( int i = 0; i < list.count(); i++ ) {
            QCheckBox *checkboxAudioDevice = new QCheckBox();
            checkboxAudioDevice->setText( QString( list.at(i) ).section( ":::", 1, 1 ) );
            QString device = QString( list.at(i).section( ":::", 0, 0 ) );
            device.append( ":::" );
            device.append( QString( list.at(i).section( ":::", 2, 2 ) ) );
            checkboxAudioDevice->setAccessibleName( device );
            checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( i ) );
            checkboxAudioDevice->setToolTip( tr ( "Select one or more devices" ) );
            ui->verticalLayoutAudioDevices->addWidget( checkboxAudioDevice );
            ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
            qDebug().noquote() << global::nameOutput << "[Audio WASAPI2] Found:" << QString( list.at(i) ).section( ":::", 1, 1 )
                               << "Device:" << QString( list.at(i) ).section( ":::", 0, 0 )
                               << "Input/Output:" << QString( list.at(i) ).section( ":::", 2, 2 );

            connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        }
        qDebug().noquote();
    } else {
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


void QvkWASAPIController::slot_audioDeviceSelected()
{
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        if ( listCheckBox.at(i)->checkState() == Qt::Checked ) {
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

    QString header = string.section( "---", 0, 0 );
    QString name   = string.section( "---", 1, 1 );
    QString device = string.section( "---", 2, 2 );

    if ( header == "[Audio-device-added]" ) {
        QCheckBox *checkboxAudioDevice = new QCheckBox();
        connect( checkboxAudioDevice, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        checkboxAudioDevice->setText( name );
        checkboxAudioDevice->setAccessibleName( device );
        QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        checkboxAudioDevice->setObjectName( "checkboxAudioDevice-" + QString::number( listAudioDevices.count() ) );
        ui->verticalLayoutAudioDevices->insertWidget( ui->verticalLayoutAudioDevices->count()-1, checkboxAudioDevice );
        ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    }

    if ( header == "[Audio-device-removed]" ) {
        QList<QCheckBox *> listAudioDevices = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        for ( int i = 0; i < listAudioDevices.count(); i++ ) {
            if ( listAudioDevices.at(i)->accessibleName() == device ) {
                delete listAudioDevices.at(i);
                listAudioDevices.removeAt(i);
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
