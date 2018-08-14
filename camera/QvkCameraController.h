#ifndef QvkCameraController_H
#define QvkCameraController_H

#include "ui_mainwindow.h"
//#include "QvkMsgInWebcamWindow.h"
#include "QvkCameraWatcher.h"

#include <QCamera>
#include <QComboBox>

class QvkCameraController : public QObject
{
    Q_OBJECT

public:
   QvkCameraController(Ui_MainWindow *ui_surface );
   virtual ~QvkCameraController();

  
public slots:


private slots:
   void slot_addedCamera( QString description, QString device );
   void slot_removedCamera( QString device );
   void slot_startCamera( bool value );

   void slot_statusChanged( QCamera::Status status );
   void slot_stateChanged( QCamera::State state );
   void slot_error( QCamera::Error error );


private:
  Ui_MainWindow *ui_vokoscreen;
  QCamera *camera = nullptr;
  QVideoWidget *videoWidget = nullptr;
  QvkCameraWatcher *cameraWatcher;


protected:


signals:


};

#endif
