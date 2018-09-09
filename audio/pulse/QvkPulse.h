#ifndef QvkPulse_H 
#define QvkPulse_H

#include "ui_mainwindow.h"
#include <QObject>
#include <gst/gst.h>

class QvkPulse: public QObject
{
    Q_OBJECT

public:
    QvkPulse(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkPulse();

    
public slots:
    void slot_getPulsesDevices( bool value );


private:
    Ui_MainWindow *ui;
    QString get_AudioDeviceString( GstDevice *device );
    QStringList get_all_Audio_devices();


private slots:


protected:
  
  
signals:
    void signal_audioDeviceRemovedOrAdded();

    
};

#endif
