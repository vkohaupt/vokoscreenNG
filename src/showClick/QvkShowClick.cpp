/* vokoscreenNG
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
    setGlobalMouse();
    setSpezialSlider();
    setSpezialCheckBox();
    setToolButtonDefaultValues();
    setPreviewWidget();
}


void QvkShowClick::setColorButtons()
{
    QList<QHBoxLayout *> listLayaout = ui->gridLayout_showclick_color_pushButton->findChildren<QHBoxLayout *>();
    for ( int i = 0; i < listLayaout.count(); i++ ) {
        QHBoxLayout *boxLayout = listLayaout.at(i);
        for ( int x = 0; x < 20; x++ ) {
            if ( boxLayout->objectName().section( "_", 2, 2 ) == QVariant::fromValue( Qt::GlobalColor(x) ).toString() ) {
                QvkPushButton *vkPushButton = new QvkPushButton( Qt::GlobalColor(x) );
                vkPushButton->setObjectName( "PushButton_ShowClick_color_" + QVariant::fromValue( Qt::GlobalColor(x) ).toString() );
                vkPushButton->setMaximumHeight( 23 );
                boxLayout->addWidget( vkPushButton );
                connect( vkPushButton, &QPushButton::clicked, this, [=]() { vkPreviewWidget->setColor( Qt::GlobalColor(x) ); } );
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
    vkSpezialSliderDiameter->setBigHandel( false );
    connect( vkSpezialSliderDiameter, &QSlider::valueChanged, this, [=]( int value ) { vkPreviewWidget->setDiameter( value ); } );

    vkSpezialSliderOpacity = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutOpacity->insertWidget( 0, vkSpezialSliderOpacity );
    vkSpezialSliderOpacity->setObjectName( "sliderShowClickOpacity" );
    vkSpezialSliderOpacity->setTracking( true );
    vkSpezialSliderOpacity->setMinimum( 1 );
    vkSpezialSliderOpacity->setMaximum( 100 );
    vkSpezialSliderOpacity->setValue( 70 );
    vkSpezialSliderOpacity->setShowValue( false );
    vkSpezialSliderOpacity->show();
    vkSpezialSliderOpacity->setBigHandel( false );
    connect( vkSpezialSliderOpacity, &QSlider::valueChanged, this, [=]( int value ) { vkPreviewWidget->setOpacity( (double)value / 100 ); } );

    vkSpezialSliderShowtime = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayoutShowtime->insertWidget( 0, vkSpezialSliderShowtime );
    vkSpezialSliderShowtime->setObjectName( "sliderShowClickShowtime" );
    vkSpezialSliderShowtime->setTracking( true );
    vkSpezialSliderShowtime->setMinimum( 1 );
    vkSpezialSliderShowtime->setMaximum( 20 );
    vkSpezialSliderShowtime->setValue( 10 );
    vkSpezialSliderShowtime->setShowValue( false );
    vkSpezialSliderShowtime->show();
    vkSpezialSliderShowtime->setBigHandel( false );
}


void QvkShowClick::setSpezialCheckBox()
{
    vkSpezialCheckbox = new QvkSpezialCheckbox();
    vkSpezialCheckbox->setObjectName( "spezialCheckboxShowclick" );
    vkSpezialCheckbox->set_colorChecked( Qt::darkGreen );
    vkSpezialCheckbox->set_colorUnChecked( Qt::darkRed );
    vkSpezialCheckbox->set_imageChecked( ":/pictures/spezialCheckBox/on.png" );
    vkSpezialCheckbox->set_imageUnChecked( ":/pictures/spezialCheckBox/off.png" );
    vkSpezialCheckbox->set_waitBeforeTrigger( 0 );
    ui->horizontalLayout_15->insertWidget( 1, vkSpezialCheckbox );
    connect( vkSpezialCheckbox, SIGNAL( signal_clicked( bool ) ), vkGlobalMouse, SLOT( slot_on( bool ) ) );
}


void QvkShowClick::setToolButtonDefaultValues()
{
    connect( ui->toolButtonShowclickColorDefault,    &QPushButton::clicked, this, [=]() { vkPreviewWidget->setColor( colorDefault ); } );
    connect( ui->toolButtonShowclickDiameterDefault, &QPushButton::clicked, this, [=]() { vkSpezialSliderDiameter->setValue( diameterDefault ); } );
    connect( ui->toolButtonShowclickOpacityDefault,  &QPushButton::clicked, this, [=]() { vkSpezialSliderOpacity->setValue( opacityDefault ); } );
    connect( ui->toolButtonShowclickTimeDefault,     &QPushButton::clicked, this, [=]() { vkSpezialSliderShowtime->setValue( timeDefault ); } );
}


void QvkShowClick::setGlobalMouse()
{
    vkGlobalMouse = new QvkGlobalMouse();
    connect( vkGlobalMouse, SIGNAL( signal_mousePressed( int, int, QString ) ), this, SLOT( slot_mousePressed( int, int, QString ) ) );
}


void QvkShowClick::slot_mousePressed( int x, int y, QString mouseButton )
{
    Q_UNUSED(x)
    Q_UNUSED(y)
    QColor color;
    QList<QvkPushButton *> listPushButton = ui->centralWidget->findChildren<QvkPushButton *>();
    for ( int i = 0; i < listPushButton.count(); i++ ) {
        QPushButton *pushButton = listPushButton.at(i);
        if ( ( pushButton->underMouse() == true ) and ( pushButton->objectName().contains( "ShowClick_color" ) ) ) {
            QString objectNameColor = pushButton->objectName().section( "_", 3, 3);
            for ( int x = 0; x < 20; x++ ) {
                if ( QVariant::fromValue( Qt::GlobalColor(x) ).toString().contains( objectNameColor ) ) {
                    color = Qt::GlobalColor(x);
                    break;
                }
            }
            break;
        } else {
            color = vkPreviewWidget->getColor();
        }
    }

    vk_x = QCursor::pos().x();
    vk_y = QCursor::pos().y();
    vk_mouseButton = mouseButton;
    vk_color = color;

    // Under Gnome no action if closing the GUI or doubleclick.
    // We wait 200 ms and then show the showclick window.
    QTimer::singleShot( 200, Qt::PreciseTimer, this, SLOT( slot_animateWindow() ) );
}


void QvkShowClick::slot_animateWindow()
{

    int x = vk_x;
    int y = vk_y;
    QString mouseButton = vk_mouseButton;
    QColor color = vk_color;

    QvkAnimateWindow *vkAnimateWindow = new QvkAnimateWindow( this,
                                                             x,
                                                             y,
                                                             vkSpezialSliderShowtime->value() * 100,
                                                             mouseButton,
                                                             vkSpezialSliderDiameter->value(),
                                                             vkSpezialSliderOpacity->value(),
                                                             color );
    vkAnimateWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
    vkAnimateWindow->show();
}


void QvkShowClick::setPreviewWidget()
{
    vkPreviewWidget = new QvkPreviewWidget( ui->frame_3 );
    vkPreviewWidget->setObjectName( "widgetCircle_1" );
    vkPreviewWidget->show();
}
