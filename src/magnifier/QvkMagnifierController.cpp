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

#include "QvkMagnifierController.h"
#include "QvkSpezialSlider.h"

QvkMagnifierController::QvkMagnifierController( Ui_formMainWindow *ui ) : vkMagnifier(new QvkMagnifier)
{
    QvkSpezialSlider *sliderMagnification = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_8->insertWidget( 0, sliderMagnification );
    sliderMagnification->setObjectName( "sliderMagnification" );
    sliderMagnification->setTracking( true );
    sliderMagnification->setMinimum( 1 );
    sliderMagnification->setMaximum( 3 );
    sliderMagnification->setPageStep( 1 );
    sliderMagnification->show();

    connect( ui->checkBoxMagnifier, SIGNAL( clicked( bool ) ),     vkMagnifier, SLOT( slot_magnifierShow( bool ) ) );
    connect( sliderMagnification,   SIGNAL( valueChanged( int ) ), this,        SLOT( slot_valueChanged( int ) ) );
    sliderMagnification->setValue( 2 );

    connect( ui->toolButton_magnifier_window, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonWindowClicked( bool ) ) );
    connect( ui->toolButton_magnifier_circle, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonCircleClicked( bool ) ) );
}


QvkMagnifierController::~QvkMagnifierController()
{
}


void QvkMagnifierController::slot_valueChanged( int value )
{
    if ( value == 1 ) {
       vkMagnifier->slot_magnifier200x200();
    }

    if ( value == 2 ) {
       vkMagnifier->slot_magnifier400x200();
    }

    if ( value == 3 ) {
       vkMagnifier->slot_magnifier600x200();
    }
}


void QvkMagnifierController::slot_toolButtonWindowClicked( bool value )
{
    vkMagnifier->isToolButtonWindow = value;
    vkMagnifier->isToolButtonCircle = false;
}


void QvkMagnifierController::slot_toolButtonCircleClicked( bool value )
{
    vkMagnifier->isToolButtonCircle = value;
    vkMagnifier->isToolButtonWindow = false;
}
