#ifndef QvkAudioAlsa_H 
#define QvkAudioAlsa_H

#include "ui_mainwindow.h"
#include "QvkThreadAlsa.h"

#include <QObject>

class QvkAudioAlsa: public QObject
{
    Q_OBJECT
    
public:
    QvkAudioAlsa( Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAudioAlsa();

    
public slots:
    void slot_start( bool value );

  
private:
    Ui_MainWindow *ui;
    int counter;
    QvkThreadAlsa *vkThreadAlsa;


private slots:
    void slot_getAlsaDevices();
    void slot_clearVerticalLayoutAudioDevices();
    void slot_update(int count);


protected:
  
  
signals:

    
};

#endif
