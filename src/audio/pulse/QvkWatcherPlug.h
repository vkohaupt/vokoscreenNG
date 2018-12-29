#ifndef QvkWatcherPlug_H
#define QvkWatcherPlug_H

#include <gst/gst.h>

#include <QObject>

class QvkWatcherPlug: public QObject
{
    Q_OBJECT


public:
    QvkWatcherPlug();
    virtual ~QvkWatcherPlug();

    
public slots:
    GstDeviceMonitor *start_monitor();


private:


private slots:


protected:
  
  
signals:

    
};

#endif
