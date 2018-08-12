#include "QvkWebcamController.h"
#include "QvkWebcamWatcher.h"
#include "QvkVideoSurface.h"

QvkWebcamController::QvkWebcamController( Ui_MainWindow *value )
{
    ui_vokoscreen = value;
    QvkWebcamWatcher *vkWebcamWatcher = new QvkWebcamWatcher;

    connect( vkWebcamWatcher, SIGNAL( signal_new_QCameraInfoList_a_camera_was_added( QList<QCameraInfo> ) ), this, SLOT( slot_new_camera_list( QList<QCameraInfo> ) ) );
    connect( ui_vokoscreen->ComboBoxCamera, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_loadCamera() ) );
    connect( ui_vokoscreen->CheckBoxCamera, SIGNAL( toggled( bool ) ), this, SLOT( slot_Start_Stop_Camera( bool ) )  );

    connect( vkWebcamWatcher, SIGNAL( signal_cameras_available( bool ) ), ui_vokoscreen->CheckBoxCamera, SLOT( setEnabled( bool ) ) );

    connect( vkWebcamWatcher, SIGNAL( signal_cameras_available( bool ) ), ui_vokoscreen->ComboBoxCamera, SLOT( setEnabled( bool ) ) );
    connect( vkWebcamWatcher, SIGNAL( signal_no_camera_available() ),     ui_vokoscreen->ComboBoxCamera, SLOT( clear() ) );

    videoSurface = new QvkVideoSurface( this );
    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_newImage( QImage ) ) );
}


QvkWebcamController::~QvkWebcamController()
{
}


void QvkWebcamController::slot_new_camera_list( QList<QCameraInfo> cameraInfoList )
{
    qDebug() << "***************************   slot_new_camera_list( QList<QCameraInfo> cameraInfoList )";
    ui_vokoscreen->ComboBoxCamera->clear();
    for ( int x = 0; x < cameraInfoList.count(); x++  )
    {
        ui_vokoscreen->ComboBoxCamera->addItem( QIcon::fromTheme( "camera-web", QIcon( ":/pictures/webcam.png" ) ),
                                                cameraInfoList.at( x ).description(),
                                                cameraInfoList.at( x ).deviceName() );
    }
}


void QvkWebcamController::slot_loadCamera()
{
    qDebug() << "***************************  slot_loadCamera()";
    if ( ui_vokoscreen->ComboBoxCamera->count() == 0 )
    {
        if ( ui_vokoscreen->CheckBoxCamera->isChecked() == true )
        {
          qDebug() << "11111111111111111111";
          //ui_vokoscreen->CheckBoxCamera->setChecked( false );
          camera = nullptr;
          webcamWindow = nullptr;
        }
        return;
    }
    qDebug() << "22222222222222222222222222";

    delete camera;
    delete webcamWindow;

    camera = new QCamera( ui_vokoscreen->ComboBoxCamera->currentData().toByteArray() );
    camera->setCaptureMode( QCamera::CaptureViewfinder );
    //connect( camera, SIGNAL( error( QCamera::Error ) ), this, SLOT( slot_error( QCamera::Error ) ) );
    //connect( camera, QOverload<QCamera::Error>::of(&QCamera::error), [=](QCamera::Error value){ qDebug() << value; });
    //connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
    //connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );

    QCameraViewfinderSettings viewfinderSettings;
    viewfinderSettings.setResolution( 640, 480 );
    viewfinderSettings.setMinimumFrameRate( 0.0 );
    viewfinderSettings.setMaximumFrameRate( 0.0 );
    camera->setViewfinderSettings( viewfinderSettings );

    camera->setViewfinder( videoSurface );

    webcamWindow = new QvkWebcamWindow;
    connect( webcamWindow, SIGNAL( signal_webcamwindow_close() ), ui_vokoscreen->CheckBoxCamera, SLOT( click() ) );
    camera->load();
}


void QvkWebcamController::slot_Start_Stop_Camera( bool value )
{
    if ( value == true )
    {
        qDebug() << "***************************  slot_Start_Stop_Camera  --> Start";
        webcamWindow->show();
        camera->start();
    }

    if ( value == false )
    {
        qDebug() << "***************************  slot_Start_Stop_Camera  --> Stop";
        webcamWindow->hide();
        camera->stop();
    }
}


void QvkWebcamController::slot_newImage( QImage image )
{
    // Passt Bild beim resizen des Fensters an
    image = image.scaled( webcamWindow->width(), webcamWindow->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    webcamWindow->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}


void QvkWebcamController::slot_statusChanged( QCamera::Status status )
{
    switch ( status )
    {
      case QCamera::UnavailableStatus : { qDebug() << "[vokoscreen]" << status; break; }// 0
      case QCamera::UnloadedStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 1
      case QCamera::LoadingStatus     : { qDebug() << "[vokoscreen]" << status; break; }// 2
      case QCamera::UnloadingStatus   : { qDebug() << "[vokoscreen]" << status; break; }// 3
      case QCamera::LoadedStatus      : { qDebug() << "[vokoscreen]" << status; break; }// 4
      case QCamera::StandbyStatus     : { qDebug() << "[vokoscreen]" << status; break; }// 5
      case QCamera::StartingStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 6
      case QCamera::StoppingStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 7
      case QCamera::ActiveStatus      : { qDebug() << "[vokoscreen]" << status; break; }// 8
    }
}


void QvkWebcamController::slot_stateChanged( QCamera::State state )
{
    switch ( state )
    {
      case QCamera::UnloadedState : { qDebug() << "[vokoscreen]" << state; break;  };// 0
      case QCamera::LoadedState   : { qDebug() << "[vokoscreen]" << state; break;  };// 1
      case QCamera::ActiveState   : { qDebug() << "[vokoscreen]" << state; break;  };// 2
    }
}


void QvkWebcamController::slot_error( QCamera::Error error )
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
