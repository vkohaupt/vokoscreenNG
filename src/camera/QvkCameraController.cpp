#include "QvkCameraController.h"

#include <QCameraViewfinder>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

#include <QPaintEngine>

// Hint: min. Qt 5.9.2

QvkCameraController::QvkCameraController(Ui_formMainWindow *ui_surface ):cameraWatcher(new QvkCameraWatcher()),
                                                                         cameraWindow(new QvkCameraWindow()),
                                                                         videoSurface(new QvkVideoSurface())
{
    ui_vokoscreen = ui_surface;

    ui_vokoscreen->dialRotateCamera->setMinimum( 0 );
    ui_vokoscreen->dialRotateCamera->setMaximum ( 360 );
    ui_vokoscreen->dialRotateCamera->setWrapping( true );

    ui_vokoscreen->checkBoxCameraOnOff->setEnabled( false );
    ui_vokoscreen->comboBoxCamera->setEnabled( false );
    ui_vokoscreen->checkBoxCameraGray->setEnabled( false );
    ui_vokoscreen->checkBoxCameraInvert->setEnabled( false );
    ui_vokoscreen->checkBoxCameraMirror->setEnabled( false );
    ui_vokoscreen->radioButtonCameraLeft->setEnabled( false );
    ui_vokoscreen->radioButtonCameraTop->setEnabled( false );
    ui_vokoscreen->radioButtonCameraRight->setEnabled( false );
    ui_vokoscreen->radioButtonCameraBottom->setEnabled( false );
    ui_vokoscreen->dialRotateCamera->setEnabled( false );

    connect( cameraWatcher, SIGNAL( signal_addedCamera( QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString) ),         this, SLOT( slot_removedCamera( QString ) ) );
#ifdef Q_OS_LINUX
    cameraWatcher->cameraWatcherInit();
#endif
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->comboBoxCamera,          SLOT( setDisabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->checkBoxCameraMirror,    SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->checkBoxCameraInvert,    SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->checkBoxCameraGray,      SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonCameraTop,    SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonCameraRight,  SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonCameraBottom, SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->radioButtonCameraLeft,   SLOT( setEnabled( bool ) ) );
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_vokoscreen->dialRotateCamera,        SLOT( setEnabled( bool ) ) );
#ifdef Q_OS_WIN
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), cameraWatcher, SLOT( slot_startStopCameraTimer( bool ) ) );
#endif
    connect( ui_vokoscreen->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this,          SLOT( slot_startCamera( bool ) ) );

    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );

    connect( ui_vokoscreen->radioButtonCameraLeft,  SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonLeftMiddle() ) );
    connect( ui_vokoscreen->radioButtonCameraTop,   SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonTopMiddle() ) );
    connect( ui_vokoscreen->radioButtonCameraRight, SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonRightMiddle() ) );
    connect( ui_vokoscreen->radioButtonCameraBottom,SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonBottomMiddle() ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_radioButtonLeftMiddle()
{
    ui_vokoscreen->dialRotateCamera->setValue( 90 );
}

void QvkCameraController::slot_radioButtonTopMiddle()
{
    ui_vokoscreen->dialRotateCamera->setValue( 180 );
}

void QvkCameraController::slot_radioButtonRightMiddle()
{
    ui_vokoscreen->dialRotateCamera->setValue( 270 );
}

void QvkCameraController::slot_radioButtonBottomMiddle()
{
    ui_vokoscreen->dialRotateCamera->setValue( 360 );
}


void QvkCameraController::slot_setNewImage( QImage image )
{
    if ( ui_vokoscreen->checkBoxCameraMirror->isChecked() == true )
        image = image.mirrored ( true, false );

    if ( ui_vokoscreen->checkBoxCameraInvert->isChecked() == true )
        image.invertPixels( QImage::InvertRgb );

    if ( ui_vokoscreen->checkBoxCameraGray->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Grayscale8 );

    QTransform transform;
    transform.rotate( ui_vokoscreen->dialRotateCamera->value() );
    image = image.transformed( transform );

    image = image.scaled( cameraWindow->width(), cameraWindow->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    cameraWindow->setPixmap( QPixmap::fromImage( image, Qt::AutoColor) );
}


void QvkCameraController::slot_addedCamera( QString description, QString device )
{
    ui_vokoscreen->checkBoxCameraOnOff->setEnabled( true );
    ui_vokoscreen->comboBoxCamera->setEnabled( true );
    ui_vokoscreen->comboBoxCamera->addItem( description, device.toLatin1() );
}


void QvkCameraController::slot_removedCamera( QString device )
{
    if ( ( ui_vokoscreen->checkBoxCameraOnOff->isChecked() == true ) and ( ui_vokoscreen->comboBoxCamera->currentData() == device.toLatin1() ) )
    {
        cameraWindow->close();
    }

    int x = ui_vokoscreen->comboBoxCamera->findData( device.toLatin1() );
    ui_vokoscreen->comboBoxCamera->removeItem( x );

    if ( ui_vokoscreen->comboBoxCamera->count() == 0 )
    {
        ui_vokoscreen->checkBoxCameraOnOff->setEnabled( false );
        ui_vokoscreen->comboBoxCamera->setEnabled( false );
        ui_vokoscreen->checkBoxCameraGray->setEnabled( false );
        ui_vokoscreen->checkBoxCameraInvert->setEnabled( false );
        ui_vokoscreen->checkBoxCameraMirror->setEnabled( false );

        ui_vokoscreen->radioButtonCameraLeft->setEnabled( false );
        ui_vokoscreen->radioButtonCameraTop->setEnabled( false );
        ui_vokoscreen->radioButtonCameraRight->setEnabled( false );
        ui_vokoscreen->radioButtonCameraBottom->setEnabled( false );
        ui_vokoscreen->dialRotateCamera->setEnabled( false );
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
        disconnect( camera, nullptr, nullptr, nullptr );
        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
        connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );

        QCameraViewfinderSettings viewfinderSettings;
        viewfinderSettings.setResolution( 640, 480 );
        viewfinderSettings.setMinimumFrameRate( 0.0 );
        viewfinderSettings.setMaximumFrameRate( 0.0 );
        camera->setViewfinderSettings( viewfinderSettings );

        delete cameraWindow;
        cameraWindow = new QvkCameraWindow();
        connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_vokoscreen->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
        cameraWindow->setWindowTitle( vkSettings.getProgName() + " " + "camera"  + " " + vkSettings.getVersion() );
        QIcon icon( QString::fromUtf8( ":/pictures/player/vokoscreen.png" ) );
        cameraWindow->setWindowIcon( icon );

        cameraWindow->resize( 320, 240 );
        camera->setViewfinder( videoSurface );
        cameraWindow->show();

#ifdef Q_OS_LINUX
        camera->load();
#endif

#ifdef Q_OS_WIN
        camera->start();
#endif

        // Bash: Show properties from camera
        // uvcdynctrl -f -d /dev/video0

        // Test with QVideoWidget, only for Linux.
        /*
        delete videoWidget;
        videoWidget = new QVideoWidget();
        videoWidget->setWindowTitle( vkSettings.getProgName() + " " + "camera"  + " " + vkSettings.getVersion() );
        videoWidget->resize( 320, 240 );
        camera->setViewfinder( videoWidget );
        videoWidget->show();
        camera->load();
        */
    }
    else
    {
        disconnect( camera, nullptr, nullptr, nullptr );
        camera->stop();
        camera->unload();
        cameraWindow->close();
        //videoWidget->close();
    }
}


void QvkCameraController::slot_statusChanged( QCamera::Status status )
{
    switch ( status )
    {
      case QCamera::UnavailableStatus : { qDebug() << "[vokoscreen]" << status; break; }// 0
      case QCamera::UnloadedStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 1
      case QCamera::LoadingStatus     : { qDebug() << "[vokoscreen]" << status; break; }// 2
      case QCamera::UnloadingStatus   : { qDebug() << "[vokoscreen]" << status; break; }// 3
      case QCamera::LoadedStatus      : { qDebug() << "[vokoscreen]" << status;
                                          #ifdef Q_OS_LINUX
                                             camera->start();
                                          #endif
                                          break;
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
