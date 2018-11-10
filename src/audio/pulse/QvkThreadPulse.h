#ifndef QvkThreadPulse_H
#define QvkThreadPulse_H

#include "ui_mainwindow.h"
#include <QObject>
#include <QThread>


class QvkThreadPulse: public QThread
{
    Q_OBJECT
    void run() override;


public:
    QvkThreadPulse();
    virtual ~QvkThreadPulse();

    
public slots:
    void slot_startThread( bool value );


private:
    bool running = false;

private slots:


protected:
  
  
signals:
    void signal_countAudioDevices( int count );

    
};

#endif
