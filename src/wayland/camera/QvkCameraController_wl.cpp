/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
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
#include "QvkCameraController_wl.h"
#include "QvkCameraWatcher_wl.h"
#include "QvkCameraSingle_wl.h"
#include "QvkSettings_wl.h"

#include "ui_QvkCameraSingle_wl.h"

#include <QDebug>
#include <QList>
#include <QCheckBox>

/*
 *
 *            QvkCameraController_wl
 *    (Im Controller wird der Watcher und die
 *     Instanzen jeder Camera mit Surface gemanagt)
 *                     |
 *         ------------------------
 *         |                      |
 * QvkCameraWatcher_wl   QvkCameraSingle_wl
 *                       (Im Singel wird eine einzelne Camera und
 *                        und ein einzelnes Surface gemanagt)
 *                                |
 *                       QvkCameraSurface_wl
 *
 */

QvkCameraController_wl::QvkCameraController_wl( Ui_formMainWindow_wl *ui_surface )
{
    ui = ui_surface;
    ui->help_camera_camera->hide();
}


QvkCameraController_wl::~QvkCameraController_wl()
{
}


void QvkCameraController_wl::init()
{
    QvkCameraWatcher_wl *vkCameraWatcher_wl = new QvkCameraWatcher_wl;
    connect(vkCameraWatcher_wl,
            &QvkCameraWatcher_wl::signal_cameraChanged,
            this,
            [this](QString device){slot_camera_added_or_removed(device);}
    );

    vkCameraWatcher_wl->init();
}


void QvkCameraController_wl::slot_camera_added_or_removed( QString device )
{
    if(device.contains("added")){
        vkCameraSingle_wl = new QvkCameraSingle_wl;
        vkCameraSingle_wl->set_GUIui(ui);
        vkCameraSingle_wl->set_objectName(device);
        ui->verticalLayout_3->addWidget(vkCameraSingle_wl);
        emit signal_forSystrayCameraAdded(vkCameraSingle_wl->ui->checkBoxCameraOnOff);

        // Kameras werden in der GUI disabled oder enabled
        connect( vkCameraSingle_wl,
                 &QvkCameraSingle_wl::signal_checkBoxCameraOnOff,
                 this,
                 [=](bool checked){slot_disableEnableCameras(checked);}
        );

        // Kamera On Off wird von der GUI weitergeleitet an Systray
        connect( vkCameraSingle_wl,
                 &QvkCameraSingle_wl::signal_forSystrayCameraOnOff,
                 this,
                 [=](QCheckBox *checkBox){ emit signal_forSystrayCameraOnOff(checkBox);}
        );

        //---------------------------------------------
        // Zuerst die Camera mithilfe der ID suchen diese befindet sich in cameraDevice.id() ...
        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        QCameraDevice cameraDevice;
        for(int x = 0; x < cameras.count(); x++){
            cameraDevice = cameras.at(x);
            if(cameraDevice.id() == device.section(":::", 0, 0)){
                break;
            }
        }

        // dann die dazugehörige Combobox für die Pixelformate ermitteln ...
        QList<QComboBox *> listComboBoxPixelformat = ui->centralwidget->findChildren<QComboBox *>();
        QComboBox *comboBoxPixelformat = NULL;
        for(int i = 0; i < listComboBoxPixelformat.count(); i++){
            comboBoxPixelformat = listComboBoxPixelformat.at(i);
            if(comboBoxPixelformat->objectName() == QString("comboBoxCameraPixelformatVideoID_" + device.section(":::", 0, 0))){
                connect(comboBoxPixelformat, &QComboBox::currentTextChanged, [=](){
                    if ( vkCameraSingle_wl->ui->checkBoxCameraOnOff->isChecked() == true ){
                        // checkBoxCameraOnOff ist on
                        // Ok funktioniert
                        vkCameraSingle_wl->ui->checkBoxCameraOnOff->click();
                        set_ResolutionOnComboBox(device, cameraDevice);
                        //vkCameraSurface_wl->is_setNewImageRect = false;
                        vkCameraSingle_wl->ui->checkBoxCameraOnOff->click();
                    }else{
                        // checkBoxCameraOnOff ist off
                        // Ok funktioniert
                        set_ResolutionOnComboBox(device, cameraDevice);
                    }
                });
                break;
            }
        }
        // und die Pixelformate wie zum Beispiel JPEG und YUYV in die Combobox stellen ...
        for(int i = 0; i < cameraDevice.videoFormats().count(); i++){
            QCameraFormat videoFormat = cameraDevice.videoFormats().at(i);
            QString format = QVideoFrameFormat::pixelFormatToString(videoFormat.pixelFormat()).toUpper();
            if(comboBoxPixelformat->findText(format) == -1){
                comboBoxPixelformat->addItem(format, videoFormat.pixelFormat());
            }
        }
        set_ResolutionOnComboBox(device, cameraDevice);
   }

    if(device.contains("removed")){
        // Camera ist aktiv und wurde entfernt
        QList<QCheckBox *> listCheckBox = ui->centralwidget->findChildren<QCheckBox *>();
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            QString str_1 = checkBox->objectName();
            QString str_2 = "checkBoxCameraVideoID_" + device.section(":::", 0, 0);
            if ( str_1 == str_2 ){
                if(checkBox->isChecked() == true){
                    checkBox->click();
                }
                break;
            }
        }

        QList<QvkCameraSingle_wl *> listCameraSingle = ui->centralwidget->findChildren<QvkCameraSingle_wl *>();
        for(int i = 0; i < listCameraSingle.count(); i++){
            QvkCameraSingle_wl *cameraSingle = listCameraSingle.at(i);
            if(cameraSingle->objectName() == QString("cameraSingleVideoID_" + device.section(":::", 0, 0 ))){
                delete cameraSingle;
            }
        }
        emit signal_forSystrayCameraRemoved(device);
    }
}


void QvkCameraController_wl::slot_disableEnableCameras(bool checked)
{
    // Disable all other cameras
    if ( checked == true ){
        QList<QCheckBox *> listCheckBox = ui->centralwidget->findChildren<QCheckBox *>();
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( checkBox->objectName().contains("checkBoxCameraVideoID_") ){
                if ( checkBox->isChecked() == false ){
                    checkBox->parentWidget()->setDisabled(true);
                }
            }
        }
    }

    // Enable all cameras
    if ( checked == false ){
        QList<QCheckBox *> listCheckBox = ui->centralwidget->findChildren<QCheckBox *>();
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( checkBox->objectName().contains("checkBoxCameraVideoID_") ){
                if ( checkBox->isChecked() == false ){
                    checkBox->parentWidget()->setEnabled(true);
                }
            }
        }
    }
}

void QvkCameraController_wl::set_ResolutionOnComboBox(QString device, QCameraDevice cameraDevice)
{
    // ComboBox für Pixelformate ermitteln
    QList<QComboBox *> listComboBoxPixelformat = ui->centralwidget->findChildren<QComboBox *>();
    QComboBox *comboBoxPixelformat = NULL;
    for(int i = 0; i < listComboBoxPixelformat.count(); i++){
        comboBoxPixelformat = listComboBoxPixelformat.at(i);
        if(comboBoxPixelformat->objectName() == QString("comboBoxCameraPixelformatVideoID_" + device.section(":::", 0, 0))){
            break;
        }
    }

    // ComboxBox für die Auflösungen ermitteln ...
    QList<QComboBox *> listComboBoxResolution = ui->centralwidget->findChildren<QComboBox *>();
    QComboBox *comboBoxResolution = NULL;
    for(int i = 0; i < listComboBoxResolution.count(); i++){
        comboBoxResolution = listComboBoxResolution.at(i);
        if(comboBoxResolution->objectName() == QString("comboBoxCameraResolutionVideoID_" + device.section(":::", 0, 0))){
            break;
        }
    }
    comboBoxResolution->clear();

    // dann die Auflösungen der Kamera ermitteln und in die ComboBox stellen
    for(int i = 0; i < cameraDevice.videoFormats().count(); i++){
        QCameraFormat cameraFormat = cameraDevice.videoFormats().at(i);
        QString width = QString::number(cameraFormat.resolution().width());
        QString height = QString::number(cameraFormat.resolution().height());
        QString resolution = width + " x " + height;
        QVariant variantData = QVariant::fromValue(cameraFormat);
        cameraFormat = variantData.value<QCameraFormat>();
        if(QVariant::fromValue(comboBoxPixelformat->currentData()) == cameraFormat.pixelFormat()){
            if(comboBoxResolution->findText(resolution) == -1){
                comboBoxResolution->addItem(comboBoxPixelformat->currentText() + " " + resolution, variantData);
            }
        }
    }
}
