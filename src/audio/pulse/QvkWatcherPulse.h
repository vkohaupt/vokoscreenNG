#ifndef QvkWatcherPulse_H
#define QvkWatcherPulse_H

#include "QvkPulseGstr.h"

#include "ui_formMainWindow.h"
#include <QObject>
#include <QTimer>


class QvkWatcherPulse: public QObject
{
    Q_OBJECT


public:
    QvkWatcherPulse();
    virtual ~QvkWatcherPulse();
    QTimer *timer;

    
public slots:
    void slot_start_stop_thread_timer( bool value );
    void slot_start_thread_timer();


private:
    bool running = false;
    bool firstStart;
    int zaehler = 0;


private slots:
    void slot_trigger();
    void slot_set_first_start( bool value );
    void slot_stop_thread_timer();


protected:
  
  
signals:
    void signal_countAudioDevices( int count );

    
};

#endif
