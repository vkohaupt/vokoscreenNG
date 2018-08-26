#include "QvkCameraController.h"

#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

#include <QPaintEngine>

// Hint: min. Qt 5.9.2

QvkCameraController::QvkCameraController(Ui_MainWindow *ui_surface ):cameraWatcher(new QvkCameraWatcher()),
                                                                     cameraWindow(new QvkCameraWindow()),
                                                                     videoSurface(new QvkVideoSurface( this ))
{
    ui_vokoscreen = ui_surface;

    vkSettings.readAll();

    ui_vokoscreen->toolButtonCameraHelp->setIcon( ui_vokoscreen->toolButtonCameraHelp->style()->standardIcon( QStyle::SP_MessageBoxInformation ) );

    ui_vokoscreen->dialRotate->setMinimum( 0 );
    ui_vokoscreen->dialRotate->setMaximum ( 360 );
    ui_vokoscreen->dialRotate->setWrapping( true );

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

    connect( cameraWatcher, SIGNAL( signal_addedCamera( QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString) ),         this, SLOT( slot_removedCamera( QString ) ) );

    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->comboBoxCamera, SLOT( setDisabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->checkBoxMirror, SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->checkBoxInvert, SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->checkBoxGray,   SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonTopMiddle,    SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonRightMiddle,  SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonBottomMiddle, SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonLeftMiddle,   SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), ui_vokoscreen->dialRotate,              SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), cameraWatcher, SLOT( slot_startStopCameraTimer( bool ) ) );
    connect( ui_vokoscreen->checkBoxCamera, SIGNAL( toggled( bool ) ), this,          SLOT( slot_startCamera( bool ) ) );

    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );

    connect( ui_vokoscreen->radioButtonLeftMiddle,  SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonLeftMiddle() ) );
    connect( ui_vokoscreen->radioButtonTopMiddle,   SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonTopMiddle() ) );
    connect( ui_vokoscreen->radioButtonRightMiddle, SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonRightMiddle() ) );
    connect( ui_vokoscreen->radioButtonBottomMiddle,SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonBottomMiddle() ) );

    // Checkable Widget sind in vokoscreen standardmäßig nicht gesetzt.
    // Diese werden hier beziehungsweise wenn die Settings vorhanden sind dort gesetzt.
    ui_vokoscreen->radioButtonBottomMiddle->clicked( true );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_radioButtonLeftMiddle()
{
    ui_vokoscreen->dialRotate->setValue( 90 );
}

void QvkCameraController::slot_radioButtonTopMiddle()
{
    ui_vokoscreen->dialRotate->setValue( 180 );
}

void QvkCameraController::slot_radioButtonRightMiddle()
{
    ui_vokoscreen->dialRotate->setValue( 270 );
}

void QvkCameraController::slot_radioButtonBottomMiddle()
{
    ui_vokoscreen->dialRotate->setValue( 360 );
}


void QvkCameraController::slot_setNewImage( QImage image )
{
    if ( ui_vokoscreen->checkBoxMirror->isChecked() == true )
        image = image.mirrored ( true, false );

    if ( ui_vokoscreen->checkBoxInvert->isChecked() == true )
        image.invertPixels( QImage::InvertRgb );

    if ( ui_vokoscreen->checkBoxGray->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Grayscale8 );

    QTransform transform;
    transform.rotate( ui_vokoscreen->dialRotate->value() );
    image = image.transformed( transform );

    image = image.scaled( cameraWindow->width(), cameraWindow->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    cameraWindow->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}


void QvkCameraController::slot_addedCamera( QString description, QString device )
{
    ui_vokoscreen->checkBoxCamera->setEnabled( true );
    ui_vokoscreen->comboBoxCamera->setEnabled( true );
    ui_vokoscreen->comboBoxCamera->addItem( description, device );
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
/*
        delete cameraWindow;
        cameraWindow = new QvkCameraWindow();
        connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_vokoscreen->checkBoxCamera, SLOT( setChecked( bool ) ) );
        cameraWindow->setWindowTitle( vkSettings.getProgName() + " " + "camera"  + " " + vkSettings.getVersion() );
        cameraWindow->resize( 320, 240 );
        camera->setViewfinder( videoSurface );
        cameraWindow->show();
        camera->load();
*/


        delete videoWidget;
        videoWidget = new QVideoWidget();
qDebug() << videoWidget->parent()-> << videoWidget->parentWidget();
        QPaintEngine *paintEngine = videoWidget->paintEngine();


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
        cameraWindow->close();
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
