#ifndef QvkAudioAlsa_H 
#define QvkAudioAlsa_H

#include "ui_mainwindow.h"
#include <QObject>

class QvkAudioAlsa: public QObject
{
    Q_OBJECT
    
public:
    QvkAudioAlsa(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAudioAlsa();

    
public slots:
    void slot_getAlsaDevices( bool value );

  
private:
    Ui_MainWindow *ui;

    
private slots:


protected:
  
  
signals:

    
};

#endif
