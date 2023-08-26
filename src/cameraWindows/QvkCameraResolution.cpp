/* vokoscreenNG - A desktop recorder
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

#include "QvkCameraResolution.h"
#include "global.h"

#include <QSettings>
#include <QCameraInfo>

QvkCameraResolution::QvkCameraResolution( Ui_formMainWindow *ui, QvkCameraSettingsDialog *settingsDialog )
{
    ui_formMainWindow = ui;
    vkCameraSettingsDialog = settingsDialog;
}


QvkCameraResolution::~QvkCameraResolution()
{
}


void QvkCameraResolution::slot_resolution( int value )
{
    Q_UNUSED(value);
    QByteArray device = ui_formMainWindow->comboBoxCamera->currentData().toByteArray();
    delete camera;
    camera = new QCamera( device );
    connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
    connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );
    camera->load();
}


void QvkCameraResolution::slot_statusChanged( QCamera::Status status )
{
    switch ( status )
    {
    case QCamera::UnavailableStatus : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 0
    case QCamera::UnloadedStatus    : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 1
    case QCamera::LoadingStatus     : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 2
    case QCamera::UnloadingStatus   : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 3
    case QCamera::LoadedStatus      : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status;
        ui_formMainWindow->comboBoxCameraResolution->clear();
        vkCameraSettingsDialog->ui->comboBoxCameraResolution->clear();
        QCameraViewfinderSettings cameraViewfinderSettings;
        QList<QSize> resolution = camera->supportedViewfinderResolutions( cameraViewfinderSettings );
        for ( int x = 0; x < resolution.count(); x++ ) {
            QString width = QString::number( resolution.at(x).width() );
            QString height = QString::number( resolution.at(x).height() );
            ui_formMainWindow->comboBoxCameraResolution->addItem( width + "x" + height );
            vkCameraSettingsDialog->ui->comboBoxCameraResolution->addItem( width + "x" + height );
        }
        qDebug().noquote() << global::nameOutput << "[Camera] Resolutions for:"
                           << ui_formMainWindow->comboBoxCamera->currentText()
                           << ui_formMainWindow->comboBoxCamera->currentData().toByteArray();
        qDebug() << resolution;

        camera->unload();
        disconnect( camera, nullptr, nullptr, nullptr );

        QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
        QString valueText = settings.value( "comboBoxCameraResolution", "640x480" ).toString();
        int index = ui_formMainWindow->comboBoxCameraResolution->findText( valueText );
        if ( index == -1 ) { index = 0; }
        ui_formMainWindow->comboBoxCameraResolution->setCurrentIndex( index );
        vkCameraSettingsDialog->ui->comboBoxCameraResolution->setCurrentIndex( index );

        QList<QSlider *> listSlider = ui_formMainWindow->centralWidget->findChildren<QSlider *>();
        for ( int i = 0; i < listSlider.count(); i++ ) {
            if ( ( listSlider.at(i)->objectName() == "sliderCameraWindowSize" ) or ( listSlider.at(i)->objectName() == "sliderCameraWindowZoom" ) ) {
                QVariant variant = settings.value( listSlider.at(i)->objectName() );
                if ( variant.isValid() ) {
                    listSlider.at(i)->setValue( variant.toInt() );
                }
            }
        }

        break;
    }// 4
    case QCamera::StandbyStatus     : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 5
    case QCamera::StartingStatus    : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 6
    case QCamera::StoppingStatus    : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 7
    case QCamera::ActiveStatus      : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << status; break; }// 8
    }
}


void QvkCameraResolution::slot_stateChanged( QCamera::State state )
{
    switch ( state )
    {
      case QCamera::UnloadedState : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << state; break;  }// 0
      case QCamera::LoadedState   : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << state; break;  }// 1
      case QCamera::ActiveState   : { qDebug().noquote() << global::nameOutput << "[Camera] QvkCameraResolution" << state; break;  }// 2
    }
}
