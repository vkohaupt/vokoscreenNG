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

#include "QvkCameraController.h"
#include "QvkCameraResolution.h"
#include "global.h"
#include "camerasettingsdialog.h"

#include <QCameraInfo>

QvkCameraController::QvkCameraController( Ui_formMainWindow *ui_surface ):videoSurface(new QvkVideoSurface())
{
    ui_formMainWindow = ui_surface;

    sliderCameraWindowSize = new QvkSpezialSlider( Qt::Horizontal );
    ui_formMainWindow->horizontalLayout_45->insertWidget( 1, sliderCameraWindowSize );
    sliderCameraWindowSize->setObjectName( "sliderCameraWindowSize" );
    sliderCameraWindowSize->setMinimum( 1 );
    sliderCameraWindowSize->setMaximum( 3 );
    sliderCameraWindowSize->setValue( 2 );
    sliderCameraWindowSize->show();
    sliderCameraWindowSize->setShowValue( false );
    sliderCameraWindowSize->setEnabled( false );

    vkCameraSettingsDialog = new cameraSettingsDialog;

    QvkCameraResolution *vkCameraResolution = new QvkCameraResolution( ui_formMainWindow, vkCameraSettingsDialog );
    connect( ui_formMainWindow->comboBoxCamera, SIGNAL( currentIndexChanged( int ) ), vkCameraResolution, SLOT( slot_resolution( int ) ) );
    connect( ui_formMainWindow->comboBoxCameraResolution, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_resolutionChanged() ) );

    getAllDevices();

    cameraWindow = new QvkCameraWindow( ui_surface, sliderCameraWindowSize, vkCameraSettingsDialog );
    cameraWindow->hide();
    connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_formMainWindow->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
    cameraWindow->setWindowTitle( QString( tr( "Camera") ) );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    cameraWindow->setWindowIcon( icon );

    QvkCameraWatcher *cameraWatcher = new QvkCameraWatcher( ui_formMainWindow );

    connect( cameraWatcher, SIGNAL( signal_addedCamera(   QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString ) ),          this, SLOT( slot_removedCamera( QString ) ) );

    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_formMainWindow->comboBoxCamera, SLOT( setDisabled( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this, SLOT( slot_startCamera( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraWindowFrame, SLOT( setEnabled( bool ) ) );

    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );

    connect( ui_formMainWindow->checkBoxCameraWindowFrame, SIGNAL( toggled( bool ) ), this, SLOT( slot_frameOnOff( bool ) ) );

    connect( sliderCameraWindowSize, SIGNAL( valueChanged( int ) ), this, SLOT( slot_sliderMoved( int ) ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_resolutionChanged()
{
    if ( ui_formMainWindow->checkBoxCameraOnOff->checkState() == Qt::Checked )
    {
        ui_formMainWindow->checkBoxCameraOnOff->click();
        ui_formMainWindow->checkBoxCameraOnOff->click();

        if ( vkCameraSettingsDialog->isVisible() == true )
        {
            vkCameraSettingsDialog->close();
            vkCameraSettingsDialog->show();
        }
    }
}


void QvkCameraController::getAllDevices()
{
    QList<QCameraInfo> camerasInfoList = QCameraInfo::availableCameras();
    if ( !camerasInfoList.empty() )
    {
        for ( int i = 0; i < camerasInfoList.count(); i++ )
        {
            if ( ( camerasInfoList.at(i).description() > "" ) and ( !camerasInfoList.at(i).description().contains( "@device:pnp" ) ) )
            {
               qDebug().noquote() << global::nameOutput << "[Camera] Found:" << camerasInfoList.at(i).description() << camerasInfoList.at(i).deviceName();
               slot_addedCamera( camerasInfoList.at(i).description(), camerasInfoList.at(i).deviceName() );
            }
        }
    }
}


void QvkCameraController::slot_frameOnOff( bool value )
{
    if ( cameraWindow->isVisible() == true )
    {
        Qt::WindowFlags flags;

#ifdef Q_OS_WIN
        if ( value == true )
        {
            flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }

        if ( value == false )
        {
            flags = Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }
#endif

#ifdef Q_OS_LINUX
        if ( value == true )
        {
            vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setDisabled( true );
            cameraWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint );
            ui_formMainWindow->checkBoxCameraOnOff->click();
            ui_formMainWindow->checkBoxCameraOnOff->click();
        }

        if ( value == false )
        {
            vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setDisabled( false );
            flags = Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }
#endif

        cameraWindow->show();
    }
}


void QvkCameraController::slot_sliderMoved( int value )
{
    if ( value == 1 )
    {
        cameraWindow->resize( 160, 120 );
    }

    if ( value == 2 )
    {
        cameraWindow->resize( 320, 240 );
    }

    if ( value == 3 )
    {
        cameraWindow->resize( 639, 479 );
    }
}


void QvkCameraController::slot_setNewImage( QImage image )
{
#ifdef Q_OS_LINUX
    if ( ui_formMainWindow->checkBoxCameraMirrorHorizontal->isChecked() == true )
        image = image.mirrored( true, false );

    if ( ui_formMainWindow->checkBoxCameraMirrorVertical->isChecked() == true )
        image = image.mirrored( false, true );
#endif

#ifdef Q_OS_WIN
    if ( ui_formMainWindow->checkBoxCameraMirrorHorizontal->isChecked() == true )
        image = image.mirrored( true, false );

    if ( ui_formMainWindow->checkBoxCameraMirrorVertical->isChecked() == false )
        image = image.mirrored( false, true );
#endif

    if ( ui_formMainWindow->checkBoxCameraInvert->isChecked() == true )
        image.invertPixels( QImage::InvertRgb );

    if ( ui_formMainWindow->checkBoxCameraGray->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Grayscale8 );

    if ( ui_formMainWindow->checkBoxCameraMono->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Mono );

    image = image.scaled( cameraWindow->width(), cameraWindow->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    cameraWindow->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}


void QvkCameraController::slot_addedCamera( QString description, QString device )
{
    ui_formMainWindow->checkBoxCameraOnOff->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->setEnabled( true );
    ui_formMainWindow->comboBoxCameraResolution->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->addItem( description, device.toLatin1() );
    ui_formMainWindow->checkBoxCameraGray->setEnabled( true );
    ui_formMainWindow->checkBoxCameraInvert->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMirrorHorizontal->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMirrorVertical->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMono->setEnabled( true );
    sliderCameraWindowSize->setEnabled( true );
    ui_formMainWindow->labelCameraWindowSize160_120->setEnabled( true );
    ui_formMainWindow->labelCameraWindowSize320_240->setEnabled( true );
    ui_formMainWindow->labelCameraWindowSize640_480->setEnabled( true );
}


void QvkCameraController::slot_removedCamera( QString device )
{
    if ( ( ui_formMainWindow->checkBoxCameraOnOff->isChecked() == true ) and ( ui_formMainWindow->comboBoxCamera->currentData().toString() == device ) )
    {
        cameraWindow->close();
    }

    int x = ui_formMainWindow->comboBoxCamera->findData( device.toLatin1() );
    ui_formMainWindow->comboBoxCamera->removeItem( x );

    if ( ui_formMainWindow->comboBoxCamera->count() == 0 )
    {
        ui_formMainWindow->checkBoxCameraOnOff->setEnabled( false );
        ui_formMainWindow->comboBoxCamera->setEnabled( false );
        ui_formMainWindow->comboBoxCameraResolution->clear();
        ui_formMainWindow->comboBoxCameraResolution->setEnabled( false );
        ui_formMainWindow->checkBoxCameraGray->setEnabled( false );
        ui_formMainWindow->checkBoxCameraInvert->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMirrorHorizontal->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMirrorVertical->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMono->setEnabled( false );
        sliderCameraWindowSize->setEnabled( false );
        ui_formMainWindow->labelCameraWindowSize160_120->setEnabled( false );
        ui_formMainWindow->labelCameraWindowSize320_240->setEnabled( false );
        ui_formMainWindow->labelCameraWindowSize640_480->setEnabled( false );
    }
}


void QvkCameraController::slot_startCamera( bool value )
{
    if ( value == true )
    {
        QByteArray device = ui_formMainWindow->comboBoxCamera->currentData().toByteArray();
        delete camera;
        camera = new QCamera( device );
        camera->setCaptureMode( QCamera::CaptureViewfinder );
        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
        connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );
        connect( camera, QOverload<QCamera::Error>::of( &QCamera::error ), this, [=]( QCamera::Error value ){ slot_error( value ); });

        int frameWidth = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt();
        int frameHeight = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt();
        QCameraViewfinderSettings viewfinderSettings;
        viewfinderSettings.setResolution( frameWidth, frameHeight );
        viewfinderSettings.setMinimumFrameRate( 0.0 );
        viewfinderSettings.setMaximumFrameRate( 0.0 );
        camera->setViewfinderSettings( viewfinderSettings );

        slot_sliderMoved( sliderCameraWindowSize->value() );

        camera->setViewfinder( videoSurface );
        cameraWindow->setStyleSheet( "background-color:black;" );
        cameraWindow->show();
#ifdef Q_OS_LINUX
        camera->load();
#endif
#ifdef Q_OS_WIN
        camera->start();
#endif
    }
    else
    {
        disconnect( camera, nullptr, nullptr, nullptr );
        camera->stop();
        camera->unload();
        cameraWindow->close();
    }
}


void QvkCameraController::slot_statusChanged( QCamera::Status status )
{
    switch ( status )
    {
      case QCamera::UnavailableStatus : { qDebug().noquote() << global::nameOutput << status; break; }// 0
      case QCamera::UnloadedStatus    : { qDebug().noquote() << global::nameOutput << status; break; }// 1
      case QCamera::LoadingStatus     : { qDebug().noquote() << global::nameOutput << status; break; }// 2
      case QCamera::UnloadingStatus   : { qDebug().noquote() << global::nameOutput << status; break; }// 3
      case QCamera::LoadedStatus      : { qDebug().noquote() << global::nameOutput << status;
                                          #ifdef Q_OS_LINUX
                                             camera->start();
                                          #endif
                                          break;
                                        }// 4
      case QCamera::StandbyStatus     : { qDebug().noquote() << global::nameOutput << status; break; }// 5
      case QCamera::StartingStatus    : { qDebug().noquote() << global::nameOutput << status; break; }// 6
      case QCamera::StoppingStatus    : { qDebug().noquote() << global::nameOutput << status; break; }// 7
      case QCamera::ActiveStatus      : { qDebug().noquote() << global::nameOutput << status; break; }// 8
    }
}


void QvkCameraController::slot_stateChanged( QCamera::State state )
{
    switch ( state )
    {
      case QCamera::UnloadedState : { qDebug().noquote() << global::nameOutput << state; break;  }// 0
      case QCamera::LoadedState   : { qDebug().noquote() << global::nameOutput << state; break;  }// 1
      case QCamera::ActiveState   : { qDebug().noquote() << global::nameOutput << state; break;  }// 2
    }
}


void QvkCameraController::slot_error( QCamera::Error error )
{
    switch ( error )
    {
    case QCamera::NoError:
    {
        break;
    }
    case QCamera::CameraError:
    {
        qDebug().noquote() << global::nameOutput << "General Camera error";
        cameraWindow->clear();
        cameraWindow->setStyleSheet( "background-color:white;" );
        cameraWindow->setText( "Camera is busy " );
        break;
    }
    case QCamera::InvalidRequestError:
    {
        qDebug().noquote() << global::nameOutput << "Camera invalid request error";
        break;
    }
    case QCamera::ServiceMissingError:
    {
        qDebug().noquote() << global::nameOutput << "Camera service missing error";
        break;
    }
    case QCamera::NotSupportedFeatureError :
    {
        qDebug().noquote() << global::nameOutput << "Camera not supported error";
        break;
    }
    }
}
