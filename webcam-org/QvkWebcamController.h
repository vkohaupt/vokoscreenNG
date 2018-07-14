#ifndef QvkWebcamController_H 
#define QvkWebcamController_H

#include "QvkWebcamWindow.h"
#include "QvkVideoSurface.h"
//#include "QvkSettings.h"
//#include "ui_vokoscreen.h"
#include "ui_mainwindow.h"
#include "QvkMsgInWebcamWindow.h"

#include <QCamera>

class QvkWebcamController : public QObject
{
    Q_OBJECT

public:
   QvkWebcamController( Ui_MainWindow *value);
   virtual ~QvkWebcamController();
   QvkWebcamWindow *webcamWindow;
   QvkMsgInWebcamWindow *msgInWebcamWindow;

  
public slots:
   void displayWebcam( QByteArray device );


private slots:
  void myStatusChanged(QCamera::Status status );
  void myStateChanged( QCamera::State state );
  void webcamOnOff(int value );
  void addToComboBox(QStringList description, QStringList device );
  void ifCameraRemovedCloseWindow(QString value);
  void setActiveCamera( QString value );
  QString getActiveCamera();
  void setNewImage( QImage image );
  void setMirrorOnOff( bool value );
  void rotateDialclicked();
  //void setCheckboxWebcamFromSettings();
#ifndef Q_OS_WIN
  void overFullScreenWebcamCheckBox_OnOff();
#endif
  void resolution(int index );


private:
  QCamera *camera;
  QString aktivCamera;
  QvkVideoSurface *videoSurface;
  //QvkSettings vkSettings;
  bool mirrored;
  Ui_MainWindow *myUi;


protected:
  
signals:
  void closeWebcamWindow();
  void webcamBusy();

};

#endif
