#include "QvkCameraWatcherLinux.h"
#include <QCameraInfo>
#include <QThread>

QvkCameraWatcher::QvkCameraWatcher()
{
}


void QvkCameraWatcher::cameraWatcherInit()
{
    cameraCount = 0;
    oldDeviceList.clear();
    QStringList deviceList;
    QString path = "/dev/";

    QFileSystemWatcher * fileSystemWatcher = new QFileSystemWatcher();
    fileSystemWatcher->addPath( path );
    connect( fileSystemWatcher, SIGNAL( directoryChanged( QString ) ), this, SLOT( myfileSystemWatcher( QString ) ) );

    QDir dir( path );
    QStringList filters;
    filters << "video*";
    deviceList = dir.entryList( filters, QDir::System, QDir::Time );
    deviceList.sort();

    oldDeviceList = deviceList;
    cameraCount = deviceList.count();

    for ( int x = 0; x < deviceList.count(); x++ )
    {
        QCameraInfo cameraInfo( QString( path + deviceList.at(x) ).toLatin1() );
        emit signal_addedCamera( cameraInfo.description() , path + deviceList.at(x) );
    }
}


QvkCameraWatcher::~QvkCameraWatcher()
{
}


/*
 * Add or remove plugin or plugout devices
 */
void QvkCameraWatcher::myfileSystemWatcher( QString path )
{
    QDir dir( path );
    QStringList filters;
    filters << "video*";
    QStringList newDeviceList = dir.entryList( filters, QDir::System, QDir::Time );
    newDeviceList.sort();

    // add plugin device
    if ( newDeviceList.count() > cameraCount )
    {
        QString addedDevice;
        for ( int x = 0; x < newDeviceList.count(); x++ )
        {
            if ( not oldDeviceList.contains( newDeviceList.at(x) ) )
            {
                addedDevice = newDeviceList.at(x);
            }
        }

        cameraCount = newDeviceList.count();

        // waiting for data from CameraInfo
        bool b = false;
        while ( b == false )
        {
            QCameraInfo cameraInfo( QString( path + addedDevice ).toLatin1() );
            if ( cameraInfo.isNull() == false )
            {
                b = true;
                emit signal_addedCamera( cameraInfo.description() , path + addedDevice );
            }
        }

        qDebug() << "[vokoscreen] added device" << addedDevice;
    }

    // remove plugout device
    if ( newDeviceList.count() < cameraCount )
    {
        QString removedDevice;
        for ( int x = 0; x < oldDeviceList.count(); x++ )
        {
            if ( not newDeviceList.contains( oldDeviceList.at(x) ) )
            {
                removedDevice = oldDeviceList.at(x);
            }
        }

        cameraCount = newDeviceList.count();
        emit signal_removedCamera( path + removedDevice );
        qDebug() << "[vokoscreen] removed device" << removedDevice;
    }

    if ( oldDeviceList.count() != newDeviceList.count() )
    {
        oldDeviceList = newDeviceList;
    }
}
