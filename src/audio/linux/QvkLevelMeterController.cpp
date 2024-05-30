﻿/* vokoscreenNG - A desktop recorder
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
#include <QVBoxLayout>

QvkLevelMeterController::QvkLevelMeterController()
{
    maxSteps = 1000;
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}


void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{
    QString index = checkBox->objectName().right(2);

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setObjectName( "lineEditLevelMeter_" + index );
    global::listChildren->append( lineEdit );

    connect( lineEdit, SIGNAL( textChanged(QString) ), this, SLOT( slot_textChanged(QString) ) );
    connect( lineEdit, SIGNAL( textChanged(QString) ), this, SLOT( update() ) );

    setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
    setFixedHeight(4);
    setTextVisible(false);
    setMinimum(0);
    setMaximum(maxSteps);
    setToolTip(checkBox->text());

    layout->addWidget( this );

    vkLevelMeter = new QvkLevelMeter;
    QString device = checkBox->accessibleName().section( ":::", 0, 0);
    QString name = checkBox->text();
    vkLevelMeter->start( device, name, index );
}


void QvkLevelMeterController::remove_ProgressBar( QCheckBox *checkBox )
{
    vkLevelMeter->stop();

    // Remove LineEdit
    for ( int i = 0; i < global::listChildren->count(); i++ ) {
        QLineEdit *lineEdit = global::listChildren->at(i);
        if ( lineEdit->objectName().right(2) == checkBox->objectName().right(2) ) {
            global::listChildren->removeAt(i);
        }
    }
}


void QvkLevelMeterController::slot_textChanged( QString string )
{
    setValue( string.toDouble() * maxSteps );
}
