#ifndef QvkPulseGstr_H
#define QvkPulseGstr_H

#include <gst/gst.h>

#include <QObject>

class QvkPulseGstr: public QObject
{
    Q_OBJECT


public:
    QvkPulseGstr();
    virtual ~QvkPulseGstr();
    QString get_AudioDeviceString( GstDevice *device );
    QStringList get_all_Audio_devices();

    
public slots:


private:


private slots:


protected:
  
  
signals:

    
};

#endif
