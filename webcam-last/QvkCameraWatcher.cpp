#include "QvkCameraWatcher.h" 

#include <QCameraInfo>

QvkCameraWatcher::QvkCameraWatcher():oldCount(0)
{
    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_detectCameras() ) );
    timer->start(1000);
}


QvkCameraWatcher::~QvkCameraWatcher()
{
}


/*
 * Is called periodically by the timer
 */
void QvkCameraWatcher::slot_detectCameras()
{
    timer->stop();
    int newCount = QCameraInfo::availableCameras().count();
    QList<QCameraInfo> listCameraInfo = QCameraInfo::availableCameras();
qDebug() << listCameraInfo;
    if ( newCount > oldCount )
    {
        oldCount = newCount;
        emit signal_new_QCameraInfoList_a_camera_was_added( listCameraInfo );
    }

    if ( newCount < oldCount )
    {
        oldCount = newCount;
        emit signal_new_QCameraInfoList_a_camera_was_removed( listCameraInfo );
    }

    if ( newCount == 0 )
    {
        emit signal_cameras_available( false );
        emit signal_no_camera_available();
    }
    else
    {
        emit signal_cameras_available( true );
    }
    timer->start();
}

