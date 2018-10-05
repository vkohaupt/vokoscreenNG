#ifndef QvkAudioWindows_H 
#define QvkAudioWindows_H

#include "ui_mainwindow.h"
#include <QObject>

class QvkAudioWindows: public QObject
{
    Q_OBJECT
    
public:
    QvkAudioWindows(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAudioWindows();

    
public slots:
    void slot_getWindowsDevices();

  
private:
    Ui_MainWindow *ui;

    
private slots:


protected:
  
  
signals:

    
};

#endif
