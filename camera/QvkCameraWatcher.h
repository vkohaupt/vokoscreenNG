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

  
private slots:
  void detectCameras();
  QString addedDeviceName( QStringList myNewDeviceNameList, QString device );
  QString removedDeviceName( QStringList myNewDeviceNameList, QStringList myOldDeviceNameList );


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
};

#endif
