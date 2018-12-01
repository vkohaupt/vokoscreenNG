#ifndef QvkAudioPulse_H
#define QvkAudioPulse_H

#include "ui_formMainWindow.h"

#include "QvkThreadPulse.h"
#include "QvkPulseGstr.h"

#include <QObject>

class QvkAudioPulse: public QObject
{
    Q_OBJECT

public:
    QvkAudioPulse();
    QvkAudioPulse( Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkAudioPulse();
    

public slots:


private:
    Ui_formMainWindow *ui;
    QvkThreadPulse *vkThreadPulse;
    int counter;
    void getPulseDevices();
    void clearVerticalLayoutAudioDevices();


private slots:
    void slot_update(int count);
    void slot_set_counter_back();


protected:

  
signals:

    
};

#endif
