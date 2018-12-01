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
    

public slots:
    void slot_start_stop_thread_timer( bool value );


private:
    bool running = false;
    QTimer *timer;
    bool firstStart = true;


private slots:
    void slot_trigger_Thread();
    void slot_set_first_start( bool value );


protected:
  
  
signals:
    void signal_countAudioDevices( int count );

    
};

#endif
