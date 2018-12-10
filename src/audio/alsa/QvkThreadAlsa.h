#ifndef QvkThreadAlsa_H
#define QvkThreadAlsa_H

#include "ui_formMainWindow.h"
#include <QObject>
#include <QThread>
#include <QTimer>

class QvkThreadAlsa: public QThread
{
    Q_OBJECT
    void run() override;


public:
    QvkThreadAlsa();
    virtual ~QvkThreadAlsa();
    QTimer *timer;


public slots:
    void slot_start_stop_thread_timer( bool value );
    void slot_start_thread_timer();


private:
    bool running = false;
    bool firstStart = true;


private slots:
    void slot_trigger_Thread();
    void slot_set_first_start( bool value );

    void slot_stop_thread_timer();


protected:
  
  
signals:
    void signal_countAudioDevices( int count );

    
};

#endif
