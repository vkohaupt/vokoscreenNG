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
#include <QHBoxLayout>

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
    setGlobalMouse();
    setPreviewWidget();
}


void QvkShowClick::setColorButtons()
{
    QList<QHBoxLayout *> listLayaout = ui->gridLayout_showclick_color_pushButton->findChildren<QHBoxLayout *>();
    for ( int i = 0; i < listLayaout.count(); i++ )
    {
        for ( int x = 0; x < 20; x++ )
        {
            if ( listLayaout.at(i)->objectName().section( "_", 2, 2 ) == QVariant::fromValue( Qt::GlobalColor(x) ).toString() )
            {
                QvkPushButton *vkPushButton = new QvkPushButton( Qt::GlobalColor(x) );
                vkPushButton->setObjectName( "PushButton_ShowClick_color_" + QVariant::fromValue( Qt::GlobalColor(x) ).toString() );
                vkPushButton->setMaximumHeight( 23 );
                listLayaout.at(i)->addWidget( vkPushButton );
                connect( vkPushButton, &QPushButton::clicked, [=](){ vkPreviewWidget->setColor( Qt::GlobalColor(x) ); } );
                break;
            }
        }
    }
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


void QvkShowClick::slot_valueChanged_SpezialSlider_Diameter( int value )
{
    vkPreviewWidget->setDiameter( value );
}


void QvkShowClick::slot_valueChanged_SpezialSlider_Opacity( int value )
{
    vkPreviewWidget->setOpacity( (double)value / 100 );
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


void QvkShowClick::setToolButtonDefaultValues()
{
    connect( ui->toolButtonShowclickColorDefault,    &QPushButton::clicked, [=] () { vkPreviewWidget->setColor( colorDefault ); } );
    connect( ui->toolButtonShowclickDiameterDefault, &QPushButton::clicked, [=] () { vkSpezialSliderDiameter->setValue( diameterDefault ); } );
    connect( ui->toolButtonShowclickOpacityDefault,  &QPushButton::clicked, [=] () { vkSpezialSliderOpacity->setValue( opacityDefault ); } );
    connect( ui->toolButtonShowclickTimeDefault,     &QPushButton::clicked, [=] () { vkSpezialSliderShowtime->setValue( timeDefault ); } );
}


void QvkShowClick::setGlobalMouse()
{
    vkGlobalMouse = new QvkGlobalMouse();
    connect( vkGlobalMouse, SIGNAL( signal_mousePressed( int, int, QString ) ), this, SLOT( slot_mousePressed( int, int, QString ) ) );
}


void QvkShowClick::slot_mousePressed( int x, int y, QString mouseButton )
{
    QColor color;
    QList<QvkPushButton *> listPushButton = ui->centralWidget->findChildren<QvkPushButton *>();
    for ( int i = 0; i < listPushButton.count(); i++ )
    {
        if ( ( listPushButton.at(i)->underMouse() == true ) and ( listPushButton.at(i)->objectName().contains( "ShowClick_color" ) ) )
        {
            QString objectNameColor = listPushButton.at(i)->objectName().section( "_", 3, 3);
            for ( int x = 0; x < 20; x++ )
            {
                if ( QVariant::fromValue( Qt::GlobalColor(x) ).toString().contains( objectNameColor ) )
                {
                    color = Qt::GlobalColor(x);
                    break;
                }
            }
            break;
        } else
        {
            color = vkPreviewWidget->getColor();
        }
    }

    QvkAnimateWindow *animateWindow = new QvkAnimateWindow( this );
    animateWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
    animateWindow->init( x, y, vkSpezialSliderShowtime->value() * 100, mouseButton, vkSpezialSliderDiameter->value(), vkSpezialSliderOpacity->value(), color );
}



void QvkShowClick::setPreviewWidget()
{
    vkPreviewWidget = new QvkPreviewWidget( ui->frame_3 );
    vkPreviewWidget->setObjectName( "widgetCircle_1" );
    vkPreviewWidget->show();
}
