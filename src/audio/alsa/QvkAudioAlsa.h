#ifndef QvkAudioAlsa_H 
#define QvkAudioAlsa_H

#include "ui_formMainWindow.h"
#include "QvkThreadAlsa.h"

#include <QObject>

class QvkAudioAlsa: public QObject
{
    Q_OBJECT
    
public:
    QvkAudioAlsa( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkAudioAlsa();

    
public slots:

  
private:
    Ui_formMainWindow *ui;
    int counter;
    QvkThreadAlsa *vkThreadAlsa;
    void clearVerticalLayoutAudioDevices();
    void getAlsaDevices();


private slots:
    void slot_update(int count);
    void slot_set_counter();
    void slot_cleanup();
    void slot_start_thread_timer();


protected:
  
  
signals:

    
};

#endif
