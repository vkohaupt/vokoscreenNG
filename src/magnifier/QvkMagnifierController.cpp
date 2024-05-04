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

#include <QDebug>

QvkMagnifierController::QvkMagnifierController( Ui_formMainWindow *myui ) : vkMagnifier(new QvkMagnifier)
{
    ui = myui;

    sliderMagnificationRectangle = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_rectangle->insertWidget( 0, sliderMagnificationRectangle );
    sliderMagnificationRectangle->setObjectName( "sliderMagnificationRectangle" );
    sliderMagnificationRectangle->setTracking( true );
    sliderMagnificationRectangle->setMinimum( 1 );
    sliderMagnificationRectangle->setMaximum( 3 );
    sliderMagnificationRectangle->setPageStep( 1 );
    sliderMagnificationRectangle->show();

    sliderMagnificationElipse = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_elipse->insertWidget( 0, sliderMagnificationElipse );
    sliderMagnificationElipse->setObjectName( "sliderMagnificationElipse" );
    sliderMagnificationElipse->setTracking( true );
    sliderMagnificationElipse->setMinimum( 2 );
    sliderMagnificationElipse->setMaximum( 3 );
    sliderMagnificationElipse->setPageStep( 1 );
    sliderMagnificationElipse->show();

    connect( ui->checkBoxMagnifier,         SIGNAL( clicked(bool) ),     vkMagnifier, SLOT( slot_magnifierShow(bool) ) );
    connect( ui->checkBoxMagnifier,         SIGNAL( clicked(bool) ),     this,        SLOT( slot_label_magnifier_on_screen_clear(bool) ) );
    connect( sliderMagnificationRectangle,  SIGNAL( valueChanged(int) ), this,        SLOT( slot_valueChangedRectangle(int) ) );
    sliderMagnificationRectangle->setValue( 2 );
    connect( sliderMagnificationElipse,     SIGNAL( valueChanged(int) ), this,        SLOT( slot_valueChangedElipse(int) ) );

    connect( ui->toolButton_magnifier_rectangle, SIGNAL( clicked(bool) ), this, SLOT( slot_toolButtonRectangleClicked(bool) ) );
    connect( ui->toolButton_magnifier_elipse,    SIGNAL( clicked(bool) ), this, SLOT( slot_toolButtonElipseClicked(bool) ) );
    connect( ui->toolButton_magnifier_circle,    SIGNAL( clicked(bool) ), this, SLOT( slot_toolButtonCircleClicked(bool) ) );

    connect( vkMagnifier, SIGNAL( signal_magnifier_on_screen(QScreen*) ), this, SLOT( slot_magnifier_on_screen(QScreen*) ) );
}


QvkMagnifierController::~QvkMagnifierController()
{
}


void  QvkMagnifierController::slot_magnifier_on_screen( QScreen *myScreen )
{
    QString str = myScreen->name().remove( "." ).remove( '\\' );
    ui->label_magnifier_on_screen->setText( str );
}


void QvkMagnifierController::slot_label_magnifier_on_screen_clear( bool value )
{
    if ( value == false ) {
        ui->label_magnifier_on_screen->clear();
    }
}


void QvkMagnifierController::slot_valueChangedRectangle( int value )
{
    if ( ui->toolButton_magnifier_rectangle->isChecked() == true )
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
}


void QvkMagnifierController::slot_valueChangedElipse( int value )
{
    if ( ui->toolButton_magnifier_elipse->isChecked() == true )
    {
        if ( value == 2 ) {
            vkMagnifier->slot_magnifier400x200();
        }

        if ( value == 3 ) {
            vkMagnifier->slot_magnifier600x200();
        }
    }
}


void QvkMagnifierController::slot_toolButtonRectangleClicked( bool value )
{
    vkMagnifier->isToolButtonRectangle = value;
    vkMagnifier->isToolButtonElipse = false;
    vkMagnifier->isToolButtonCircle = false;
    slot_valueChangedRectangle( sliderMagnificationRectangle->value() );
}


void QvkMagnifierController::slot_toolButtonElipseClicked( bool value )
{
    vkMagnifier->isToolButtonRectangle = false;
    vkMagnifier->isToolButtonElipse = value;
    vkMagnifier->isToolButtonCircle = false;
    slot_valueChangedElipse( sliderMagnificationElipse->value() );
}


void QvkMagnifierController::slot_toolButtonCircleClicked( bool value )
{
    vkMagnifier->isToolButtonRectangle = false;
    vkMagnifier->isToolButtonElipse = false;
    vkMagnifier->isToolButtonCircle = value;
    vkMagnifier->slot_magnifier200x200();
}
