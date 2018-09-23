#ifndef QvkHelp_H 
#define QvkHelp_H

#include "ui_mainwindow.h"
#include <QObject>

#ifdef Q_OS_LINUX
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#endif

class QvkHelp: public QObject
{
    Q_OBJECT
    
public:
    QvkHelp(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkHelp();
    void initHelp();

    
public slots:
    void slot_close();

  
private:
    Ui_MainWindow *ui;
    QString vk_helpPath;


#ifdef Q_OS_LINUX
    QWebEngineProfile *webEngineProfile;
    QWebEnginePage *webEnginePage;
    QWebEngineView *webEngineView;
#endif
    

private slots:
    void slot_screenFullscreen();
    void slot_screenWindow();
    void slot_screenArea();
    void slot_screenCountdown();

    void slot_audioHelp();

    void slot_miscHelpVideoPath();
    void slot_miscHelpStartTime();
    void slot_miscHelpStopRecordingAfter();
    void slot_miscHelpScal();
    void slot_miscHelpLimitOfFreeDiskSpace();

    void slot_availableHelp();


protected:
  
  
signals:

    
};

#endif
