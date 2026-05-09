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
#include <QMediaDevices>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMediaCaptureSession>
#include <QCheckBox>

QvkCameraController_wl::QvkCameraController_wl( Ui_formMainWindow_wl *ui_surface )
{
    ui = ui_surface;

    ui->checkBoxCameraOnOff->hide();
    ui->comboBoxCamera->hide();
    ui->comboBoxCameraResolution->hide();
    ui->help_camera_camera->hide();

    QvkCameraWatcher_wl *vkCameraWatcher_wl = new QvkCameraWatcher_wl;
    connect(vkCameraWatcher_wl,
            &QvkCameraWatcher_wl::signal_cameraChanged,
            this,
            [this](QString device){slot_camera_added_or_removed(device);});

    vkCameraWatcher_wl->init();
}


QvkCameraController_wl::~QvkCameraController_wl()
{
}


void QvkCameraController_wl::slot_camera_added_or_removed( QString device )
{
    if(device.contains("added")){
        QCheckBox *checkBoxCameraOnOff = new QCheckBox;
        checkBoxCameraOnOff->setText(device.section(":::", 1, 1 ));
        checkBoxCameraOnOff->setObjectName("checkBoxCameraVideoID_" + device.section(":::", 0, 0)); // Im ObjectName steckt die id
        ui->layoutAllCameras->insertWidget(0, checkBoxCameraOnOff);

        connect(checkBoxCameraOnOff, &QCheckBox::clicked, this, [=](bool value){
            slot_checkBoxCameraOnOff(value, checkBoxCameraOnOff);});
    }
}


void QvkCameraController_wl::slot_checkBoxCameraOnOff(bool checked, QCheckBox *checkBoxCameraOnOff)
{
    // Surface wird einmal angelegt für alle Cameras
    if (vkCameraSurface_wl == NULL){
        vkCameraSurface_wl = new QvkCameraSurface_wl();

        connect(vkCameraSurface_wl, &QvkCameraSurface_wl::signal_cameraSurfaceClose, this, [=](){checkBoxCameraOnOff->click();});

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
        vkCameraSurface_wl->set_checkBoxCameraMirrorHorizontal(ui->checkBoxCameraMirrorHorizontal);
        vkCameraSurface_wl->set_checkBoxCameraMirrorVertical(ui->checkBoxCameraMirrorVertical);
    }

    if ( checked == true ){
        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for ( int x = 0; x < cameras.count(); x++ ){
            QCameraDevice cameraDevice = cameras.at(x);
            if ( cameraDevice.id() == checkBoxCameraOnOff->objectName().section("_", 1, 1) ){
                qDebug() << cameraDevice.id() << checkBoxCameraOnOff->objectName().section("_", 1, 1);
                camera = new QCamera(cameraDevice);
                QVideoSink *videoSink = new QVideoSink;
                connect(videoSink,
                        &QVideoSink::videoFrameChanged,
                        vkCameraSurface_wl,
                        [this](QVideoFrame videoFrame){
                    vkCameraSurface_wl->slot_setCameraImage(videoFrame);
                });

                const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
                for ( int i = 0; i < cameraFormatList.count(); i++ ) {
                    //qDebug() << cameraFormatList.at(i).pixelFormat() << cameraFormatList.at(i).resolution();
/*
                    if ( cameraFormatList.at(i).pixelFormat() == comboBoxCameraVideoFormat->currentData() ) {
                        if ( cameraFormatList.at(i).resolution() == comboBoxCameraResolution->currentData() ) {
                            camera->setCameraFormat( cameraFormatList.at(i) );
                            QString width  = QString::number( cameraFormatList.at(i).resolution().width() );
                            QString height = QString::number( cameraFormatList.at(i).resolution().height() );
                            qDebug().noquote() << global::nameOutput
                                               << "[Camera] Start with format:"
                                               << cameraFormatList.at(i).pixelFormat()
                                               << "and resolution:"
                                               << width + "x" + height;
                        }
                    }
*/
                }

                camera->setCameraFormat(cameraFormatList.at(0));

                QMediaCaptureSession *captureSession = new QMediaCaptureSession;
                captureSession->setCamera(camera);
                captureSession->setVideoOutput(videoSink);
                camera->start();
                vkCameraSurface_wl->show();
            }
        }
    }

    if ( checked == false ){
        camera->stop();
        delete camera;
        camera = NULL;

        //Don´t make a vkCameraSurface_wl->close(). It is not good, if close the window from taskbar
        vkCameraSurface_wl->hide();
    }
}

/*
void QvkCameraController_wl::slot_checkBoxCameraOnOff( bool value )
{
    // Camera starten
    if ( value == true ) {
        camera = new QCamera( cameraDevice );
        connect( camera, SIGNAL( errorChanged() ), this, SLOT( slot_cameraError() ) );

        // Format und Resolution von Widget ermitteln und anwenden
        const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
        for ( int i = 0; i < cameraFormatList.count(); i++ ) {
            if ( cameraFormatList.at(i).pixelFormat() == comboBoxCameraVideoFormat->currentData() ) {
                if ( cameraFormatList.at(i).resolution() == comboBoxCameraResolution->currentData() ) {
                    camera->setCameraFormat( cameraFormatList.at(i) );
                    QString width  = QString::number( cameraFormatList.at(i).resolution().width() );
                    QString height = QString::number( cameraFormatList.at(i).resolution().height() );
                    qDebug().noquote() << global::nameOutput
                                       << "[Camera] Start with format:"
                                       << cameraFormatList.at(i).pixelFormat()
                                       << "and resolution:"
                                       << width + "x" + height;
                }
            }
        }

        videoSink = new QVideoSink;
        connect( videoSink, SIGNAL( videoFrameChanged( QVideoFrame ) ), this, SLOT( slot_videoFrameChanged( QVideoFrame ) ) );

        vkCameraWindow->show();

        captureSession = new QMediaCaptureSession;
        captureSession->setCamera( camera );
        captureSession->setVideoOutput( videoSink );

        camera->start();
    }

    // Camera stopen
    if ( value == false ) {
        disconnect( videoSink );
        delete videoSink;

        camera->stop();
        delete camera;
        camera = Q_NULLPTR;

        delete captureSession;

        vkCameraWindow->hide();
        qDebug().noquote() << global::nameOutput << "[Camera] Stop";
    }
}
*/
