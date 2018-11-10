#ifndef QvkAudioPulse_H
#define QvkAudioPulse_H

#include "QvkThreadPulse.h"

#include "ui_mainwindow.h"
#include <QObject>

#include <gst/gst.h>


class QvkAudioPulse: public QObject
{
    Q_OBJECT

public:
    QvkAudioPulse(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAudioPulse();
    QvkThreadPulse *vkThreadPulse;
    

public slots:
    void slot_start( bool value );
    void slot_clearVerticalLayoutAudioDevices();
    void slot_update(int count);


private:
    Ui_MainWindow *ui;
    QString get_AudioDeviceString( GstDevice *device );
    QStringList get_all_Audio_devices();
    QTimer *timer;
    int counter;
    void getPulseDevices();


private slots:


protected:
  
  
signals:

    
};

#endif
