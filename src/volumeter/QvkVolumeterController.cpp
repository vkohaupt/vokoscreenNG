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
#include "QvkVolumeterController.h"

#include <QAudioDevice>
#include <QMediaDevices>
#include <QCheckBox>

QvkVolumeterController::QvkVolumeterController( QvkMainWindow *myParent )
{
    vkMainWindow = myParent;
    ui = vkMainWindow->ui;

    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        qDebug().noquote() << global::nameOutput << "[Volumeter]" << devices.at(i).description() << devices.at(i).id();
    }

    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        list << checkBox->accessibleName();
    }

    QPushButton *pushButton = new QPushButton;
    pushButton->setText( "Vorschau" );

    // Geräte ID vergleichen
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        for ( int x = 0; x < devices.count(); x++ ) {
            QCheckBox *checkBox = listQCheckBox.at(i);
            if ( checkBox->accessibleName() == devices.at(x).id() ) {
                InputStart *inputStart = new InputStart( devices.at(x) );
                QLabel *label = new QLabel;
                ui->verticalLayout_volumeter->addWidget( label );
                connect( inputStart, SIGNAL( signal_level(qreal) ), label, SLOT( setNum(qreal) ) );
                connect( pushButton, SIGNAL( clicked(bool) ), inputStart, SLOT( slot_stop() ) );
            }
        }
    }

    ui->verticalLayout_volumeter->addWidget( pushButton );
}


QvkVolumeterController::~QvkVolumeterController()
{
}
