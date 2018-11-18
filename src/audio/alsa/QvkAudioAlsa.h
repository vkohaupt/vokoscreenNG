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

  
private:
    Ui_MainWindow *ui;
    int counter;
    QvkThreadAlsa *vkThreadAlsa;
    void clearVerticalLayoutAudioDevices();
    void getAlsaDevices();


private slots:
    void slot_update(int count);
    void slot_set_counter();


protected:
  
  
signals:

    
};

#endif
