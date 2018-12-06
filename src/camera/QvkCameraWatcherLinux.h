#ifndef QvkWebcamWatcher_H
#define QvkWebcamWatcher_H

#include <QStringList>
#include <QDir>
#include <QFileSystemWatcher>
#include <QDebug>

class QvkCameraWatcher: public QObject
{

Q_OBJECT

public:
  QvkCameraWatcher();
  virtual ~QvkCameraWatcher();
  void cameraWatcherInit();


public slots:
  void myfileSystemWatcher( QString path );


private slots:


signals:
  void signal_removedCamera( QString value );
  void signal_addedCamera( QString description, QString deviceName );


protected:


private:
  QStringList oldDeviceList;
  int cameraCount;

};

#endif
