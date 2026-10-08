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
#include "QvkAudioWindowsController.h"
#include "QvkAudioWindowsSingle.h"

#include <QString>
#include <QDebug>
#include <QCheckBox>
#include <QList>
#include <QToolButton>

QvkAudioWindowsController::QvkAudioWindowsController(Ui_formMainWindow *ui_mainwindow)
{
    ui = ui_mainwindow;

    ui->verticalLayoutAudioDevices->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    vkAudioWindowsWatcher = new QvkAudioWindowsWatcher(ui);
    connect(vkAudioWindowsWatcher,
            &QvkAudioWindowsWatcher::signal_audio_added_removed,
            this,
            [=](QString value){
        slot_pluggedInOutDevice(value);
    });
}


QvkAudioWindowsController::~QvkAudioWindowsController()
{
}


void QvkAudioWindowsController::slot_audioDeviceSelected()
{
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for(int i = 0; i < listCheckBox.count(); i++){
        QCheckBox *checkBox = listCheckBox.at(i);
        if (checkBox->checkState() == Qt::Checked){
            value = true;
            break;
        }
    }
    emit signal_haveAudioDeviceSelected(value);
}


void QvkAudioWindowsController::slot_pluggedInOutDevice(QString string)
{
    QString deviceID    = string.section(":::", 0, 0); // DeviceID
    QString description = string.section(":::", 1, 1); // Beschreibung
    QString type        = string.section(":::", 2, 2); // Microphone or speaker
    QString api         = string.section(":::", 3, 3); // alsa
    QString action      = string.section(":::", 4, 4); // Action: Added or removed
    QString device      = string.section(":::", 5, 5); // DeviceName
    Q_UNUSED(api)
    Q_UNUSED(type)

    if (deviceID == ""){
        qDebug().noquote() << global::nameOutput << "[Audio] DeviceID is empty -> return";
        return;
    }

    QvkAudioWindowsSingle *vkAudioWindowsSingle;
    if (action == "[Audio-device-added]"){
        qDebug().noquote() << global::nameOutput << "[Audio][Controller]"
                           << deviceID
                           << "Added:"
                           << description
                           << device;
        vkAudioWindowsSingle = new QvkAudioWindowsSingle();
        vkAudioWindowsSingle->set_GUIui(ui);
        vkAudioWindowsSingle->setObjectName("AudioWindowsSingle__" + deviceID);
        ui->verticalLayoutAudioDevices->addWidget(vkAudioWindowsSingle);
        vkAudioWindowsSingle->init(string);
        connect(vkAudioWindowsSingle,
                &QvkAudioWindowsSingle::signal_haveAudioDeviceSelected,
                this,
                [=](bool value){
            ui->labelAudioCodec->setEnabled(value);
            ui->comboBoxAudioCodec->setEnabled(value);
        });
        connect(this,
                &QvkAudioWindowsController::signal_haveAudioDeviceSelected,
                this,
                [=](bool value){
            ui->labelAudioCodec->setEnabled(value);
            ui->comboBoxAudioCodec->setEnabled(value);
        });
    }

    if (action == "[Audio-device-removed]"){
        // Audio Levelmeter ausschalten da das Programm sonst abstürtzt.
        QList<QToolButton *> list = ui->scrollAreaAudioDevice->findChildren<QToolButton *>();
        for(int i = 0; i < list.count(); i++){
            QToolButton *toolButton = list.at(i);
            if(toolButton->objectName().section("--", 1, 1) == device){
                if (toolButton->isChecked()){
                    toolButton->click();
                }
            }
        }

        QList<QvkAudioWindowsSingle *> listQvkAudioWindowsSingle = ui->scrollAreaAudioDevice->findChildren<QvkAudioWindowsSingle *>();
        for(int i = 0; i < listQvkAudioWindowsSingle.count(); i++){
            QvkAudioWindowsSingle *vkAudioWindowsSingle = listQvkAudioWindowsSingle.at(i);
            if (vkAudioWindowsSingle->objectName().section("__", 1, 1) == deviceID){
                vkAudioWindowsSingle->vkAudioWindowsLevelMeter->deleteLater();
                vkAudioWindowsSingle->deleteLater();
                qDebug().noquote() << global::nameOutput << "[Audio][device removed]" << deviceID << description << device;
                break;
            }
        }
    }
    slot_audioDeviceSelected();
}
