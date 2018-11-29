#ifndef QvkThreadPulse_H
#define QvkThreadPulse_H

#include "QvkPulseGstr.h"

#include "ui_mainwindow.h"
#include <QThread>
#include <QTimer>


class QvkThreadPulse: public QThread
{
    Q_OBJECT
    void run() override;


public:
    QvkThreadPulse();
    virtual ~QvkThreadPulse();

    
public slots:
    void slot_start_stop_thread_timer( bool value );


private:
    bool running = false;
    QTimer *timer;
    bool firstStart;
    int zaehler = 0;

private slots:
    void slot_trigger_Thread();
    void slot_set_first_start( bool value );

    void slot_start_thread_timer();
    void slot_stop_thread_timer();

protected:
  
  
signals:
    void signal_countAudioDevices( int count );

    
};

#endif
