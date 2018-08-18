#ifndef QvkCameraController_H
#define QvkCameraController_H

#include "ui_mainwindow.h"
#include "QvkVideoSurface.h"
#include <qvkwebcamwindow.h>

#include <QCamera>
#include <QCameraInfo>

class QvkCameraController : public QObject
{
    Q_OBJECT

public:
    QvkCameraController( Ui_MainWindow *value );
    virtual ~QvkCameraController();


public slots:


private slots:
    void slot_loadCamera();
    void slot_Start_Stop_Camera(bool value);
    void slot_new_camera_list( QList<QCameraInfo> cameraInfoList );
    void slot_error( QCamera::Error error );
    void slot_statusChanged( QCamera::Status status );
    void slot_stateChanged( QCamera::State state );
    void slot_newImage( QImage image );


private:
    Ui_MainWindow *ui_vokoscreen;
    QCamera *camera = nullptr;
    QvkWebcamWindow *webcamWindow = nullptr;
    QvkVideoSurface *videoSurface;


protected:


signals:


};

#endif
