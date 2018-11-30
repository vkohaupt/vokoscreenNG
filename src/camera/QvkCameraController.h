#ifndef QvkCameraController_H
#define QvkCameraController_H

#include "ui_mainwindow.h"
#include "QvkCameraWatcher.h"
#include "QvkCameraWindow.h"

#include "QvkSettings.h"
#include "QvkVideoSurface.h"

#include <QCamera>
#include <QComboBox>

#include <QVideoWidget>

class QvkCameraController : public QObject
{
    Q_OBJECT

public:
   QvkCameraController(Ui_formMainWindow *ui_surface );
   virtual ~QvkCameraController();

  
public slots:


private slots:
   void slot_addedCamera( QString description, QString device );
   void slot_removedCamera( QString device );
   void slot_startCamera( bool value );

   void slot_statusChanged( QCamera::Status status );
   void slot_stateChanged( QCamera::State state );
   void slot_error( QCamera::Error error );

   void slot_setNewImage( QImage image );

   void slot_radioButtonLeftMiddle();
   void slot_radioButtonTopMiddle();
   void slot_radioButtonRightMiddle();
   void slot_radioButtonBottomMiddle();


private:
  QvkSettings vkSettings;
  Ui_formMainWindow *ui_vokoscreen;
  QCamera *camera = nullptr;
  QvkCameraWatcher *cameraWatcher;
  QvkCameraWindow *cameraWindow;
  //QVideoWidget *videoWidget = nullptr;
  QvkVideoSurface *videoSurface;



protected:


signals:


};

#endif
