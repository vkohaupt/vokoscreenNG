/* vokoscreenNG
 * Copyright (C) 2017-2021 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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

#include "QvkShowClick.h"
#include "QvkPushButton.h"
#include "QvkAnimateWindow.h"

#include <QDebug>

QvkShowClick::QvkShowClick()
{
}


void QvkShowClick::init( Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;

    setColorButtons();
    setSpezialSlider();
    setSpezialCheckBox();
    setToolButtonDefaultValues();
    setMouseClick();
    setPreviewWidgets();
}


void QvkShowClick::setColorButtons()
{
    int minimumHeight = 34;
    QvkPushButton *vkPushButton_white = new QvkPushButton( Qt::white );
    vkPushButton_white->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_white->addWidget( vkPushButton_white );
    connect( vkPushButton_white, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_white() ) );

    QvkPushButton *vkPushButton_black = new QvkPushButton( Qt::black );
    vkPushButton_black->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_black->addWidget( vkPushButton_black );
    connect( vkPushButton_black, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_black() ) );

    QvkPushButton *vkPushButton_red = new QvkPushButton( Qt::red );
    vkPushButton_red->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_red->addWidget( vkPushButton_red );
    connect( vkPushButton_red, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_red() ) );

    QvkPushButton *vkPushButton_darkRed = new QvkPushButton( Qt::darkRed );
    vkPushButton_darkRed->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkRed->addWidget( vkPushButton_darkRed );
    connect( vkPushButton_darkRed, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkRed() ) );

    QvkPushButton *vkPushButton_green = new QvkPushButton( Qt::green );
    vkPushButton_green->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_green->addWidget( vkPushButton_green );
    connect( vkPushButton_green, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_green() ) );

    QvkPushButton *vkPushButton_darkGreen = new QvkPushButton( Qt::darkGreen );
    vkPushButton_darkGreen->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkGreen->addWidget( vkPushButton_darkGreen );
    connect( vkPushButton_darkGreen, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkGreen() ) );

    QvkPushButton *vkPushButton_blue = new QvkPushButton( Qt::blue );
    vkPushButton_blue->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_blue->addWidget( vkPushButton_blue );
    connect( vkPushButton_blue, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_blue() ) );

    QvkPushButton *vkPushButton_darkBlue = new QvkPushButton( Qt::darkBlue );
    vkPushButton_darkBlue->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkBlue->addWidget( vkPushButton_darkBlue );
    connect( vkPushButton_darkBlue, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkBlue() ) );

    QvkPushButton *vkPushButton_cyan = new QvkPushButton( Qt::cyan );
    vkPushButton_cyan->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_cyan->addWidget( vkPushButton_cyan );
    connect( vkPushButton_cyan, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_cyan() ) );

    QvkPushButton *vkPushButton_darkCyan = new QvkPushButton( Qt::darkCyan );
    vkPushButton_darkCyan->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkCyan->addWidget( vkPushButton_darkCyan );
    connect( vkPushButton_darkCyan, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkCyan() ) );

    QvkPushButton *vkPushButton_magenta = new QvkPushButton( Qt::magenta );
    vkPushButton_magenta->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_magenta->addWidget( vkPushButton_magenta );
    connect( vkPushButton_magenta, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_magenta() ) );

    QvkPushButton *vkPushButton_darkMagenta = new QvkPushButton( Qt::darkMagenta );
    vkPushButton_darkMagenta->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkMagenta->addWidget( vkPushButton_darkMagenta );
    connect( vkPushButton_darkMagenta, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkMagenta() ) );

    QvkPushButton *vkPushButton_yellow = new QvkPushButton( Qt::yellow );
    vkPushButton_yellow->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_yellow->addWidget( vkPushButton_yellow );
    connect( vkPushButton_yellow, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_yellow() ) );

    QvkPushButton *vkPushButton_darkYellow = new QvkPushButton( Qt::darkYellow );
    vkPushButton_darkYellow->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkYellow->addWidget( vkPushButton_darkYellow );
    connect( vkPushButton_darkYellow, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkYellow() ) );

    QvkPushButton *vkPushButton_gray = new QvkPushButton( Qt::gray );
    vkPushButton_gray->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_gray->addWidget( vkPushButton_gray );
    connect( vkPushButton_gray, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_gray() ) );

    QvkPushButton *vkPushButton_darkGray = new QvkPushButton( Qt::darkGray );
    vkPushButton_darkGray->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_darkGray->addWidget( vkPushButton_darkGray );
    connect( vkPushButton_darkGray, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_darkGray() ) );
}

void QvkShowClick::slot_vkPushButton_white()
{
    vkPreviewWidget_1->setColor( Qt::white );
    vkPreviewWidget_2->setColor( Qt::white );
    vkPreviewWidget_3->setColor( Qt::white );
}

void QvkShowClick::slot_vkPushButton_black()
{
    vkPreviewWidget_1->setColor( Qt::black );
    vkPreviewWidget_2->setColor( Qt::black );
    vkPreviewWidget_3->setColor( Qt::black );
}

void QvkShowClick::slot_vkPushButton_red()
{
    vkPreviewWidget_1->setColor( Qt::red );
    vkPreviewWidget_2->setColor( Qt::red );
    vkPreviewWidget_3->setColor( Qt::red );
}

void QvkShowClick::slot_vkPushButton_darkRed()
{
    vkPreviewWidget_1->setColor( Qt::darkRed );
    vkPreviewWidget_2->setColor( Qt::darkRed );
    vkPreviewWidget_3->setColor( Qt::darkRed );
}

void QvkShowClick::slot_vkPushButton_green()
{
    vkPreviewWidget_1->setColor( Qt::green );
    vkPreviewWidget_2->setColor( Qt::green );
    vkPreviewWidget_3->setColor( Qt::green );
}

void QvkShowClick::slot_vkPushButton_darkGreen()
{
    vkPreviewWidget_1->setColor( Qt::darkGreen );
    vkPreviewWidget_2->setColor( Qt::darkGreen );
    vkPreviewWidget_3->setColor( Qt::darkGreen );
}

void QvkShowClick::slot_vkPushButton_blue()
{
    vkPreviewWidget_1->setColor( Qt::blue );
    vkPreviewWidget_2->setColor( Qt::blue );
    vkPreviewWidget_3->setColor( Qt::blue );
}

void QvkShowClick::slot_vkPushButton_darkBlue()
{
    vkPreviewWidget_1->setColor( Qt::darkBlue );
    vkPreviewWidget_2->setColor( Qt::darkBlue );
    vkPreviewWidget_3->setColor( Qt::darkBlue );
}

void QvkShowClick::slot_vkPushButton_cyan()
{
    vkPreviewWidget_1->setColor( Qt::cyan );
    vkPreviewWidget_2->setColor( Qt::cyan );
    vkPreviewWidget_3->setColor( Qt::cyan );
}

void QvkShowClick::slot_vkPushButton_darkCyan()
{
    vkPreviewWidget_1->setColor( Qt::darkCyan );
    vkPreviewWidget_2->setColor( Qt::darkCyan );
    vkPreviewWidget_3->setColor( Qt::darkCyan );
}

void QvkShowClick::slot_vkPushButton_magenta()
{
    vkPreviewWidget_1->setColor( Qt::magenta );
    vkPreviewWidget_2->setColor( Qt::magenta );
    vkPreviewWidget_3->setColor( Qt::magenta );
}

void QvkShowClick::slot_vkPushButton_darkMagenta()
{
    vkPreviewWidget_1->setColor( Qt::darkMagenta );
    vkPreviewWidget_2->setColor( Qt::darkMagenta );
    vkPreviewWidget_3->setColor( Qt::darkMagenta );
}

void QvkShowClick::slot_vkPushButton_yellow()
{
    vkPreviewWidget_1->setColor( Qt::yellow );
    vkPreviewWidget_2->setColor( Qt::yellow );
    vkPreviewWidget_3->setColor( Qt::yellow );
}

void QvkShowClick::slot_vkPushButton_darkYellow()
{
    vkPreviewWidget_1->setColor( Qt::darkYellow );
    vkPreviewWidget_2->setColor( Qt::darkYellow );
    vkPreviewWidget_3->setColor( Qt::darkYellow );
}

void QvkShowClick::slot_vkPushButton_gray()
{
    vkPreviewWidget_1->setColor( Qt::gray );
    vkPreviewWidget_2->setColor( Qt::gray );
    vkPreviewWidget_3->setColor( Qt::gray );
}

void QvkShowClick::slot_vkPushButton_darkGray()
{
    vkPreviewWidget_1->setColor( Qt::darkGray );
    vkPreviewWidget_2->setColor( Qt::darkGray );
    vkPreviewWidget_3->setColor( Qt::darkGray );
}


void QvkShowClick::setSpezialSlider()
{
    vkSpezialSliderDiameter = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutCircle->insertWidget( 0, vkSpezialSliderDiameter );
    vkSpezialSliderDiameter->setObjectName( "sliderShowClickCircle" );
    vkSpezialSliderDiameter->setTracking( true );
    vkSpezialSliderDiameter->setMinimum( 40 );
    vkSpezialSliderDiameter->setMaximum( 80 );
    vkSpezialSliderDiameter->setValue( 50 );
    vkSpezialSliderDiameter->setShowValue( false );
    vkSpezialSliderDiameter->show();
    connect( vkSpezialSliderDiameter, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Diameter( int ) ) );

    vkSpezialSliderOpacity = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutOpacity->insertWidget( 0, vkSpezialSliderOpacity );
    vkSpezialSliderOpacity->setObjectName( "sliderShowClickOpacity" );
    vkSpezialSliderOpacity->setTracking( true );
    vkSpezialSliderOpacity->setMinimum( 1 );
    vkSpezialSliderOpacity->setMaximum( 100 );
    vkSpezialSliderOpacity->setValue( 70 );
    vkSpezialSliderOpacity->setShowValue( false );
    vkSpezialSliderOpacity->show();
    connect( vkSpezialSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Opacity( int ) ) );

    vkSpezialSliderShowtime = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutShowtime->insertWidget( 0, vkSpezialSliderShowtime );
    vkSpezialSliderShowtime->setObjectName( "sliderShowClickShowtime" );
    vkSpezialSliderShowtime->setTracking( true );
    vkSpezialSliderShowtime->setMinimum( 1 );
    vkSpezialSliderShowtime->setMaximum( 20 );
    vkSpezialSliderShowtime->setValue( 10 );
    vkSpezialSliderShowtime->setShowValue( false );
    vkSpezialSliderShowtime->show();
}


void QvkShowClick::setPreviewWidgets()
{
    vkPreviewWidget_1 = new QvkPreviewWidget( ui->widget_1 );
    vkPreviewWidget_1->setObjectName( "widgetCircle_1" );
    vkPreviewWidget_1->show();

    vkPreviewWidget_2 = new QvkPreviewWidget( ui->widget_2 );
    vkPreviewWidget_2->setObjectName( "widgetCircle_2" );
    vkPreviewWidget_2->show();

    vkPreviewWidget_3 = new QvkPreviewWidget( ui->widget_3 );
    vkPreviewWidget_3->setObjectName( "widgetCircle_3" );
    vkPreviewWidget_3->show();
}


void QvkShowClick::slot_valueChanged_SpezialSlider_Diameter( int value )
{
    vkPreviewWidget_1->setDiameter( value );
    vkPreviewWidget_2->setDiameter( value );
    vkPreviewWidget_3->setDiameter( value );
}


void QvkShowClick::slot_valueChanged_SpezialSlider_Opacity( int value )
{
    double value_1 = value;
    vkPreviewWidget_1->setOpacity( value_1 / 100 );
    vkPreviewWidget_2->setOpacity( value_1 / 100 );
    vkPreviewWidget_3->setOpacity( value_1 / 100 );
}

void QvkShowClick::setToolButtonDefaultValues()
{
    connect( ui->toolButtonShowclickDiameterDefault, SIGNAL( clicked() ), this, SLOT( slot_spezialSlider_Diameter_Default() ) );
    connect( ui->toolButtonShowclickOpacityDefault,  SIGNAL( clicked() ), this, SLOT( slot_spezialSlider_Opacity_Default() ) );
    connect( ui->toolButtonShowclickTimeDefault,     SIGNAL( clicked() ), this, SLOT( slot_spezialSlider_showTime_Default() ) );
}


void QvkShowClick::slot_spezialSlider_Diameter_Default()
{
    vkSpezialSliderDiameter->setValue( diameterDefault );
}


void QvkShowClick::slot_spezialSlider_Opacity_Default()
{
    vkSpezialSliderOpacity->setValue( opacityDefault );
}


void QvkShowClick::slot_spezialSlider_showTime_Default()
{
    vkSpezialSliderShowtime->setValue( timeDefault );
}


void QvkShowClick::setSpezialCheckBox()
{
    vkSpezialCheckbox = new QvkSpezialCheckbox();
    vkSpezialCheckbox->setObjectName( "spezialCheckboxShowclick" );
    ui->horizontalLayout_15->insertWidget( 1, vkSpezialCheckbox );
    connect( vkSpezialCheckbox, SIGNAL( signal_clicked( bool ) ), this, SLOT( slot_pointerOnOff( bool ) ) );
}


void QvkShowClick::slot_pointerOnOff( bool value )
{
    if ( value == true )
    {
        pointerOnOff = true;
        vkGlobalMouse->setCursorOn();
        vkGlobalMouse->mousePressed();
    }

    if ( value == false )
    {
        pointerOnOff = false;
        vkGlobalMouse->setCursorOff();
    }
}

void QvkShowClick::setMouseClick()
{
    vkGlobalMouse = new QvkGlobalMouse();
    connect( vkGlobalMouse, SIGNAL( signal_mousePressed( int, int, QString ) ), this, SLOT( slot_mousePressed( int, int, QString ) ) );
}


void QvkShowClick::slot_mousePressed( int x, int y, QString mouseButton )
{
    QvkAnimateWindow *animateWindow = new QvkAnimateWindow( this );
    animateWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
    animateWindow->init( x, y, vkSpezialSliderShowtime->value() * 100, mouseButton, vkSpezialSliderDiameter->value(), vkSpezialSliderOpacity->value(), vkPreviewWidget_1->getColor() );
}

