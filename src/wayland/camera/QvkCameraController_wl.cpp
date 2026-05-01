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

// Wird vom Watcher aufgerufen
void QvkCameraController_wl::slot_camera_added_or_removed( QString device )
{
    if(device.contains("added")){
        QCheckBox *checkBox = new QCheckBox;
        checkBox->setText(device.section(":::", 1, 1 ));
        checkBox->setObjectName(device.section(":::", 0, 0)); // Objectname ist gleich die id
        ui->layoutAllCameras->addWidget(checkBox);

        connect(checkBox, &QCheckBox::clicked, this, [=](){

            if (checkBox->isChecked() == true){
                vkCameraSurface_wl = new QvkCameraSurface_wl();
                vkCameraSurface_wl->show();

                const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
                for ( int x = 0; x < cameras.count(); x++ ){
                    QCameraDevice cameraDevice = cameras.at(x);
                    if ( cameraDevice.id() == device.section(":::", 0, 0) ){
                        QCamera *camera = new QCamera(cameraDevice);
                        QVideoSink *videoSink = new QVideoSink;
                        connect(videoSink,
                                &QVideoSink::videoFrameChanged,
                                vkCameraSurface_wl,
                                [this](QVideoFrame videoFrame){
                            vkCameraSurface_wl->slot_setCameraImage(videoFrame);
                        });

                        QMediaCaptureSession *captureSession = new QMediaCaptureSession;
                        captureSession->setCamera( camera );
                        captureSession->setVideoOutput( videoSink );
                        camera->start();
                    }
                }
            }
            if (checkBox->isChecked() == false){
                //camera->stop();
            }
        });
    }

    if ( device.contains( "removed" ) ) {
        QList<QCheckBox *> listCheckBox = ui->centralwidget->findChildren<QCheckBox *>( "checkBoxCameraOnOff" );
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listCheckBox.at(i);
            if ( checkBox->accessibleName().section( ":::", 0, 0 ) == device.section( ":::", 0, 0 ) ) {
                ui->layoutAllCameras->removeWidget( checkBox );
                if ( checkBox->checkState() == Qt::Checked ) {
                    checkBox->click();
                }
                delete checkBox;
            }
        }
    }
}
