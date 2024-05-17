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

#include "global.h"
#include "QvkLevelMeterController.h"

#include <QAudioDevice>
#include <QMediaDevices>
#include <QCheckBox>
#include <QProgressBar>

QvkLevelMeterController::QvkLevelMeterController()
{
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}

#ifdef Q_OS_UNIX
void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{
    QList<QAudioDevice > devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        QAudioDevice audioDevice = devices.at(i);
        if ( audioDevice.id() == checkBox->accessibleName() ) {

            qDebug().noquote() << global::nameOutput << "[Audio] [Levelmeter]" << devices.at(i).description() << devices.at(i).id();

            setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
            setFixedHeight(4);
            setTextVisible(false);
            setMinimum(0);
            setMaximum(10000);
            setToolTip(checkBox->text());

            layout->addWidget( this );

            inputStart = new InputStart( devices.at(i) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( setValue(int) ) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( update() ) );
            inputStart->slot_start();
        }
    }
}
#endif


#ifdef Q_OS_WIN
void QvkLevelMeterController::add_ProgressBar( QCheckBox *checkBox, QVBoxLayout *layout )
{

    qDebug() << checkBox->objectName() << layout->objectName();

    QList<QAudioDevice > devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        QAudioDevice audioDevice = devices.at(i);
        if ( checkBox->accessibleName().contains( audioDevice.id() ) == true )  {
            qDebug().noquote() << global::nameOutput << "[Audio] [Levelmeter]" << devices.at(i).description() << devices.at(i).id();

            setObjectName( "progressBarAudioDevice-" + checkBox->objectName().right(2) );
            setFixedHeight(4);
            setTextVisible(false);
            setMinimum(0);
            setMaximum(10000);
            setToolTip(checkBox->text());

            layout->addWidget( this );

            inputStart = new InputStart( devices.at(i) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( setValue(int) ) );
            connect( inputStart, SIGNAL( signal_level(int) ), this, SLOT( update() ) );
            inputStart->slot_start();
        }
    }
}
#endif


void QvkLevelMeterController::remove_ProgressBar()
{
    inputStart->slot_stop();
    disconnect( inputStart, SIGNAL( signal_level(int) ), nullptr, nullptr );
}
