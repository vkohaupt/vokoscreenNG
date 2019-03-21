#ifndef QvkCameraController_H
#define QvkCameraController_H

#include "ui_formMainWindow.h"

#ifdef Q_OS_LINUX
#include "QvkCameraWatcherLinux.h"
#endif
#ifdef Q_OS_WIN
#include "QvkCameraWatcherWindows.h"
#endif

#include "QvkCameraWindow.h"
#include "QvkVideoSurface.h"

#include <QCamera>
#include <QComboBox>

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

   void slot_frameOnOff( bool value );
   void slot_sliderMoved( int value );


private:
  Ui_formMainWindow *ui_formMainWindow;
  QCamera *camera = nullptr;
  QvkCameraWatcher *cameraWatcher;
  QvkCameraWindow *cameraWindow;
  QvkVideoSurface *videoSurface;


protected:


signals:


};

#endif
