#ifndef QvkCameraWatcher_H 
#define QvkCameraWatcher_H

#include <QStringList>
#include <QObject>
#include <QTimer>

class QvkCameraWatcher: public QObject
{
  
Q_OBJECT

public:
  QvkCameraWatcher();
  virtual ~QvkCameraWatcher();


public slots:
  void slot_startStopCameraTimer( bool value );

  
private slots:
  void slot_detectCameras();


signals:
  void signal_removedCamera( QString value );
  void signal_addedCamera( QString description, QString deviceName );


protected:  

  
private:
  int newcount;
  QStringList newDescriptionList;
  QStringList newDeviceNameList;
  QStringList oldDeviceNameList;

  QTimer *timer;

  QString addedDeviceName( QStringList myNewDeviceNameList, QString device );
  QString removedDeviceName( QStringList myNewDeviceNameList, QStringList myOldDeviceNameList );

};

#endif
