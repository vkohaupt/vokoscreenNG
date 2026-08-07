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

QvkCameraController_wl::QvkCameraController_wl(Ui_formMainWindow_wl *m_GuiUi)
{
    GuiUi = m_GuiUi;
    GuiUi->help_camera_camera->hide();
    GuiUi->widgetCamera->hide();

    vkSliderCameraWindowSize = new QvkSpezialSlider(Qt::Horizontal);
    GuiUi->horizontalLayout_16->insertWidget(1, vkSliderCameraWindowSize);
    vkSliderCameraWindowSize->setObjectName("sliderCameraWindowSize");
    vkSliderCameraWindowSize->setInvertedAppearance(true);
    vkSliderCameraWindowSize->setInvertedControls(true);
    vkSliderCameraWindowSize->setMinimum(100);
    // Sehr hohe Vorgabe von "setMaximum" damit beim einlesen
    // aus den Settings auch hohe Werte richtig in der GUI angezeigt werden
    vkSliderCameraWindowSize->setMaximum(10000);
    vkSliderCameraWindowSize->setValue(100);
    vkSliderCameraWindowSize->show();
    vkSliderCameraWindowSize->setShowValue(true);
    vkSliderCameraWindowSize->setBigHandel(true);
    vkSliderCameraWindowSize->setEnabled(false);

    vkSliderCameraWindowZoom = new QvkSpezialSlider(Qt::Horizontal);
    GuiUi->horizontalLayout_6->insertWidget(1, vkSliderCameraWindowZoom);
    vkSliderCameraWindowZoom->setObjectName("sliderCameraWindowZoom");
    vkSliderCameraWindowZoom->setMinimum(0);
    vkSliderCameraWindowZoom->setMaximum(0);
    vkSliderCameraWindowZoom->setValue(0);
    vkSliderCameraWindowZoom->show();
    vkSliderCameraWindowZoom->setShowValue(true);
    vkSliderCameraWindowZoom->setEnabled(true);
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
    // Varibale device enthält zum Beispiel folgenden Inhalt "/dev/video1:::UVC Camera (046d:0809):::added"

    if(device.contains("added")){
        vkCameraSingle_wl = new QvkCameraSingle_wl;
        vkCameraSingle_wl->set_GUIui(GuiUi);
        vkCameraSingle_wl->set_init(device);
        vkCameraSingle_wl->set_PixelFormatOnComboBox(device.section(":::", 0, 0));
        GuiUi->verticalLayout_3->addWidget(vkCameraSingle_wl);
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

   }

    if(device.contains("removed")){
        // Camera ist aktiv und wurde entfernt
        QList<QCheckBox *> listCheckBox = GuiUi->centralwidget->findChildren<QCheckBox *>();
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

        QList<QvkCameraSingle_wl *> listCameraSingle = GuiUi->centralwidget->findChildren<QvkCameraSingle_wl *>();
        for(int i = 0; i < listCameraSingle.count(); i++){
            QvkCameraSingle_wl *cameraSingle = listCameraSingle.at(i);
            if(cameraSingle->objectName() == QString("cameraSingleVideoID_" + device.section(":::", 0, 0 ))){
                delete cameraSingle;
            }
        }
        emit signal_forSystrayCameraRemoved(device);
    }

    // Optionen ein bzw. ausblenden
    QList<QvkCameraSingle_wl *> listCameraSingle = GuiUi->centralwidget->findChildren<QvkCameraSingle_wl *>();
    if (listCameraSingle.count() > 0){
        GuiUi->widgetCamera->setVisible(true);
    }else{
        GuiUi->widgetCamera->setVisible(false);
    }
}


void QvkCameraController_wl::slot_disableEnableCameras(bool checked)
{
    // Disable all other cameras
    if ( checked == true ){
        QList<QCheckBox *> listCheckBox = GuiUi->centralwidget->findChildren<QCheckBox *>();
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
        QList<QCheckBox *> listCheckBox = GuiUi->centralwidget->findChildren<QCheckBox *>();
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
