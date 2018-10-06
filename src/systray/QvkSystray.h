#ifndef QvkSystray_H
#define QvkSystray_H

#include "ui_mainwindow.h"

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

class QvkSystray: public QSystemTrayIcon
{
    Q_OBJECT

public:
    QvkSystray(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkSystray();
    void init();

    
public slots:
    void slot_closeSystray();


private:
    Ui_MainWindow *ui;
    QMenu *menu;
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *continueAction;
    QAction *hideAction;
    QAction *exitAction;


private slots:
    void slot_hide();


protected:
  
  
signals:
    void signal_SystemtrayIsClose();
    
};

#endif
