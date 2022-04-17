/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "ui_camerasettingsdialog.h"

#include "camerasettingsdialog.h"
#include "global.h"

cameraSettingsDialog::cameraSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cameraSettingsDialog)
{
    setModal( true );
    setWindowFlags( Qt::Window | Qt::WindowStaysOnTopHint | Qt::Tool );
    ui->setupUi(this);

    setWindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
    setWindowTitle( tr( "Camera settings" ) );

    dialog_sliderCameraWindowSize = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_3->insertWidget( 1, dialog_sliderCameraWindowSize );
    dialog_sliderCameraWindowSize->setObjectName( "sliderCameraWindowSize" );
    dialog_sliderCameraWindowSize->setMinimum( 0 );
    dialog_sliderCameraWindowSize->setMaximum( 3000 );
    dialog_sliderCameraWindowSize->setValue( 0 );
    dialog_sliderCameraWindowSize->show();
    dialog_sliderCameraWindowSize->setShowValue( true );

    dialog_sliderCameraWindowZoom = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_2->insertWidget( 1, dialog_sliderCameraWindowZoom );
    dialog_sliderCameraWindowZoom->setObjectName( "sliderCameraWindowZoom" );
    dialog_sliderCameraWindowZoom->setMinimum( 0 );
    dialog_sliderCameraWindowZoom->setMaximum( 3000 );
    dialog_sliderCameraWindowZoom->setValue( 0 );
    dialog_sliderCameraWindowZoom->show();
    dialog_sliderCameraWindowZoom->setShowValue( true );

}

cameraSettingsDialog::~cameraSettingsDialog()
{
    delete ui;
}


