#ifndef QvkWebcamWatcher_H 
#define QvkWebcamWatcher_H

#include <QObject>
#include <QCameraInfo>
#include <QTimer>

class QvkWebcamWatcher: public QObject
{
  
Q_OBJECT

public:
  QvkWebcamWatcher();
  virtual ~QvkWebcamWatcher();


public slots:

  
private slots:
  void slot_detectCameras();

  
signals:
  void signal_new_list_a_camera_was_added( QList<QCameraInfo> );
  void signal_new_list_a_camera_was_removed( QList<QCameraInfo> );


protected:  

  
private:
  QTimer *timer;
  int oldCount;


};

#endif
