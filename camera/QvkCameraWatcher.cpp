#include "QvkCameraWatcher.h" 

#include <QCameraInfo>

using namespace std;

QvkCameraWatcher::QvkCameraWatcher() : newcount(0)
{
    newDescriptionList.clear();
    newDeviceNameList.clear();
    oldDeviceNameList.clear();

    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( detectCameras() ) );
    timer->start( 1000 );
}


QvkCameraWatcher::~QvkCameraWatcher()
{
}


/*
 * Return removed device
 */
QString QvkCameraWatcher::removedDeviceName(QStringList myNewDeviceNameList, QStringList myOldDeviceNameList )
{
    QStringList removedList;
    int x;
    for ( x = 0; x < myOldDeviceNameList.count(); x++ )
    {
        removedList = myNewDeviceNameList.filter( myOldDeviceNameList[ x ] );
        if ( removedList.empty() )
            break;
    }
    return myOldDeviceNameList[ x ];
}


/*
 * Return added device
 * Wenn myNewDeviceNameList das Device nicht beinhaltet wird es hinzugefügt
 */
QString QvkCameraWatcher::addedDeviceName( QStringList myNewDeviceNameList, QString device )
{
    QString newDevice;
    if ( !myNewDeviceNameList.empty() )
    {
        if ( myNewDeviceNameList.contains( device ) == false )
        {
            newDevice = device;
        }
    }
    else
    {
        newDevice = device;
    }
    return newDevice;
}


/*
 * Is called periodically by the timer
 */
void QvkCameraWatcher::detectCameras()
{
    timer->stop();
    bool deviceAdded = false;

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    qDebug() << cameras;
    for ( int i = 0; i < QCameraInfo::availableCameras().count(); i++  )
    {
        QString cameraDevice = addedDeviceName( newDeviceNameList, cameras.at(i).deviceName() );
        if ( cameraDevice > "" )
        {
            newDeviceNameList << cameraDevice;
            newDescriptionList << cameras.at(i).description();
            newcount = newDeviceNameList.count();
            deviceAdded = true;
            emit signal_addedCamera( cameras.at(i).description(), cameraDevice );
        }
    }

    // Ein Device wurde hinzugefügt, Funktion wird dann hier abgebrochen
    if ( deviceAdded == true )
    {
        timer->start(1000);
        return;
    }

    if ( QCameraInfo::availableCameras().count() < newcount )
    {
        oldDeviceNameList = newDeviceNameList;
        newDescriptionList.clear();
        newDeviceNameList.clear();

        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        foreach ( const QCameraInfo &cameraInfo, cameras )
        {
            newDescriptionList << cameraInfo.description();
            newDeviceNameList << cameraInfo.deviceName();
        }

        newcount = newDeviceNameList.count();

        // detected which camera was removed
        QString cameraDevice = removedDeviceName( newDeviceNameList , oldDeviceNameList );
        emit signal_removedCamera( cameraDevice );
    }
    timer->start( 1000 );
}

