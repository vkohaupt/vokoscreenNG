#ifndef QvkAudioAlsa_H 
#define QvkAudioAlsa_H

#include "ui_mainwindow.h"
#include <QObject>
#include <QTimer>

class QvkAudioAlsa: public QObject
{
    Q_OBJECT
    
public:
    QvkAudioAlsa(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAudioAlsa();

    
public slots:
    void slot_start( bool value );

  
private:
    Ui_MainWindow *ui;
    QTimer *timer;
    int counter;


private slots:
    void slot_getAlsaDevices();
    void slot_clearVerticalLayoutAudioDevices();
    void slot_update();


protected:
  
  
signals:

    
};

#endif
