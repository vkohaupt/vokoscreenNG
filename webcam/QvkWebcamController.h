#ifndef QvkWebcamController_H 
#define QvkWebcamController_H

#include "ui_mainwindow.h"
#include "QvkVideoSurface.h"
#include <qvkwebcamwindow.h>

#include <QCamera>
#include <QCameraInfo>

class QvkWebcamController : public QObject
{
    Q_OBJECT

public:
    QvkWebcamController( Ui_MainWindow *value );
    virtual ~QvkWebcamController();


public slots:


private slots:
    void slot_setCamera();
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
