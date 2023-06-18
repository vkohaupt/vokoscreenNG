/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2023 Volker Kohaupt
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

#include "QvkCameraController.h"
#include "global.h"

#include <QDebug>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QCameraFormat>
#include <QRadioButton>
#include <QList>
#include <QVideoFrame>
#include <QImage>

QvkCameraController::QvkCameraController( Ui_formMainWindow *ui_surface )
{
    ui = ui_surface;

    ui->checkBoxCameraOnOff->deleteLater();
    ui->comboBoxCamera->deleteLater();
    ui->comboBoxCameraResolution->deleteLater();
    ui->toolButton_camera_view_rectangle->deleteLater();
    ui->toolButton_camera_view_ellipse->deleteLater();
    ui->toolButton_camera_view_circle->deleteLater();
    ui->checkBoxCameraWindowFrame->deleteLater();
    ui->checkBoxCameraMirrorHorizontal->deleteLater();
    ui->checkBoxCameraMirrorVertical->deleteLater();
    ui->checkBoxCameraInvert->deleteLater();
    ui->checkBoxCameraGray->deleteLater();
    ui->checkBoxCameraMono->deleteLater();

    // SpacerItems aus GUI löschen
    for (int i = 0; i < ui->horizontalLayout_65->count(); ++i) {
        QLayoutItem *layoutItem = ui->horizontalLayout_65->itemAt(i);
        if (layoutItem->spacerItem()) {
            ui->horizontalLayout_65->removeItem( layoutItem );
            delete layoutItem;
            --i;
        }
    }

    // Create a vertical layout
    QVBoxLayout *layoutAllCameras = new QVBoxLayout;
    ui->horizontalLayout_63->addLayout( layoutAllCameras );
    layoutAllCameras->setObjectName( "layoutAllCameras" );

    // Add Devices
    const QList<QCameraDevice> camerasInfoList = QMediaDevices::videoInputs();
    for ( int i = 0; i < camerasInfoList.count(); i++ ) {
        vkCameraSingle = new QvkCameraSingle( ui, camerasInfoList.at(i) );
        buttonGroup->addButton( vkCameraSingle->radioButtonCamera );
    }

    // Am Ende ein spaceritem einfügen
    QSpacerItem *spacerItem = new QSpacerItem( 100,100, QSizePolicy::Expanding, QSizePolicy::Expanding );
    layoutAllCameras->addItem( spacerItem );

    // Dann ein Label für den Kameratext hinzufügen
    labelCurrentCamera->setObjectName( "labelCurrentCamera" );
    labelCurrentCamera->setAlignment( Qt::AlignHCenter );
    QFont font;
    font.setBold( true );
    labelCurrentCamera->setFont( font );
    layoutAllCameras->addWidget( labelCurrentCamera );
}


QvkCameraController::~QvkCameraController()
{
}
