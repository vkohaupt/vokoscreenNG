/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "QvkAudioController.h"

#include <QDebug>

QvkAudioController::QvkAudioController( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    radioButtonDirectSound = new QRadioButton();
    radioButtonDirectSound->setObjectName( "radioButtonDirectSound" );
    radioButtonDirectSound->setText( "DirectSound" );

    radioButtonWASAPI = new QRadioButton();
    radioButtonWASAPI->setObjectName( "radioButtonWASAPI" );
    radioButtonWASAPI->setText( "WASAPI" );

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    ui->verticalLayout_4->insertLayout( 0, hBoxLayout );
    hBoxLayout->addWidget( radioButtonDirectSound );
    hBoxLayout->addWidget( radioButtonWASAPI );
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hBoxLayout->addSpacerItem( spacer );

    vkWASAPIController = new QvkWASAPIController( ui );
    vkDirectSoundController = new QvkDirectSoundController( ui );

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), radioButtonDirectSound, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), radioButtonWASAPI,      SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), radioButtonDirectSound, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), radioButtonWASAPI,      SLOT( setDisabled( bool ) ) );

    connect( radioButtonWASAPI,      SIGNAL( toggled( bool ) ), this, SLOT( slot_WASAPI( bool ) ) );
    connect( radioButtonDirectSound, SIGNAL( toggled( bool ) ), this, SLOT( slot_DirectSound( bool ) ) );

    vkWASAPIController->init();
    vkDirectSoundController->init();
}


QvkAudioController::~QvkAudioController()
{
}

void QvkAudioController::slot_WASAPI( bool value )
{
    Q_UNUSED(value);

    vkDirectSoundController->vkDirectSoundWatcher->timer->stop();

    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        ui->verticalLayoutAudioDevices->removeWidget( listCheckBox.at(i) );
        delete listCheckBox.at(i);
    }

    vkWASAPIController->getAllDevices();
    vkWASAPIController->slot_audioDeviceSelected();
    vkWASAPIController->vkWASAPIWatcher->timer->start();
}

void QvkAudioController::slot_DirectSound( bool value )
{
    Q_UNUSED(value);

    vkWASAPIController->vkWASAPIWatcher->timer->stop();

    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        ui->verticalLayoutAudioDevices->removeWidget( listCheckBox.at(i) );
        delete listCheckBox.at(i);
    }

    vkDirectSoundController->getAllDevices();
    vkDirectSoundController->slot_audioDeviceSelected();
    vkDirectSoundController->vkDirectSoundWatcher->timer->start();
}
