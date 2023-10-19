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
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QList>

QvkCameraController::QvkCameraController( Ui_formMainWindow *ui_surface )
{
    ui = ui_surface;

    ui->checkBoxCameraOnOff->hide();
    ui->comboBoxCamera->hide();
    ui->comboBoxCameraResolution->hide();
    ui->labelCameraWindowSize->hide();
    ui->toolButton_camera_view_rectangle->hide();
    ui->toolButton_camera_view_ellipse->hide();
    ui->toolButton_camera_view_circle->hide();
    ui->checkBoxCameraWindowFrame->hide();
    ui->checkBoxCameraMirrorHorizontal->hide();
    ui->checkBoxCameraMirrorVertical->hide();
    ui->checkBoxCameraInvert->hide();
    ui->checkBoxCameraGray->hide();
    ui->checkBoxCameraMono->hide();

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

    // Hide Widget if only one camera
    if ( camerasInfoList.count() == 1 ) {
        vkCameraSingle->radioButtonCamera->hide();
        labelCurrentCamera->hide();
    }
}


QvkCameraController::~QvkCameraController()
{
}
