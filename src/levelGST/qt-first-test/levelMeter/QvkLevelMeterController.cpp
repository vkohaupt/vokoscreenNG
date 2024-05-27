/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
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

#include "QvkLevelMeterController.h"
#include "QvkLevelMeter.h"
#include "global.h"

#include <QCheckBox>
#include <QProgressBar>
#include <QLineEdit>

QvkLevelMeterController::QvkLevelMeterController()
{
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}


void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{
    QString index = checkBox->objectName().right(2);
    if ( index == "00" ) {
        global::lineEdit_00 = new QLineEdit;
        global::lineEdit_00->setObjectName( "lineEdit_" + index );
        connect( global::lineEdit_00, SIGNAL( textChanged(QString) ), this, SLOT( slot_pluggedInOutDevice(QString) ) );
        qDebug() << ".........." << index;
    }
    if ( index == "01" ) {
        global::lineEdit_01 = new QLineEdit;
        global::lineEdit_01->setObjectName( "lineEdit_" + index );
        connect( global::lineEdit_01, SIGNAL( textChanged(QString) ), this, SLOT( slot_pluggedInOutDevice(QString) ) );
        qDebug() << ".........." << index;
    }
    if ( index == "02" ) {
        global::lineEdit_02 = new QLineEdit;
        global::lineEdit_02->setObjectName( "lineEdit_" + index );
        connect( global::lineEdit_02, SIGNAL( textChanged(QString) ), this, SLOT( slot_pluggedInOutDevice(QString) ) );
        qDebug() << ".........." << index;
    }
    if ( index == "03" ) {
        global::lineEdit_03 = new QLineEdit;
        global::lineEdit_03->setObjectName( "lineEdit_" + index );
        connect( global::lineEdit_03, SIGNAL( textChanged(QString) ), this, SLOT( slot_pluggedInOutDevice(QString) ) );
        qDebug() << ".........." << index;
    }

    setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
    setFixedHeight(4);
    setTextVisible(false);
    setMinimum(0);
    setMaximum(10000);
    setToolTip(checkBox->text());

    layout->addWidget( this );

    QvkLevelMeter *vkLevelMeter = new QvkLevelMeter;
    QString device = checkBox->accessibleName().section( ":::", 0, 0);
    vkLevelMeter->start( device, index );
}


void QvkLevelMeterController::remove_ProgressBar()
{
}
