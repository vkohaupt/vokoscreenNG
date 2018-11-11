#ifndef QvkThreadAlsa_H
#define QvkThreadAlsa_H

#include "ui_mainwindow.h"
#include <QObject>
#include <QThread>


class QvkThreadAlsa: public QThread
{
    Q_OBJECT
    void run() override;


public:
    QvkThreadAlsa();
    virtual ~QvkThreadAlsa();

    
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
