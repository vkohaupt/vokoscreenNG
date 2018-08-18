#ifndef QvkCameraWatcher_H 
#define QvkCameraWatcher_H

#include <QObject>
#include <QCameraInfo>
#include <QTimer>

class QvkCameraWatcher: public QObject
{

    Q_OBJECT

public:
    QvkCameraWatcher();
    virtual ~QvkCameraWatcher();


public slots:


private slots:
    void slot_detectCameras();


signals:
    void signal_new_QCameraInfoList_a_camera_was_added( QList<QCameraInfo> );
    void signal_new_QCameraInfoList_a_camera_was_removed( QList<QCameraInfo> );
    void signal_cameras_available( bool );
    void signal_no_camera_available();


protected:  


private:
    QTimer *timer;
    int oldCount;


};

#endif
