/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2026 Volker Kohaupt
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

#include "global.h"
#include "QvkAudioPipewireController_wl.h"
#include "QvkAudioPipewireSingle_wl.h"

#include <QDebug>
#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>
#include <QHBoxLayout>
#include <QIcon>
#include <QSize>
#include <QList>
#include <QLineEdit>
#include <QFrame>

QvkAudioPipewireController_wl::QvkAudioPipewireController_wl(Ui_formMainWindow_wl *ui_mainwindow )
{
    ui = ui_mainwindow;

    global::listChildren = new QList<QLineEdit*>();

    ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    global::lineEditPipewireWatcher = new QLineEdit;
    connect(global::lineEditPipewireWatcher,
            &QLineEdit::textChanged,
            this,
            [=](QString value){
        slot_pluggedInOutDevice(value);
    });
    vkAudioPipewireWatcher_wl = new QvkAudioPipewireWatcher_wl( ui );
}


QvkAudioPipewireController_wl::~QvkAudioPipewireController_wl()
{
}


void QvkAudioPipewireController_wl::slot_audioDeviceSelected()
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


void QvkAudioPipewireController_wl::slot_pluggedInOutDevice( QString string )
{
    QString deviceID    = string.section( ":::", 0, 0 ); // DeviceID
    QString description = string.section( ":::", 1, 1 ); // Beschreibung
    QString type        = string.section( ":::", 2, 2 ); // Microphone or speaker
    QString api         = string.section( ":::", 3, 3 ); // alsa
    QString action      = string.section( ":::", 4, 4 ); // Action: Added or removed
    QString device      = string.section( ":::", 5, 5 ); // DeviceName
    Q_UNUSED(api)
    Q_UNUSED(type)

    if ( deviceID == "" ) {
        qDebug().noquote() << global::nameOutput << "[Audio] DeviceID is empty -> return";
        return;
    }
    QvkAudioPipewireSingle_wl *vkAudioPipewireSingle_wl;
    if ( action == "[Audio-device-added]" ) {
        vkAudioPipewireSingle_wl = new QvkAudioPipewireSingle_wl();
        vkAudioPipewireSingle_wl->set_GUIui(ui);
        vkAudioPipewireSingle_wl->setObjectName("AudioPipewireSingle__" + deviceID  );
        ui->verticalLayoutAudioDevices->addWidget(vkAudioPipewireSingle_wl);
        vkAudioPipewireSingle_wl->init(string);
        connect(vkAudioPipewireSingle_wl,
                &QvkAudioPipewireSingle_wl::signal_haveAudioDeviceSelected,
                this,
                [=](bool value){
            ui->labelAudioCodec->setEnabled(value);
            ui->comboBoxAudioCodec->setEnabled(value);
            ui->checkBoxSeparatedAudioTracks->setEnabled(value);
            ui->toolButtonSeparatedAudioTracksReset->setEnabled(value);
        });
        connect(this,
                &QvkAudioPipewireController_wl::signal_haveAudioDeviceSelected,
                this,
                [=](bool value){
            ui->labelAudioCodec->setEnabled(value);
            ui->comboBoxAudioCodec->setEnabled(value);
            ui->checkBoxSeparatedAudioTracks->setEnabled(value);
            ui->toolButtonSeparatedAudioTracksReset->setEnabled(value);
        });

        qDebug().noquote() << global::nameOutput << "[Audio][device added]" << description << device;
    }

    if ( action == "[Audio-device-removed]" ) {
        QList<QvkAudioPipewireSingle_wl *> listQvkAudioPipewireSingle_wl = ui->scrollAreaAudioDevice->findChildren<QvkAudioPipewireSingle_wl *>();
        for ( int i = 0; i < listQvkAudioPipewireSingle_wl.count(); i++ ) {
            QvkAudioPipewireSingle_wl *vkAudioPipewireSingle_wl = listQvkAudioPipewireSingle_wl.at(i);
            if ( vkAudioPipewireSingle_wl->objectName().section("__", 1, 1) == deviceID ) {
                vkAudioPipewireSingle_wl->vkAudioPipewireLevelMeter_wl->stop();
                ui->verticalLayoutAudioDevices->removeWidget(vkAudioPipewireSingle_wl);
                vkAudioPipewireSingle_wl->vkAudioPipewireLevelMeter_wl->deleteLater();
                vkAudioPipewireSingle_wl->deleteLater();

                QLineEdit *LineEdit = global::listChildren->at(i);
                qDebug().noquote() << global::nameOutput << "[Audio] Remove" << LineEdit->objectName();
                global::listChildren->remove(i);;

                qDebug().noquote() << global::nameOutput << "[Audio][device removed]" << deviceID << description << device;
                break;
            }
        }
    }
    slot_audioDeviceSelected();
}
