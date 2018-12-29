#ifndef QvkAudioPulse_H
#define QvkAudioPulse_H

#include "ui_formMainWindow.h"

#include "QvkWatcherPulse.h"
#include "QvkPulseGstr.h"

#include <QObject>
#include <QFileSystemWatcher>

class QvkAudioPulse: public QObject
{
    Q_OBJECT

public:
    QvkAudioPulse( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkAudioPulse();
    void init();


public slots:


private:
    Ui_formMainWindow *ui;
    QvkWatcherPulse *vkWatcherPulse;
    void clearVerticalLayoutAudioDevices();
    void getPulseDevices();
    QFileSystemWatcher *fileSystemWatcher;

private slots:
    void slot_myfileSystemWatcher( QString string );


protected:

  
signals:
    void signal_noAudioDevicesAvalaible( bool );

};

#endif
