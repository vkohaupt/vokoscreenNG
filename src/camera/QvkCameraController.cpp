#include "QvkCameraController.h"
#include "global.h"

#include <QCameraViewfinder>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

#include <QPaintEngine>

QvkCameraController::QvkCameraController(Ui_formMainWindow *ui_surface ):cameraWatcher(new QvkCameraWatcher()),
                                                                         cameraWindow(new QvkCameraWindow()),
                                                                         videoSurface(new QvkVideoSurface())
{
    ui_formMainWindow = ui_surface;

    ui_formMainWindow->dialRotateCamera->setMinimum( 0 );
    ui_formMainWindow->dialRotateCamera->setMaximum ( 360 );
    ui_formMainWindow->dialRotateCamera->setWrapping( true );

    connect( cameraWatcher, SIGNAL( signal_addedCamera( QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString) ),         this, SLOT( slot_removedCamera( QString ) ) );
#ifdef Q_OS_LINUX
    cameraWatcher->cameraWatcherInit();
#endif
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_formMainWindow->comboBoxCamera,          SLOT( setDisabled( bool ) ) );

#ifdef Q_OS_WIN
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), cameraWatcher, SLOT( slot_startStopCameraTimer( bool ) ) );
#endif
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this,          SLOT( slot_startCamera( bool ) ) );

    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );

    connect( ui_formMainWindow->radioButtonCameraLeft,  SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonLeftMiddle() ) );
    connect( ui_formMainWindow->radioButtonCameraTop,   SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonTopMiddle() ) );
    connect( ui_formMainWindow->radioButtonCameraRight, SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonRightMiddle() ) );
    connect( ui_formMainWindow->radioButtonCameraBottom,SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonBottomMiddle() ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_radioButtonLeftMiddle()
{
    ui_formMainWindow->dialRotateCamera->setValue( 90 );
}

void QvkCameraController::slot_radioButtonTopMiddle()
{
    ui_formMainWindow->dialRotateCamera->setValue( 180 );
}

void QvkCameraController::slot_radioButtonRightMiddle()
{
    ui_formMainWindow->dialRotateCamera->setValue( 270 );
}

void QvkCameraController::slot_radioButtonBottomMiddle()
{
    ui_formMainWindow->dialRotateCamera->setValue( 360 );
}


void QvkCameraController::slot_setNewImage( QImage image )
{
    if ( ui_formMainWindow->checkBoxCameraMirror->isChecked() == true )
        image = image.mirrored ( true, false );

    if ( ui_formMainWindow->checkBoxCameraInvert->isChecked() == true )
        image.invertPixels( QImage::InvertRgb );

    if ( ui_formMainWindow->checkBoxCameraGray->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Grayscale8 );

    QTransform transform;
    transform.rotate( ui_formMainWindow->dialRotateCamera->value() );
    image = image.transformed( transform );

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
    ui_formMainWindow->checkBoxCameraMirror->setEnabled( true );

    ui_formMainWindow->radioButtonCameraLeft->setEnabled( true );
    ui_formMainWindow->radioButtonCameraTop->setEnabled( true );
    ui_formMainWindow->radioButtonCameraRight->setEnabled( true );
    ui_formMainWindow->radioButtonCameraBottom->setEnabled( true );
    ui_formMainWindow->dialRotateCamera->setEnabled( true );
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
        ui_formMainWindow->checkBoxCameraMirror->setEnabled( false );

        ui_formMainWindow->radioButtonCameraLeft->setEnabled( false );
        ui_formMainWindow->radioButtonCameraTop->setEnabled( false );
        ui_formMainWindow->radioButtonCameraRight->setEnabled( false );
        ui_formMainWindow->radioButtonCameraBottom->setEnabled( false );
        ui_formMainWindow->dialRotateCamera->setEnabled( false );
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
        connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_formMainWindow->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
        cameraWindow->setWindowTitle( vkSettings.getProgName() + " " + "camera"  + " " + vkSettings.getVersion() );
        QIcon icon( QString::fromUtf8( ":/pictures/player/logo.png" ) );
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
