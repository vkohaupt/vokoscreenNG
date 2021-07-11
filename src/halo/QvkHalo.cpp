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

#include "QvkHalo.h"
#include "QvkPushButton.h"


QvkHalo::QvkHalo()
{
}


QvkHalo::~QvkHalo()
{
}


void QvkHalo::init( Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;

    setSpezialSlider();
    setHaloPreviewWidget();
    setHaloWindow();
    setColorButtons();
    setToolButtonDefaultValues();
    setSpezialCheckBox();
    setTimer();
}


void QvkHalo::setSpezialSlider()
{
    vkSpezialSliderDiameter = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutDiameter->insertWidget( 0, vkSpezialSliderDiameter );
    vkSpezialSliderDiameter->setObjectName( "sliderhaloDiameter" );
    vkSpezialSliderDiameter->setTracking( true );
    vkSpezialSliderDiameter->setMinimum( 50 );
    vkSpezialSliderDiameter->setMaximum( 100 );
    vkSpezialSliderDiameter->setValue( 80 );
    vkSpezialSliderDiameter->setShowValue( false );
    vkSpezialSliderDiameter->show();
    connect( vkSpezialSliderDiameter, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Diameter( int ) ) );

    vkSpezialSliderOpacity = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutHaloOpacity->insertWidget( 0, vkSpezialSliderOpacity );
    vkSpezialSliderOpacity->setObjectName( "sliderhaloOpacity" );
    vkSpezialSliderOpacity->setTracking( true );
    vkSpezialSliderOpacity->setMinimum( 1 );
    vkSpezialSliderOpacity->setMaximum( 100 );
    vkSpezialSliderOpacity->setValue( 60 );
    vkSpezialSliderOpacity->setShowValue( false );
    vkSpezialSliderOpacity->show();
    connect( vkSpezialSliderOpacity, SIGNAL( valueChanged( int ) ), this, SLOT( slot_valueChanged_SpezialSlider_Opacity( int ) ) );
}


void QvkHalo::setHaloWindow()
{
    vkHaloWindow = new QvkHaloWindow( this );
    vkHaloWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip ); //With tooltip, no entry in Taskbar
}


void QvkHalo::setHaloPreviewWidget()
{
    vkHaloPreviewWidget = new QvkHaloPreviewWidget( ui->widget );
    vkHaloPreviewWidget->setObjectName( "widgetHaloPreview" );
    vkHaloPreviewWidget->show();
}


void QvkHalo::slot_valueChanged_SpezialSlider_Diameter( int value )
{
    vkHaloPreviewWidget->setDiameter( value );
    vkHaloWindow->setDiameter( value );
}


void QvkHalo::slot_valueChanged_SpezialSlider_Opacity( int value )
{
    double opacity = (double)value / 100.0;
    vkHaloPreviewWidget->setOpacity( opacity );
    vkHaloWindow->setOpacit( opacity );
}


void QvkHalo::setColorButtons()
{
    int minimumHeight = 34;
    QvkPushButton *vkPushButton_halo_white = new QvkPushButton( Qt::white );
    vkPushButton_halo_white->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_white->addWidget( vkPushButton_halo_white );
    connect( vkPushButton_halo_white, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_white() ) );

    QvkPushButton *vkPushButton_halo_black = new QvkPushButton( Qt::black );
    vkPushButton_halo_black->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_black->addWidget( vkPushButton_halo_black );
    connect( vkPushButton_halo_black, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_black() ) );

    QvkPushButton *vkPushButton_halo_red = new QvkPushButton( Qt::red );
    vkPushButton_halo_red->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_red->addWidget( vkPushButton_halo_red );
    connect( vkPushButton_halo_red, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_red() ) );

    QvkPushButton *vkPushButton_halo_darkRed = new QvkPushButton( Qt::darkRed );
    vkPushButton_halo_darkRed->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkRed->addWidget( vkPushButton_halo_darkRed );
    connect( vkPushButton_halo_darkRed, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkRed() ) );

    QvkPushButton *vkPushButton_halo_green = new QvkPushButton( Qt::green );
    vkPushButton_halo_green->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_green->addWidget( vkPushButton_halo_green );
    connect( vkPushButton_halo_green, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_green() ) );

    QvkPushButton *vkPushButton_halo_darkGreen = new QvkPushButton( Qt::darkGreen );
    vkPushButton_halo_darkGreen->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkGreen->addWidget( vkPushButton_halo_darkGreen );
    connect( vkPushButton_halo_darkGreen, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkGreen() ) );

    QvkPushButton *vkPushButton_halo_blue = new QvkPushButton( Qt::blue );
    vkPushButton_halo_blue->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_blue->addWidget( vkPushButton_halo_blue );
    connect( vkPushButton_halo_blue, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_blue() ) );

    QvkPushButton *vkPushButton_halo_darkBlue = new QvkPushButton( Qt::darkBlue );
    vkPushButton_halo_darkBlue->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkBlue->addWidget( vkPushButton_halo_darkBlue );
    connect( vkPushButton_halo_darkBlue, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkBlue() ) );

    QvkPushButton *vkPushButton_halo_cyan = new QvkPushButton( Qt::cyan );
    vkPushButton_halo_cyan->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_cyan->addWidget( vkPushButton_halo_cyan );
    connect( vkPushButton_halo_cyan, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_cyan() ) );

    QvkPushButton *vkPushButton_halo_darkCyan = new QvkPushButton( Qt::darkCyan );
    vkPushButton_halo_darkCyan->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkCyan->addWidget( vkPushButton_halo_darkCyan );
    connect( vkPushButton_halo_darkCyan, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkCyan() ) );

    QvkPushButton *vkPushButton_halo_magenta = new QvkPushButton( Qt::magenta );
    vkPushButton_halo_magenta->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_magenta->addWidget( vkPushButton_halo_magenta );
    connect( vkPushButton_halo_magenta, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_magenta() ) );

    QvkPushButton *vkPushButton_halo_darkMagenta = new QvkPushButton( Qt::darkMagenta );
    vkPushButton_halo_darkMagenta->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkMagenta->addWidget( vkPushButton_halo_darkMagenta );
    connect( vkPushButton_halo_darkMagenta, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkMagenta() ) );

    QvkPushButton *vkPushButton_halo_yellow = new QvkPushButton( Qt::yellow );
    vkPushButton_halo_yellow->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_yellow->addWidget( vkPushButton_halo_yellow );
    connect( vkPushButton_halo_yellow, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_yellow() ) );

    QvkPushButton *vkPushButton_halo_darkYellow = new QvkPushButton( Qt::darkYellow );
    vkPushButton_halo_darkYellow->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkYellow->addWidget( vkPushButton_halo_darkYellow );
    connect( vkPushButton_halo_darkYellow, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkYellow() ) );

    QvkPushButton *vkPushButton_halo_gray = new QvkPushButton( Qt::gray );
    vkPushButton_halo_gray->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_gray->addWidget( vkPushButton_halo_gray );
    connect( vkPushButton_halo_gray, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_gray() ) );

    QvkPushButton *vkPushButton_halo_darkGray = new QvkPushButton( Qt::darkGray );
    vkPushButton_halo_darkGray->setMinimumHeight( minimumHeight );
    ui->horizontalLayout_halo_darkGray->addWidget( vkPushButton_halo_darkGray );
    connect( vkPushButton_halo_darkGray, SIGNAL( clicked() ), this, SLOT( slot_vkPushButton_halo_darkGray() ) );
}


void QvkHalo::slot_vkPushButton_halo_white()
{
    vkHaloPreviewWidget->setColor( Qt::white );
    vkHaloWindow->setColor( Qt::white );
}


void QvkHalo::slot_vkPushButton_halo_black()
{
    vkHaloPreviewWidget->setColor( Qt::black );
    vkHaloWindow->setColor( Qt::black );
}


void QvkHalo::slot_vkPushButton_halo_red()
{
    vkHaloPreviewWidget->setColor( Qt::red );
    vkHaloWindow->setColor( Qt::red );
}


void QvkHalo::slot_vkPushButton_halo_darkRed()
{
    vkHaloPreviewWidget->setColor( Qt::darkRed );
    vkHaloWindow->setColor( Qt::darkRed );
}


void QvkHalo::slot_vkPushButton_halo_green()
{
    vkHaloPreviewWidget->setColor( Qt::green );
    vkHaloWindow->setColor( Qt::green );
}


void QvkHalo::slot_vkPushButton_halo_darkGreen()
{
    vkHaloPreviewWidget->setColor( Qt::darkGreen );
    vkHaloWindow->setColor( Qt::darkGreen );
}


void QvkHalo::slot_vkPushButton_halo_blue()
{
    vkHaloPreviewWidget->setColor( Qt::blue );
    vkHaloWindow->setColor( Qt::blue );
}


void QvkHalo::slot_vkPushButton_halo_darkBlue()
{
    vkHaloPreviewWidget->setColor( Qt::darkBlue );
    vkHaloWindow->setColor( Qt::darkBlue );
}


void QvkHalo::slot_vkPushButton_halo_cyan()
{
    vkHaloPreviewWidget->setColor( Qt::cyan );
    vkHaloWindow->setColor( Qt::cyan );
}


void QvkHalo::slot_vkPushButton_halo_darkCyan()
{
    vkHaloPreviewWidget->setColor( Qt::darkCyan );
    vkHaloWindow->setColor( Qt::darkCyan );
}


void QvkHalo::slot_vkPushButton_halo_magenta()
{
    vkHaloPreviewWidget->setColor( Qt::magenta );
    vkHaloWindow->setColor( Qt::magenta );
}


void QvkHalo::slot_vkPushButton_halo_darkMagenta()
{
    vkHaloPreviewWidget->setColor( Qt::darkMagenta );
    vkHaloWindow->setColor( Qt::darkMagenta );
}


void QvkHalo::slot_vkPushButton_halo_yellow()
{
    vkHaloPreviewWidget->setColor( Qt::yellow );
    vkHaloWindow->setColor( Qt::yellow );
}


void QvkHalo::slot_vkPushButton_halo_darkYellow()
{
    vkHaloPreviewWidget->setColor( Qt::darkYellow );
    vkHaloWindow->setColor( Qt::darkYellow );
}


void QvkHalo::slot_vkPushButton_halo_gray()
{
    vkHaloPreviewWidget->setColor( Qt::gray );
    vkHaloWindow->setColor( Qt::gray );
}


void QvkHalo::slot_vkPushButton_halo_darkGray()
{
    vkHaloPreviewWidget->setColor( Qt::darkGray );
    vkHaloWindow->setColor( Qt::darkGray );
}


void QvkHalo::setToolButtonDefaultValues()
{
    connect( ui->toolButtonDiameterDefault,  SIGNAL( clicked() ), this, SLOT( slot_DiameterDefault() ) );
    connect( ui->toolButtonOpacityDefault, SIGNAL( clicked() ), this, SLOT( slot_OpacityDefault() ) );
}

void QvkHalo::slot_DiameterDefault()
{
    vkSpezialSliderDiameter->setValue( 80 );
}


void QvkHalo::slot_OpacityDefault()
{
    vkSpezialSliderOpacity->setValue( 60 );
}


void QvkHalo::setSpezialCheckBox()
{
    vkSpezialCheckbox = new QvkSpezialCheckbox();
    vkSpezialCheckbox->setObjectName( "spezialCheckBoxHalo" );
    ui->horizontalLayout_48->insertWidget( 1, vkSpezialCheckbox );
    connect( vkSpezialCheckbox, SIGNAL( signal_clicked( bool ) ), this, SLOT( slot_haloOnOff( bool ) ) );
}


void QvkHalo::slot_haloOnOff( bool value )
{
    if ( value == true )
    {
        timer->start( 40 );
        vkHaloWindow->show();
    }

    if ( value == false )
    {
        timer->stop();
        vkHaloWindow->hide();
    }
}


void QvkHalo::setTimer()
{
    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), vkHaloWindow, SLOT( slot_followMouse() ) );
}
