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

QvkLevelMeterController::QvkLevelMeterController( QvkMainWindow *myParent )
{
    vkMainWindow = myParent;
    ui = vkMainWindow->ui;

    QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    for ( int i = 0; i < devices.count(); i++ ) {
        qDebug().noquote() << global::nameOutput << "[Audio] [Levelmeter]" << devices.at(i).description() << devices.at(i).id();
    }

    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        list << checkBox->accessibleName();
    }

    // Geräte ID vergleichen
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        for ( int x = 0; x < devices.count(); x++ ) {
            QCheckBox *checkBox = listQCheckBox.at(i);
            if ( checkBox->accessibleName() == devices.at(x).id() ) {
                InputStart *inputStart = new InputStart( devices.at(x) );

                QProgressBar *progressBar = new QProgressBar;
                progressBar->setFixedWidth(160);
                progressBar->setFixedHeight(8);
                progressBar->setTextVisible( false);

                QToolButton *toolButton = new QToolButton;
                toolButton->setCheckable( true );
                toolButton->setFixedSize( 16, 16 );

                QHBoxLayout *hBoxLayout = new QHBoxLayout;
                hBoxLayout->addWidget( toolButton );
                hBoxLayout->addWidget( progressBar );
                ui->verticalLayout_volumeter->addLayout( hBoxLayout );

                connect( inputStart, SIGNAL( signal_level(int) ), progressBar, SLOT( setValue(int) ) );
                connect( toolButton, &QToolButton::clicked, this, [=]( bool bo ) {
                    if ( bo == false ) {
                        inputStart->slot_stop();
                        progressBar->setValue(0);
                    } else {
                        inputStart->slot_start();
                    }
                } );
            }
        }
    }
}


QvkLevelMeterController::~QvkLevelMeterController()
{
}
