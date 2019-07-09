/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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
#include "global.h"

QvkCameraController::QvkCameraController(Ui_formMainWindow *ui_surface ):cameraWatcher(new QvkCameraWatcher()),
                                                                         videoSurface(new QvkVideoSurface())
{
    ui_formMainWindow = ui_surface;

    cameraWindow = new QvkCameraWindow( ui_surface );
    cameraWindow->hide();
    connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_formMainWindow->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
    cameraWindow->setWindowTitle( global::name + " " + global::version + " " + "Camera");
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    cameraWindow->setWindowIcon( icon );

    connect( cameraWatcher, SIGNAL( signal_addedCamera( QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString) ),         this, SLOT( slot_removedCamera( QString ) ) );
    cameraWatcher->cameraWatcherInit();
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_formMainWindow->comboBoxCamera, SLOT( setDisabled( bool ) ) );

#ifdef Q_OS_WIN
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), cameraWatcher, SLOT( slot_startStopCameraTimer( bool ) ) );
#endif
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this, SLOT( slot_startCamera( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraWindowFrame, SLOT( setEnabled(bool ) ) );

    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );

    connect( ui_formMainWindow->checkBoxCameraWindowFrame, SIGNAL( toggled( bool ) ), this, SLOT( slot_frameOnOff( bool ) ) );

    connect( ui_formMainWindow->sliderCameraWindowSize, SIGNAL( valueChanged( int ) ), this, SLOT( slot_sliderMoved( int ) ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_frameOnOff( bool value )
{
    if ( cameraWindow->isVisible() == true )
    {
        if ( value == true )
        {
            cameraWindow->setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
        }

        if ( value == false )
        {
            cameraWindow->setWindowFlags( Qt::Window | Qt::WindowStaysOnTopHint );
        }

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
        cameraWindow->resize( 640, 480 );
    }
}


void QvkCameraController::slot_setNewImage( QImage image )
{
    if ( ui_formMainWindow->checkBoxCameraMirrorHorizontal->isChecked() == true )
        image = image.mirrored( true, false );

    if ( ui_formMainWindow->checkBoxCameraMirrorVertical->isChecked() == true  )
        image = image.mirrored( false, true );

    if ( ui_formMainWindow->checkBoxCameraInvert->isChecked() == true )
        image.invertPixels( QImage::InvertRgb );

    if ( ui_formMainWindow->checkBoxCameraGray->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Grayscale8 );

    if ( ui_formMainWindow->checkBoxCameraMono->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Mono );

    image = image.scaled( cameraWindow->width(), cameraWindow->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    cameraWindow->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}


void QvkCameraController::slot_addedCamera( QString description, QString device )
{
    ui_formMainWindow->checkBoxCameraOnOff->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->addItem( description, device.toLatin1() );

    ui_formMainWindow->checkBoxCameraOnOff->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->setEnabled( true );
    ui_formMainWindow->checkBoxCameraGray->setEnabled( true );
    ui_formMainWindow->checkBoxCameraInvert->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMirrorHorizontal->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMirrorVertical->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMono->setEnabled( true );
    ui_formMainWindow->sliderCameraWindowSize->setEnabled( true );
    ui_formMainWindow->labelCameraWindowSize160_120->setEnabled( true );
    ui_formMainWindow->labelCameraWindowSize320_240->setEnabled( true );
    ui_formMainWindow->labelCameraWindowSize640_480->setEnabled( true );
}


void QvkCameraController::slot_removedCamera( QString device )
{
    if ( ( ui_formMainWindow->checkBoxCameraOnOff->isChecked() == true ) and ( ui_formMainWindow->comboBoxCamera->currentData() == device.toLatin1() ) )
    {
        cameraWindow->close();
    }

    int x = ui_formMainWindow->comboBoxCamera->findData( device.toLatin1() );
    ui_formMainWindow->comboBoxCamera->removeItem( x );

    if ( ui_formMainWindow->comboBoxCamera->count() == 0 )
    {
        ui_formMainWindow->checkBoxCameraOnOff->setEnabled( false );
        ui_formMainWindow->comboBoxCamera->setEnabled( false );
        ui_formMainWindow->checkBoxCameraGray->setEnabled( false );
        ui_formMainWindow->checkBoxCameraInvert->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMirrorHorizontal->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMirrorVertical->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMono->setEnabled( false );
        ui_formMainWindow->sliderCameraWindowSize->setEnabled( false );
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
        //disconnect( camera, nullptr, nullptr, nullptr );
        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
        connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );

        QCameraViewfinderSettings viewfinderSettings;
        viewfinderSettings.setResolution( 640, 480 );
        viewfinderSettings.setMinimumFrameRate( 0.0 );
        viewfinderSettings.setMaximumFrameRate( 0.0 );
        camera->setViewfinderSettings( viewfinderSettings );

        //delete cameraWindow;
        //cameraWindow = new QvkCameraWindow();
//        connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_formMainWindow->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
//        cameraWindow->setWindowTitle( global::name + " " + global::version + " " + "Camera");
//        QIcon icon( QString::fromUtf8( ":/pictures/player/logo.png" ) );
//        cameraWindow->setWindowIcon( icon );

        slot_sliderMoved( ui_formMainWindow->sliderCameraWindowSize->value() );

        camera->setViewfinder( videoSurface );
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
        //disconnect( cameraWindow, nullptr, nullptr, nullptr );
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
      case QCamera::UnloadedState : { qDebug().noquote() << global::nameOutput << state; break;  };// 0
      case QCamera::LoadedState   : { qDebug().noquote() << global::nameOutput << state; break;  };// 1
      case QCamera::ActiveState   : { qDebug().noquote() << global::nameOutput << state; break;  };// 2
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
        qDebug() <<  "QCamera" << "General Camera error";
        break;
    }
    case QCamera::InvalidRequestError:
    {
        qDebug() <<  "QCamera" << "Camera invalid request error";
        break;
    }
    case QCamera::ServiceMissingError:
    {
        qDebug() << "QCamera" <<  "Camera service missing error";
        break;
    }
    case QCamera::NotSupportedFeatureError :
    {
        qDebug() << "QCamera" << "Camera not supported error";
        break;
    }
    };
}
