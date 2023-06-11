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

    sliderCameraWindowSize = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_45->insertWidget( 0, sliderCameraWindowSize );
    sliderCameraWindowSize->setObjectName( "sliderCameraWindowSize" );
    sliderCameraWindowSize->setMinimum( 0 );
    sliderCameraWindowSize->setMaximum( 1 );
    sliderCameraWindowSize->setValue( 0 );
    sliderCameraWindowSize->show();
    sliderCameraWindowSize->setShowValue( false );
    sliderCameraWindowSize->setBigHandel( true );
    sliderCameraWindowSize->setEnabled( true );

    sliderCameraWindowZoom = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_zoom->insertWidget( 0, sliderCameraWindowZoom );
    sliderCameraWindowZoom->setObjectName( "sliderCameraWindowZoom" );
    sliderCameraWindowZoom->setMinimum( 0 );
    sliderCameraWindowZoom->setMaximum( 1 );
    sliderCameraWindowZoom->setValue( 0 );
    sliderCameraWindowZoom->show();
    sliderCameraWindowZoom->setShowValue( true );
    sliderCameraWindowZoom->setEnabled( true );

    ui->checkBoxCameraOnOff->deleteLater();
    ui->comboBoxCamera->deleteLater();
    ui->comboBoxCameraResolution->deleteLater();

    // Create a vertical layout
    QVBoxLayout *layoutAllCameras = new QVBoxLayout;
    ui->horizontalLayout_63->addLayout( layoutAllCameras );
    layoutAllCameras->setObjectName( "layoutAllCameras" );

    // Existing widgets from the GUI will hidden
    ui->horizontalLayout_65->removeWidget( ui->toolButton_camera_view_rectangle );
    delete ui->toolButton_camera_view_rectangle;
    ui->horizontalLayout_65->removeWidget( ui->toolButton_camera_view_ellipse );
    delete ui->toolButton_camera_view_ellipse;
    ui->horizontalLayout_65->removeWidget( ui->toolButton_camera_view_circle );
    delete ui->toolButton_camera_view_circle;


//    ui->toolButton_camera_view_rectangle->hide();
//    ui->toolButton_camera_view_ellipse->hide();
//    ui->toolButton_camera_view_circle->hide();
    delete ui->checkBoxCameraWindowFrame;
    ui->checkBoxCameraMirrorHorizontal->hide();
    ui->checkBoxCameraMirrorVertical->hide();
    ui->checkBoxCameraInvert->hide();
    ui->checkBoxCameraGray->hide();
    ui->checkBoxCameraMono->hide();

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
