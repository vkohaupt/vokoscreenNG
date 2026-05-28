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
#include "QvkCameraSurface_wl.h"
#include "QvkCameraSingle_wl.h"
#include "QvkSettings_wl.h"

#include "ui_QvkCameraSingle_wl.h"

#include <QDebug>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QList>
#include <QLineEdit>
#include <QPushButton>
#include <QCamera>
#include <QCameraDevice>
#include <QCameraFormat>
#include <QMediaDevices>
#include <QVideoSink>
#include <QVideoFrame>
#include <QVideoFrameFormat>
#include <QMediaCaptureSession>
#include <QCheckBox>
#include <QComboBox>

/*
 *
 *            QvkCameraController_wl
 *    (Im Controller wird der Watcher und die
 *     Instanzen jeder Camera mit Suface gemanagt)
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
            [this](QString device){slot_camera_added_or_removed(device);});

    set_surface();

    vkCameraWatcher_wl->init();
}


void QvkCameraController_wl::slot_camera_added_or_removed( QString device )
{
    if(device.contains("added")){
        QvkCameraSingle_wl *vkCameraSingle_wl = new QvkCameraSingle_wl;

        vkCameraSingle_wl->ui->comboBoxCameraFPS->hide();

        vkCameraSingle_wl->setObjectName("cameraSingleVideoID_" + device.section(":::", 0, 0)); // Im ObjectName steckt die id
        vkCameraSingle_wl->ui->checkBoxCameraOnOff->setText(device.section(":::", 1, 1 ));
        vkCameraSingle_wl->ui->checkBoxCameraOnOff->setObjectName("checkBoxCameraVideoID_" + device.section(":::", 0, 0));  // Im ObjectName steckt die id
        vkCameraSingle_wl->ui->comboBoxCameraPixelformat->setObjectName("comboBoxCameraPixelformatVideoID_" + device.section(":::", 0, 0));
        vkCameraSingle_wl->ui->comboBoxCameraResolution->setObjectName("comboBoxCameraResolutionVideoID_" + device.section(":::", 0, 0));
        vkCameraSingle_wl->ui->comboBoxCameraFPS->setObjectName("comboBoxCameraFPSVideoID_" + device.section(":::", 0, 0));
        // CheckBox und die ComboBoxen für Format und Auflösung werden nun der GUI hinzugefügt
        ui->verticalLayout_3->addWidget(vkCameraSingle_wl);

        connect(vkCameraSingle_wl->ui->checkBoxCameraOnOff, &QCheckBox::clicked, this, [=](bool value){
            vkCameraSurface_wl->is_setNewImageRect = false;
            slot_checkBoxCameraOnOff(value, vkCameraSingle_wl->ui->checkBoxCameraOnOff);
            emit signal_forSystrayCameraOnOff(vkCameraSingle_wl->ui->checkBoxCameraOnOff);
        });

        emit signal_forSystrayCameraAdded(vkCameraSingle_wl->ui->checkBoxCameraOnOff);

        connect(vkCameraSingle_wl->ui->comboBoxCameraResolution, &QComboBox::currentIndexChanged, this, [=](int value){
            Q_UNUSED(value)
            if(vkCameraSingle_wl->ui->checkBoxCameraOnOff->isChecked() == true){
                // checkBoxCameraOnOff ist on
                // Ok funktioniert
                // Wenn von kleiner Auflösung auf sehr große Auflösung umgeschaltet wird
                // sieht man den Kreis nicht da er ganz nach rechts aus dem Bildschirm rutscht
                // Dieses Problem muß noch behoben werden
                vkCameraSingle_wl->ui->checkBoxCameraOnOff->click();
                vkCameraSurface_wl->is_setNewImageRect = false;
                vkCameraSingle_wl->ui->checkBoxCameraOnOff->click();
            }
        });

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
                        vkCameraSurface_wl->is_setNewImageRect = false;
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


        // nun die ComboxBox für die FPS ermitteln ...
        QList<QComboBox *> listComboBoxFPS = ui->centralwidget->findChildren<QComboBox *>();
        QComboBox *comboBoxFPS = NULL;
        for(int i = 0; i < listComboBoxFPS.count(); i++){
            comboBoxFPS = listComboBoxFPS.at(i);
            if(comboBoxFPS->objectName() == QString("comboBoxCameraFPSVideoID_" + device.section(":::", 0, 0))){
                break;
            }
        }
        // und die FPS ermitteln und in die ComboBox stellen
        for(int i = 0; i < cameraDevice.videoFormats().count(); i++){
            QCameraFormat videoFormat = cameraDevice.videoFormats().at(i);
            QString minFPS = QString::number(videoFormat.minFrameRate());
            QString maxFPS = QString::number(videoFormat.maxFrameRate());
            comboBoxFPS->addItem(maxFPS);
        }

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


void QvkCameraController_wl::slot_checkBoxCameraOnOff(bool checked, QCheckBox *checkBoxCameraOnOff)
{
    // Disable all other cameras
    if ( checked == true ){
        QList<QCheckBox *> listCheckBox = ui->centralwidget->findChildren<QCheckBox *>();
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( checkBox->objectName().contains("checkBoxCameraVideoID_") ){
                if ( checkBox->isChecked() == false ){
                    checkBox->setDisabled(true);
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
                    checkBox->setEnabled(true);
                }
            }
        }
    }

    if ( checked == true ){
        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for ( int x = 0; x < cameras.count(); x++ ){
            QCameraDevice cameraDevice = cameras.at(x);
            if ( cameraDevice.id() == checkBoxCameraOnOff->objectName().section("_", 1, 1) ){
                camera = new QCamera(cameraDevice);
                QVideoSink *videoSink = new QVideoSink;
                connect(videoSink,
                        &QVideoSink::videoFrameChanged,
                        vkCameraSurface_wl,
                        [this](QVideoFrame videoFrame){
                    vkCameraSurface_wl->slot_setCameraImage(videoFrame);
                });

                // ComboxBox für die Formate YUYV JPEG etc. ermitteln ...
                QList<QComboBox *> listComboBoxPixelformat = ui->centralwidget->findChildren<QComboBox *>();
                QComboBox *comboBoxPixelformat = NULL;
                for(int i = 0; i < listComboBoxPixelformat.count(); i++){
                    comboBoxPixelformat = listComboBoxPixelformat.at(i);
                    if(comboBoxPixelformat->objectName() == QString("comboBoxCameraPixelformatVideoID_" + checkBoxCameraOnOff->objectName().section("_", 1, 1))){
                        break;
                    }
                }

                // ComboxBox für die Auflösungen ermitteln ...
                QList<QComboBox *> listComboBoxResolution = ui->centralwidget->findChildren<QComboBox *>();
                QComboBox *comboBoxResolution = NULL;
                for(int i = 0; i < listComboBoxResolution.count(); i++){
                    comboBoxResolution = listComboBoxResolution.at(i);
                    if(comboBoxResolution->objectName() == QString("comboBoxCameraResolutionVideoID_" + checkBoxCameraOnOff->objectName().section("_", 1, 1))){
                        break;
                    }
                }
                // und hier die Auflösung aus der ComboBox lesen und an der Kamera setzen
                QVariant variantData = comboBoxResolution->currentData();
                QCameraFormat cameraFormat = variantData.value<QCameraFormat>();
                camera->setCameraFormat(cameraFormat);

                QMediaCaptureSession *captureSession = new QMediaCaptureSession;
                captureSession->setCamera(camera);
                captureSession->setVideoOutput(videoSink);
                qDebug().noquote() << global::nameOutput
                                   << "[Camera] Start with:"
                                   << cameraDevice.id()
                                   << cameraFormat.pixelFormat()
                                   << cameraFormat.resolution()
                                   << cameraFormat.minFrameRate()
                                   << cameraFormat.maxFrameRate();

                camera->start();
                vkCameraSurface_wl->show();
            }
        }
    }

    if ( checked == false ){
        camera->stop();
        qDebug().noquote() << global::nameOutput
                           << "[Camera] Stop:"
                           << checkBoxCameraOnOff->objectName().section("_", 1, 1);
        delete camera;
        camera = NULL;

        //Don´t make a vkCameraSurface_wl->close(). It is not good, if close the window from taskbar
        vkCameraSurface_wl->hide();
    }
}


void QvkCameraController_wl::set_surface()
{
    // Surface wird einmal angelegt für alle Cameras
    if (vkCameraSurface_wl == NULL){
        vkCameraSurface_wl = new QvkCameraSurface_wl();

        QvkSettings_wl vkSettings_wl;
        QPoint point = vkSettings_wl.readCameraSurface();
        vkCameraSurface_wl->imageRect.setX(point.x());
        vkCameraSurface_wl->imageRect.setY(point.y());

        connect(ui->toolButton_camera_view_rectangle, &QToolButton::clicked, vkCameraSurface_wl, [=](){
            vkCameraSurface_wl->clearMask();
            vkCameraSurface_wl->repaint();
            vkCameraSurface_wl->setMask(vkCameraSurface_wl->pixmap.mask());
        });
        connect(ui->toolButton_camera_view_ellipse, &QToolButton::clicked, vkCameraSurface_wl, [=](){
            vkCameraSurface_wl->clearMask();
            vkCameraSurface_wl->repaint();
            vkCameraSurface_wl->setMask(vkCameraSurface_wl->pixmap.mask());
        });
        connect(ui->toolButton_camera_view_circle, &QToolButton::clicked, vkCameraSurface_wl, [=](){
            vkCameraSurface_wl->clearMask();
            vkCameraSurface_wl->repaint();
            vkCameraSurface_wl->setMask(vkCameraSurface_wl->pixmap.mask());
        });

        vkCameraSurface_wl->set_toolButtonRectangle(ui->toolButton_camera_view_rectangle);
        vkCameraSurface_wl->set_toolButtonElipse(ui->toolButton_camera_view_ellipse);
        vkCameraSurface_wl->set_toolButtonCircle(ui->toolButton_camera_view_circle);
        vkCameraSurface_wl->set_toolButtonCameraMirrorHorizontal(ui->toolButtonCameraMirrorHorizontal);
        vkCameraSurface_wl->set_toolButtonCameraMirrorVertical(ui->toolButtonCameraMirrorVertical);

        vkCameraSurface_wl->hide();
    }
}

/*
QVideoFrameFormat beinhaltet setFrameRate
// Beispiel:
QVideoFrameFormat format(QSize(1920, 1080), QVideoFrameFormat::Format_NV12);
format.setFrameRate(30.0);
QVideoFrame videoFrame(format);

QVideoframe wird am QVideoSink gesetzt
*/
