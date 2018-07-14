#include "QvkWebcamController.h"
#include "QvkWebcamWindow.h"
#include "QvkWebcamWatcher.h"
#include "QvkVideoSurface.h"

#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QVideoProbe>

QvkWebcamController::QvkWebcamController(Ui_MainWindow *value )
{
  //vkSettings.readAll();

  myUi = value;

  myUi->webcamCheckBox->setEnabled( false );

  myUi->webcamComboBox->setEnabled( false );

  myUi->rotateDial->setMinimum( 1 );
  myUi->rotateDial->setMaximum ( 360 );
  myUi->rotateDial->setValue( 1 );
  connect( myUi->rotateDial, SIGNAL( sliderPressed () ), this, SLOT( rotateDialclicked() ) );
/*
  myUi->radioButtonTopMiddle->setChecked( vkSettings.getWebcamButtonTopMiddle() );

  myUi->radioButtonRightMiddle->setChecked( vkSettings.getWebcamButtonRightMiddle() );

  myUi->radioButtonBottomMiddle->setChecked( vkSettings.getWebcamButtonBottomMiddle() );

  myUi->radioButtonLeftMiddle->setChecked( vkSettings.getWebcamButtonLeftMiddle() );

  myUi->grayCheckBox->setChecked( vkSettings.getWebcamGray() );
  myUi->invertCheckBox->setChecked( vkSettings.getWebcamInvert() );
*/
  webcamWindow = new QvkWebcamWindow();
  msgInWebcamWindow = new QvkMsgInWebcamWindow( this, webcamWindow );
  connect( this, SIGNAL( webcamBusy() ), msgInWebcamWindow, SLOT( close() ) );

  mirrored = false;

  if ( myUi->webcamCheckBox->checkState() == Qt::Unchecked )
  {
    myUi->mirrorCheckBox->setEnabled( false );
    myUi->dialFrame->setEnabled( false );
    myUi->grayCheckBox->setEnabled( false );
    myUi->invertCheckBox->setEnabled( false );
  }
  else
  {
    myUi->mirrorCheckBox->setEnabled( true );
    myUi->dialFrame->setEnabled( true );
    myUi->grayCheckBox->setEnabled( true );
    myUi->invertCheckBox->setEnabled( true );
  }
  connect( myUi->mirrorCheckBox, SIGNAL( clicked( bool ) ), this, SLOT( setMirrorOnOff( bool ) ) );
/*  if ( vkSettings.getWebcamMirrored() == Qt::Checked )
  {
    myUi->mirrorCheckBox->setEnabled( true );
    myUi->mirrorCheckBox->click();
    myUi->mirrorCheckBox->setEnabled( false );
  }
*/
  connect( myUi->webcamCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( webcamOnOff( int ) ) );
  connect( webcamWindow, SIGNAL( closeWebcamWindow() ), myUi->webcamCheckBox, SLOT( toggle() )  );
#ifndef Q_OS_WIN
  connect( webcamWindow, SIGNAL( setOverScreen() ), this, SLOT( overFullScreenWebcamCheckBox_OnOff() ) );
#endif
  videoSurface = new QvkVideoSurface( this );
  connect( videoSurface, SIGNAL( newPicture( QImage ) ), this, SLOT( setNewImage( QImage ) ) );

  QvkWebcamWatcher *webcamWatcher = new QvkWebcamWatcher();
  connect( webcamWatcher, SIGNAL( webcamDescription( QStringList, QStringList ) ), this, SLOT( addToComboBox( QStringList, QStringList ) ) );
  connect( webcamWatcher, SIGNAL( removedCamera( QString ) ), this, SLOT( ifCameraRemovedCloseWindow( QString ) ) );  

  // If all webcams complete read, then read setting for show or not show
  //connect( webcamWatcher, SIGNAL( webcamDescription( QStringList, QStringList ) ), this, SLOT( setCheckboxWebcamFromSettings() ) );

  connect( myUi->webcamComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( resolution( int ) )  );
}


QvkWebcamController::~QvkWebcamController()
{
}


void QvkWebcamController::resolution( int index )
{
    myUi->webcamCheckBox->setEnabled( false );
    myUi->webcamComboBox->setEnabled( false );
    myUi->resolutionComboBox->setEnabled( false );
    QCoreApplication::processEvents( QEventLoop::AllEvents );
    camera = new QCamera( myUi->webcamComboBox->currentData().toByteArray() );
    connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( myStatusChanged( QCamera::Status ) ) );
    connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( myStateChanged( QCamera::State ) )  );
    camera->load();
    myUi->resolutionComboBox->setEnabled( true );
    myUi->webcamComboBox->setEnabled( true );
    myUi->webcamCheckBox->setEnabled( true );
    camera->unload();
    delete camera;
}

/*
void QvkWebcamController::setCheckboxWebcamFromSettings()
{
  while ( ( myUi->webcamComboBox->count() < 1 ) or ( myUi->resolutionComboBox->count() < 1 ) )
  {
  }

  if ( Qt::CheckState( vkSettings.getWebcamOnOff() ) == Qt::Checked )
  {
      myUi->webcamCheckBox->click();
  }
}
*/

#ifndef Q_OS_WIN
void QvkWebcamController::overFullScreenWebcamCheckBox_OnOff()
{
  myUi->webcamCheckBox->click();
    QCoreApplication::processEvents( QEventLoop::AllEvents );
    webcamWindow->overFullScreenSetWindowFlags();
  myUi->webcamCheckBox->click();
}
#endif

void QvkWebcamController::webcamOnOff( int value )
{
  if ( value == Qt::Checked )
  {
    myUi->webcamComboBox->setEnabled( false );
    myUi->mirrorCheckBox->setEnabled( true );
    myUi->dialFrame->setEnabled( true );
    myUi->grayCheckBox->setEnabled( true );
    myUi->invertCheckBox->setEnabled( true );

    // save the active camera to a flag
    setActiveCamera( myUi->webcamComboBox->currentData().toString() );

    QByteArray device = myUi->webcamComboBox->currentData().toByteArray();
    displayWebcam( device );
  }

  if ( value == Qt::Unchecked )
  {
    camera->unload();
    camera->stop();
    webcamWindow->hide();
    delete camera;
    myUi->webcamComboBox->setEnabled( true );
    myUi->mirrorCheckBox->setEnabled( false );
    myUi->dialFrame->setEnabled( false );
    myUi->grayCheckBox->setEnabled( false );
    myUi->invertCheckBox->setEnabled( false );
  }
}


void QvkWebcamController::setNewImage( QImage image )
{
  if ( mirrored == true )
    image = image.mirrored ( true, false );

  if ( myUi->radioButtonLeftMiddle->isChecked() == true )
     myUi->rotateDial->setValue( 90 );

  if ( myUi->radioButtonTopMiddle->isChecked() == true )
     myUi->rotateDial->setValue( 180 );

  if ( myUi->radioButtonRightMiddle->isChecked() == true )
     myUi->rotateDial->setValue( 270 );

  if ( myUi->radioButtonBottomMiddle->isChecked() == true )
      myUi->rotateDial->setValue( 360 );

  QTransform transform;
  transform.rotate( myUi->rotateDial->value() );
  QImage transformedImage = image.transformed( transform );

  if ( myUi->grayCheckBox->isChecked() == true )
  {
    transformedImage = transformedImage.convertToFormat( QImage::Format_Grayscale8 );
  }

  if ( myUi->invertCheckBox->isChecked() == true )
  {
   transformedImage.invertPixels( QImage::InvertRgb );
  }

  // Passt Bild beim resizen des Fensters an
  transformedImage = transformedImage.scaled( webcamWindow->webcamLabel->width(), webcamWindow->webcamLabel->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
  webcamWindow->webcamLabel->setPixmap( QPixmap::fromImage( transformedImage, Qt::AutoColor) );
}

void QvkWebcamController::setMirrorOnOff( bool value )
{
  if ( value == true )
    mirrored = true;
  else
    mirrored = false;
}


void QvkWebcamController::rotateDialclicked()
{
  // Diese drei Befehle müssen sein damit der Radiobutton unchecked ist
  myUi->radioButtonTopMiddle->setCheckable( false );
  myUi->radioButtonTopMiddle->setChecked( false );
  myUi->radioButtonTopMiddle->setCheckable ( true );
  myUi->radioButtonTopMiddle->update();

  myUi->radioButtonRightMiddle->setCheckable ( false );
  myUi->radioButtonRightMiddle->setChecked( false );
  myUi->radioButtonRightMiddle->setCheckable ( true );
  myUi->radioButtonRightMiddle->update();

  myUi->radioButtonBottomMiddle->setCheckable ( false );
  myUi->radioButtonBottomMiddle->setChecked( false );
  myUi->radioButtonBottomMiddle->setCheckable ( true );
  myUi->radioButtonBottomMiddle->update();

  myUi->radioButtonLeftMiddle->setCheckable ( false );
  myUi->radioButtonLeftMiddle->setChecked( false );
  myUi->radioButtonLeftMiddle->setCheckable ( true );
  myUi->radioButtonLeftMiddle->update();
}

/*
 * If camera removed and have a display window, the diplay window will removed
 */
void QvkWebcamController::ifCameraRemovedCloseWindow( QString value )
{
   qDebug() << "[vokoscreen] camera removed" << value;
   qDebug(" ");
   if ( getActiveCamera() == value )
   {
     myUi->webcamCheckBox->setChecked( false );
   }
}

/*
 * set a flag which camera is active
 */
void QvkWebcamController::setActiveCamera( QString value )
{
   aktivCamera = value;
}

/*
 * get a flag which camera is active
 */
QString QvkWebcamController::getActiveCamera()
{
    return aktivCamera;
}


void QvkWebcamController::addToComboBox( QStringList description, QStringList device )
{
    myUi->webcamComboBox->clear();

    if ( device.count()  > 0  )
    {
      for ( int i = 0; i < description.count(); i++ )
      {
        QString descript =  description[i];
        if ( !descript.contains( "@device:pnp" ) ) // Geräte mit dieser Beschreibung aussortieren
          myUi->webcamComboBox->addItem( description[i], device[i] );
      }
    }
    else
    {
      myUi->webcamCheckBox->setEnabled( false );
      myUi->webcamComboBox->setEnabled( false );
    }

}


// http://doc.qt.io/qt-5/qmultimedia.html#AvailabilityStatus-enum
void QvkWebcamController::displayWebcam( QByteArray device )
{
    camera = new QCamera( device );
    camera->setCaptureMode( QCamera::CaptureViewfinder );

    connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( myStatusChanged( QCamera::Status ) ) );
    connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( myStateChanged( QCamera::State ) )  );

    QStringList list = myUi->resolutionComboBox->currentText().split( "x" );
    QString w = list.at(0);
    QString h = list.at(1);
    QCameraViewfinderSettings viewfinderSettings;
    viewfinderSettings.setResolution( w.toInt(), h.toInt() );
    viewfinderSettings.setMinimumFrameRate( 0.0 );
    viewfinderSettings.setMaximumFrameRate( 0.0 );
    camera->setViewfinderSettings( viewfinderSettings );

    camera->setViewfinder( videoSurface );

    webcamWindow->show();

    camera->start();
}


void QvkWebcamController::myStatusChanged( QCamera::Status status )
{
  if( myUi->webcamCheckBox->isChecked() == true )
  {
    switch ( status )
    {
      case QCamera::UnavailableStatus : { qDebug() << "[vokoscreen]" << status; break; }// 0
      case QCamera::UnloadedStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 1
      case QCamera::LoadingStatus     : { qDebug() << "[vokoscreen]" << status;
                                          msgInWebcamWindow->setMsg( tr( "Camera is loading" ) );
                                          break;
                                        }// 2
      case QCamera::UnloadingStatus   : { qDebug() << "[vokoscreen]" << status; break; }// 3
      case QCamera::LoadedStatus      : { qDebug() << "[vokoscreen]" << status;
                                          msgInWebcamWindow->setMsg( tr( "Camera is loaded" ) );
                                          qDebug() << "";
                                          break;
                                        }// 4
      case QCamera::StandbyStatus     : { qDebug() << "[vokoscreen]" << status; break; }// 5
      case QCamera::StartingStatus    : { qDebug() << "[vokoscreen]" << status;
                                          msgInWebcamWindow->setMsg( tr( "Wait a second or Camera is busy?" ) );
                                          break;
                                        }// 6
      case QCamera::StoppingStatus    : { qDebug() << "[vokoscreen]" << status; break; }// 7
      case QCamera::ActiveStatus      : { qDebug() << "[vokoscreen]" << status;
                                          emit webcamBusy();
                                          break; }// 8
    }
  }

  // Windows
  if( ( myUi->webcamCheckBox->isChecked() == false ) and ( status == QCamera::LoadedStatus ) )
//  if( ( myUi->webcamCheckBox->isChecked() == false ) and ( status == QCamera::LoadedState ) )
  {
      qDebug() << "[vokoscreen] ---Begin search camera parameters and checkbox is disabled---";
      QCameraViewfinderSettings settings;
      QList<QSize> resolution = camera->supportedViewfinderResolutions( settings );
      qDebug() << "[vokoscreen] camera resolutions" << resolution;

      // Begin load camera resolutions in combobox
      myUi->resolutionComboBox->clear();
      QStringList stringlist;
      for( int i = 0; i < resolution.count(); i++ )
      {
          int w = resolution[i].width();
          int h = resolution[i].height();
          QString wh = QString::number( w  ) + "x" + QString::number( h );
          stringlist.append( wh );
      }
      myUi->resolutionComboBox->addItems(stringlist);
      // End load

      qDebug() << "[vokoscreen] ---End search camera parameters and checkbox is disabled---";
      qDebug();

/*
      QList<QVideoFrame::PixelFormat> pixelFormat = camera->supportedViewfinderPixelFormats( settings );
      qDebug() << "[vokoscreen] camera pixelformats" << pixelFormat;
      qDebug() << "";

      QList<QCamera::FrameRateRange> frameRateRange = camera->supportedViewfinderFrameRateRanges( settings );
      for ( int i = 0; i < frameRateRange.count(); i++ )
      {
          qDebug() << "[vokoscreen] camera frameRateRange" << frameRateRange[i].minimumFrameRate << frameRateRange[i].maximumFrameRate;
      }
      qDebug() << "";
*/
  }

}


void QvkWebcamController::myStateChanged( QCamera::State state )
{
    switch ( state )
    {
      case QCamera::UnloadedState : { qDebug() << "[vokoscreen]" << state; break;  };// 0
      case QCamera::LoadedState   : { qDebug() << "[vokoscreen]" << state; break;  };// 1
      case QCamera::ActiveState   : { qDebug() << "[vokoscreen]" << state; break;  };// 2
    }
}
