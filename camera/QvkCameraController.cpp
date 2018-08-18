#include "QvkCameraController.h"
//#include "QvkQCheckboxDescription.h"
//#include "QvkQLabelDevice.h"
//#include "QvkQComboboxResolution.h"

#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

// Hint: min. Qt 5.9.2

QvkCameraController::QvkCameraController(Ui_MainWindow *ui_surface )
{
    vkSettings.readAll();

    ui_vokoscreen = ui_surface;

    ui_vokoscreen->checkBoxCamera->setEnabled( false );
    ui_vokoscreen->comboBoxCamera->setEnabled( false );
    ui_vokoscreen->checkBoxGray->setEnabled( false );
    ui_vokoscreen->checkBoxInvert->setEnabled( false );
    ui_vokoscreen->checkBoxMirror->setEnabled( false );
    ui_vokoscreen->radioButtonLeftMiddle->setEnabled( false );
    ui_vokoscreen->radioButtonTopMiddle->setEnabled( false );
    ui_vokoscreen->radioButtonRightMiddle->setEnabled( false );
    ui_vokoscreen->radioButtonBottomMiddle->setEnabled( false );
    ui_vokoscreen->dialRotate->setEnabled( false );

    cameraWatcher = new QvkCameraWatcher();

    connect( cameraWatcher, SIGNAL( signal_addedCamera( QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString) ),         this, SLOT( slot_removedCamera( QString ) ) );

    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ),        cameraWatcher, SLOT( slot_startStopCameraTimer( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ),        this, SLOT( slot_startCamera( bool ) ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_addedCamera( QString description, QString device )
{
    ui_vokoscreen->checkBoxCamera->setEnabled( true );
    ui_vokoscreen->comboBoxCamera->setEnabled( true );
    ui_vokoscreen->comboBoxCamera->addItem( description, device );

    ui_vokoscreen->checkBoxGray->setEnabled( true );
    ui_vokoscreen->checkBoxInvert->setEnabled( true );
    ui_vokoscreen->checkBoxMirror->setEnabled( true );

    ui_vokoscreen->radioButtonLeftMiddle->setEnabled( true );
    ui_vokoscreen->radioButtonTopMiddle->setEnabled( true );
    ui_vokoscreen->radioButtonRightMiddle->setEnabled( true );
    ui_vokoscreen->radioButtonBottomMiddle->setEnabled( true );
    ui_vokoscreen->dialRotate->setEnabled( true );
}


void QvkCameraController::slot_removedCamera( QString device )
{
    int x = ui_vokoscreen->comboBoxCamera->findData( device.toLatin1() );
    ui_vokoscreen->comboBoxCamera->removeItem( x );

    if ( ui_vokoscreen->comboBoxCamera->count() == 0 )
    {
        ui_vokoscreen->checkBoxCamera->setEnabled( false );
        ui_vokoscreen->comboBoxCamera->setEnabled( false );
        ui_vokoscreen->checkBoxGray->setEnabled( false );
        ui_vokoscreen->checkBoxInvert->setEnabled( false );
        ui_vokoscreen->checkBoxMirror->setEnabled( false );

        ui_vokoscreen->radioButtonLeftMiddle->setEnabled( false );
        ui_vokoscreen->radioButtonTopMiddle->setEnabled( false );
        ui_vokoscreen->radioButtonRightMiddle->setEnabled( false );
        ui_vokoscreen->radioButtonBottomMiddle->setEnabled( false );
        ui_vokoscreen->dialRotate->setEnabled( false );
    }
}


void QvkCameraController::slot_startCamera( bool value )
{
    if ( value == true )
    {
        QByteArray device = ui_vokoscreen->comboBoxCamera->currentData().toByteArray();
        delete camera;
        camera = new QCamera( device );
        camera->setCaptureMode( QCamera::CaptureViewfinder );
        disconnect( camera, 0, 0, 0 );
        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
        connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );

        QCameraViewfinderSettings viewfinderSettings;
        viewfinderSettings.setResolution( 640, 480 );
        viewfinderSettings.setMinimumFrameRate( 0.0 );
        viewfinderSettings.setMaximumFrameRate( 0.0 );
        camera->setViewfinderSettings( viewfinderSettings );

        delete videoWidget;
        videoWidget = new QVideoWidget();
        videoWidget->setWindowTitle( vkSettings.getProgName() + " " + "camera"  + " " + vkSettings.getVersion() );
        videoWidget->resize( 320, 240 );
        camera->setViewfinder( videoWidget );
        videoWidget->show();
        camera->load();
    }
    else
    {
        disconnect( camera, 0, 0, 0 );
        camera->stop();
        camera->unload();
        videoWidget->close();
    }
}


// *************************************************************

void QvkCameraController::slot_statusChanged( QCamera::Status status )
{
    switch ( status )
    {
      case QCamera::UnavailableStatus : { qDebug() << "[vokoscreen]" << status; break; }// 0
      case QCamera::UnloadedStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 1
      case QCamera::LoadingStatus     : { qDebug() << "[vokoscreen]" << status; break; }// 2
      case QCamera::UnloadingStatus   : { qDebug() << "[vokoscreen]" << status; break; }// 3
      case QCamera::LoadedStatus      : { qDebug() << "[vokoscreen]" << status;
                                          camera->start(); break;
                                        }// 4
      case QCamera::StandbyStatus     : { qDebug() << "[vokoscreen]" << status; break; }// 5
      case QCamera::StartingStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 6
      case QCamera::StoppingStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 7
      case QCamera::ActiveStatus      : { qDebug() << "[vokoscreen]" << status; break; }// 8
    }
}


void QvkCameraController::slot_stateChanged( QCamera::State state )
{
    switch ( state )
    {
      case QCamera::UnloadedState : { qDebug() << "[vokoscreen]" << state; break;  };// 0
      case QCamera::LoadedState   : { qDebug() << "[vokoscreen]" << state; break;  };// 1
      case QCamera::ActiveState   : { qDebug() << "[vokoscreen]" << state; break;  };// 2
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
