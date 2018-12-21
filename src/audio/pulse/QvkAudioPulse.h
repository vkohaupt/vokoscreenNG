#ifndef QvkAudioPulse_H
#define QvkAudioPulse_H

#include "ui_formMainWindow.h"

#include "QvkWatcherPulse.h"
#include "QvkPulseGstr.h"

#include <QObject>

class QvkAudioPulse: public QObject
{
    Q_OBJECT

public:
    QvkAudioPulse();
    QvkAudioPulse( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkAudioPulse();
    

public slots:


private:
    Ui_formMainWindow *ui;
    QvkWatcherPulse *vkWatcherPulse;
    int counter;
    void getPulseDevices();
    void clearVerticalLayoutAudioDevices();


private slots:
    void slot_update(int count);
    void slot_set_counter_back();
    void slot_cleanup();
    void slot_start_thread_timer();


protected:

  
signals:

    
};

#endif
