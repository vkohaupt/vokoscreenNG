#ifndef QvkAudioPulse_H
#define QvkAudioPulse_H

#include "ui_mainwindow.h"
#include <QObject>
#include <gst/gst.h>

class QvkAudioPulse: public QObject
{
    Q_OBJECT

public:
    QvkAudioPulse(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAudioPulse();
    bool isPulseAvailable();

    
public slots:
    void slot_getPulsesDevices( bool value );


private:
    Ui_MainWindow *ui;
    QString get_AudioDeviceString( GstDevice *device );
    QStringList get_all_Audio_devices();
    bool pulseAvailable;


private slots:


protected:
  
  
signals:
    void signal_audioDeviceRemovedOrAdded();

    
};

#endif
