#include "QvkWebcamWatcher.h" 

#include <QCameraInfo>

QvkWebcamWatcher::QvkWebcamWatcher()
{
    oldCount = 0;

    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_detectCameras() ) );
    timer->start(1000);
}


QvkWebcamWatcher::~QvkWebcamWatcher()
{
}


/*
 * Is called periodically by the timer
 */
void QvkWebcamWatcher::slot_detectCameras()
{
    timer->stop();
    int newCount = QCameraInfo::availableCameras().count();
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    if ( newCount > oldCount )
    {
        oldCount = newCount;
        emit signal_new_list_a_camera_was_added( cameras );
    }

    if ( newCount < oldCount )
    {
        oldCount = newCount;
        emit signal_new_list_a_camera_was_removed( cameras );
    }
    timer->start();
}

