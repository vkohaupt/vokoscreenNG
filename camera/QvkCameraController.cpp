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
    ui_vokoscreen = ui_surface;

    ui_vokoscreen->CheckBoxCamera->setEnabled( false );
    ui_vokoscreen->ComboBoxCamera->setEnabled( false );

    cameraWatcher = new QvkCameraWatcher();

    connect( cameraWatcher, SIGNAL( signal_addedCamera( QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString) ),         this, SLOT( slot_removedCamera( QString ) ) );

    connect( ui_vokoscreen->CheckBoxCamera, SIGNAL( toggled( bool ) ),        this, SLOT( slot_startCamera( bool ) ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_addedCamera(QString description, QString device)
{
    ui_vokoscreen->CheckBoxCamera->setEnabled( true );
    ui_vokoscreen->ComboBoxCamera->setEnabled( true );
    ui_vokoscreen->ComboBoxCamera->addItem( description, device );
}


void QvkCameraController::slot_removedCamera(QString device)
{
    int x = ui_vokoscreen->ComboBoxCamera->findData( device.toLatin1() );
    ui_vokoscreen->ComboBoxCamera->removeItem( x );

    if ( ui_vokoscreen->ComboBoxCamera->count() == 0 )
    {
        ui_vokoscreen->CheckBoxCamera->setEnabled( false );
        ui_vokoscreen->ComboBoxCamera->setEnabled( false );
    }
}


void QvkCameraController::slot_startCamera( bool value )
{
    if ( value == true )
    {
        QByteArray device = ui_vokoscreen->ComboBoxCamera->currentData().toByteArray();
        camera = new QCamera( device );
        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
        connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );

        QCameraViewfinderSettings viewfinderSettings;
        viewfinderSettings.setResolution( 640, 480 );
        viewfinderSettings.setMinimumFrameRate( 0.0 );
        viewfinderSettings.setMaximumFrameRate( 0.0 );
        camera->setViewfinderSettings( viewfinderSettings );

        videoWidget = new QVideoWidget();
        camera->setViewfinder( videoWidget );
        videoWidget->show();

        camera->load();
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
      case QCamera::LoadedStatus      : { qDebug() << "[vokoscreen]" << status; camera->start(); break; }// 4
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
