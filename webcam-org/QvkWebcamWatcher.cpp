#include "QvkWebcamWatcher.h" 

#include <QCameraInfo>
#include <QTimer>

using namespace std;

QvkWebcamWatcher::QvkWebcamWatcher()
{
    oldcount = 0;
    descriptionList.clear();
    deviceNameList.clear();
    oldDescriptionList.clear();
    oldDeviceNameList.clear();

    QTimer *timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( detectCameras() ) );
    timer->start(1000);
}


QvkWebcamWatcher::~QvkWebcamWatcher()
{
}


void QvkWebcamWatcher::getAllCameraDescription()
{
    qDebug() << "[vokoscreen]" << "---Begin search cameras---";
    oldDescriptionList = descriptionList;
    oldDeviceNameList = deviceNameList;
    descriptionList.clear();
    deviceNameList.clear();

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach ( const QCameraInfo &cameraInfo, cameras )
    {
      if ( cameraInfo.description() != "screen-capture-recorder" )
      {
        qDebug() << "[vokoscreen]" << cameraInfo.description() << cameraInfo.deviceName();
        descriptionList << cameraInfo.description();
        deviceNameList << cameraInfo.deviceName();
      }
    }
    qDebug() << "[vokoscreen]" << "---End search cameras---";
    qDebug(" ");
    qDebug() << cameras.at(0).description();
    emit webcamDescription( descriptionList, deviceNameList );
}

/*
 * Return removed device
 */
QString QvkWebcamWatcher::removedDeviceName( QStringList mydeviceNameList, QStringList myoldDeviceNameList )
{
  QStringList removedList;
  int x;
  for ( x = 0; x < myoldDeviceNameList.count(); x++ )
  {
    removedList = mydeviceNameList.filter( myoldDeviceNameList[ x ] );
    if ( removedList.empty() )
      break;
  }
  return myoldDeviceNameList[ x ];
}

/*
 * Is called periodically by the timer
 */
void QvkWebcamWatcher::detectCameras()
{
    int newcount = QCameraInfo::availableCameras().count();

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach ( const QCameraInfo &cameraInfo, cameras )
    {
      if ( cameraInfo.description() == "screen-capture-recorder" )
      {
        newcount--;
      }
    }

  if ( newcount > oldcount )
  {
     getAllCameraDescription();
     oldcount = newcount;
     return;
  }

  if ( newcount < oldcount )
  {
     getAllCameraDescription();
     oldcount = newcount;

     // detected which camera was removed
     QString cameraDevice = removedDeviceName( deviceNameList , oldDeviceNameList );
     emit removedCamera( cameraDevice );
     return;
  }
}
