#include "QvkCameraWatcher.h" 

#include <QCameraInfo>

using namespace std;

QvkCameraWatcher::QvkCameraWatcher() : newcount(0)
{
    newDescriptionList.clear();
    newDeviceNameList.clear();
    oldDeviceNameList.clear();

    timer = new QTimer(this);
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( 2000 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_detectCameras() ) );
    timer->start();
}


QvkCameraWatcher::~QvkCameraWatcher()
{
}


void QvkCameraWatcher::slot_startStopCameraTimer( bool value )
{
    if ( value == true )
    {
        timer->stop();
    }

    if ( value == false )
    {
        timer->start();
    }
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
void QvkCameraWatcher::slot_detectCameras()
{
qDebug() << "void QvkCameraWatcher::slot_detectCameras()";

// Voll Krass, wenn Camera aktiv dann availableCameras().count() = 0;
 QList<QCameraInfo> info = QCameraInfo::availableCameras();
 qDebug() << info.count();

    timer->stop();
    bool deviceAdded = false;

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
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
        timer->start();
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
        qDebug() << "Removed Camera" << cameraDevice;
        emit signal_removedCamera( cameraDevice );
    }
    timer->start();
}

