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
    setCircleWidget();
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
    vkCircleWidget_1->setColor( Qt::white );
    vkCircleWidget_2->setColor( Qt::white );
    vkCircleWidget_3->setColor( Qt::white );
}

void QvkShowClick::slot_vkPushButton_black()
{
    vkCircleWidget_1->setColor( Qt::black );
    vkCircleWidget_2->setColor( Qt::black );
    vkCircleWidget_3->setColor( Qt::black );
}

void QvkShowClick::slot_vkPushButton_red()
{
    vkCircleWidget_1->setColor( Qt::red );
    vkCircleWidget_2->setColor( Qt::red );
    vkCircleWidget_3->setColor( Qt::red );
}

void QvkShowClick::slot_vkPushButton_darkRed()
{
    vkCircleWidget_1->setColor( Qt::darkRed );
    vkCircleWidget_2->setColor( Qt::darkRed );
    vkCircleWidget_3->setColor( Qt::darkRed );
}

void QvkShowClick::slot_vkPushButton_green()
{
    vkCircleWidget_1->setColor( Qt::green );
    vkCircleWidget_2->setColor( Qt::green );
    vkCircleWidget_3->setColor( Qt::green );
}

void QvkShowClick::slot_vkPushButton_darkGreen()
{
    vkCircleWidget_1->setColor( Qt::darkGreen );
    vkCircleWidget_2->setColor( Qt::darkGreen );
    vkCircleWidget_3->setColor( Qt::darkGreen );
}

void QvkShowClick::slot_vkPushButton_blue()
{
    vkCircleWidget_1->setColor( Qt::blue );
    vkCircleWidget_2->setColor( Qt::blue );
    vkCircleWidget_3->setColor( Qt::blue );
}

void QvkShowClick::slot_vkPushButton_darkBlue()
{
    vkCircleWidget_1->setColor( Qt::darkBlue );
    vkCircleWidget_2->setColor( Qt::darkBlue );
    vkCircleWidget_3->setColor( Qt::darkBlue );
}

void QvkShowClick::slot_vkPushButton_cyan()
{
    vkCircleWidget_1->setColor( Qt::cyan );
    vkCircleWidget_2->setColor( Qt::cyan );
    vkCircleWidget_3->setColor( Qt::cyan );
}

void QvkShowClick::slot_vkPushButton_darkCyan()
{
    vkCircleWidget_1->setColor( Qt::darkCyan );
    vkCircleWidget_2->setColor( Qt::darkCyan );
    vkCircleWidget_3->setColor( Qt::darkCyan );
}

void QvkShowClick::slot_vkPushButton_magenta()
{
    vkCircleWidget_1->setColor( Qt::magenta );
    vkCircleWidget_2->setColor( Qt::magenta );
    vkCircleWidget_3->setColor( Qt::magenta );
}

void QvkShowClick::slot_vkPushButton_darkMagenta()
{
    vkCircleWidget_1->setColor( Qt::darkMagenta );
    vkCircleWidget_2->setColor( Qt::darkMagenta );
    vkCircleWidget_3->setColor( Qt::darkMagenta );
}

void QvkShowClick::slot_vkPushButton_yellow()
{
    vkCircleWidget_1->setColor( Qt::yellow );
    vkCircleWidget_2->setColor( Qt::yellow );
    vkCircleWidget_3->setColor( Qt::yellow );
}

void QvkShowClick::slot_vkPushButton_darkYellow()
{
    vkCircleWidget_1->setColor( Qt::darkYellow );
    vkCircleWidget_2->setColor( Qt::darkYellow );
    vkCircleWidget_3->setColor( Qt::darkYellow );
}

void QvkShowClick::slot_vkPushButton_gray()
{
    vkCircleWidget_1->setColor( Qt::gray );
    vkCircleWidget_2->setColor( Qt::gray );
    vkCircleWidget_3->setColor( Qt::gray );
}

void QvkShowClick::slot_vkPushButton_darkGray()
{
    vkCircleWidget_1->setColor( Qt::darkGray );
    vkCircleWidget_2->setColor( Qt::darkGray );
    vkCircleWidget_3->setColor( Qt::darkGray );
}


void QvkShowClick::setSpezialSlider()
{
    vkSpezialSliderCircle = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutCircle->insertWidget( 0, vkSpezialSliderCircle );
    vkSpezialSliderCircle->setObjectName( "sliderCircle" );
    vkSpezialSliderCircle->setTracking( true );
    vkSpezialSliderCircle->setMinimum( 40 );
    vkSpezialSliderCircle->setMaximum( 80 );
    vkSpezialSliderCircle->setValue( 50 );
    vkSpezialSliderCircle->setShowValue( false );
    vkSpezialSliderCircle->show();
    connect( vkSpezialSliderCircle, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSliderCircle( int ) ) );

    vkSpezialSliderOpacity = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutOpacity->insertWidget( 0, vkSpezialSliderOpacity );
    vkSpezialSliderOpacity->setObjectName( "sliderOpacity" );
    vkSpezialSliderOpacity->setTracking( true );
    vkSpezialSliderOpacity->setMinimum( 1 );
    vkSpezialSliderOpacity->setMaximum( 100 );
    vkSpezialSliderOpacity->setValue( 70 );
    vkSpezialSliderOpacity->setShowValue( false );
    vkSpezialSliderOpacity->show();
    connect( vkSpezialSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSliderOpacity( int ) ) );

    vkSpezialSliderShowtime = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutShowtime->insertWidget( 0, vkSpezialSliderShowtime );
    vkSpezialSliderShowtime->setObjectName( "sliderShowtime" );
    vkSpezialSliderShowtime->setTracking( true );
    vkSpezialSliderShowtime->setMinimum( 1 );
    vkSpezialSliderShowtime->setMaximum( 20 );
    vkSpezialSliderShowtime->setValue( 10 );
    vkSpezialSliderShowtime->setShowValue( false );
    vkSpezialSliderShowtime->show();
}


void QvkShowClick::setCircleWidget()
{
    vkCircleWidget_1 = new QvkCircleWidget( ui->widget_1 );
    vkCircleWidget_1->setObjectName( "widgetCircle_1" );
    vkCircleWidget_1->show();

    vkCircleWidget_2 = new QvkCircleWidget( ui->widget_2 );
    vkCircleWidget_2->setObjectName( "widgetCircle_2" );
    vkCircleWidget_2->show();

    vkCircleWidget_3 = new QvkCircleWidget( ui->widget_3 );
    vkCircleWidget_3->setObjectName( "widgetCircle_3" );
    vkCircleWidget_3->show();
}


void QvkShowClick::slot_valueChanged_SpezialSliderCircle( int value )
{
    vkCircleWidget_1->setDiameter( value );
    vkCircleWidget_2->setDiameter( value );
    vkCircleWidget_3->setDiameter( value );
}


void QvkShowClick::slot_valueChanged_SpezialSliderOpacity( int value )
{
    double value_1 = value;
    vkCircleWidget_1->setOpacity( value_1 / 100 );
    vkCircleWidget_2->setOpacity( value_1 / 100 );
    vkCircleWidget_3->setOpacity( value_1 / 100 );
}

void QvkShowClick::setToolButtonDefaultValues()
{
    connect( ui->toolButtonSliderCircleDefault,  SIGNAL( clicked() ), this, SLOT( slot_SliderCircleDefault() ) );
    connect( ui->toolButtonSliderOpacityDefault, SIGNAL( clicked() ), this, SLOT( slot_SliderOpacityDefault() ) );
    connect( ui->toolButtonShowTimeDefault,      SIGNAL( clicked() ), this, SLOT( slot_SlidershowTimeDefault() ) );
}


void QvkShowClick::slot_SliderCircleDefault()
{
    vkSpezialSliderCircle->setValue( 50 );
}


void QvkShowClick::slot_SliderOpacityDefault()
{
    vkSpezialSliderOpacity->setValue( 70 );
}


void QvkShowClick::slot_SlidershowTimeDefault()
{
    vkSpezialSliderShowtime->setValue( 10 );
}


void QvkShowClick::setSpezialCheckBox()
{
    vkSpezialCheckbox = new QvkSpezialCheckbox();
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
    animateWindow->init( x, y, vkSpezialSliderShowtime->value() * 100, mouseButton, vkSpezialSliderCircle->value(), vkSpezialSliderOpacity->value(), vkCircleWidget_1->getColor() );
}

