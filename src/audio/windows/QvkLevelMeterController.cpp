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
#include <QHBoxLayout>

QvkLevelMeterController::QvkLevelMeterController()
{
    maxSteps = 1000;
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}


void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QHBoxLayout *layout )
{
    QString index = checkBox->objectName().right(2);

    lineEdit = new QLineEdit();
    lineEdit->setObjectName( "lineEditLevelMeter_" + index );
    global::listChildren->append( lineEdit );

    connect( lineEdit, SIGNAL( textChanged(QString) ), this, SLOT( slot_textChanged(QString) ) );
    connect( lineEdit, SIGNAL( textChanged(QString) ), this, SLOT( update() ) );

    setObjectName( "progressBarAudioDevice-" + index );
    setFixedHeight(18);
    setTextVisible(false);
    setMinimum(0);
    setMaximum(maxSteps);
    setToolTip( "Index:" + index );

    layout->addWidget( this );

    vkLevelMeter = new QvkLevelMeter;
    QString device = checkBox->accessibleName().section( ":::", 0, 0);
    QString name = checkBox->accessibleName().section( ":::", 1, 1 );
    QString SourceOrPlayback = checkBox->accessibleName().section( ":::", 2, 2);
    set_Text( name );
    vkLevelMeter->start( device, name, index, SourceOrPlayback );
}


void QvkLevelMeterController::remove_LineEdit( QString index )
{
    // Remove LineEdit
    for ( int i = 0; i < global::listChildren->count(); i++ ) {
        QLineEdit *lineEdit_1 = global::listChildren->at(i);
        if ( lineEdit_1->objectName().right(2) == index ) {
            disconnect( lineEdit, nullptr, nullptr, nullptr );
            global::listChildren->removeAt(i);
            break;
        }
    }
}


void QvkLevelMeterController::slot_textChanged( QString string )
{
    setValue( string.toDouble() * maxSteps );
}
