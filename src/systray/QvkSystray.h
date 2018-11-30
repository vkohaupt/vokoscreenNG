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
    QvkSystray( Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkSystray();
    void init();

    
public slots:
    void slot_closeSystray();


private:
    Ui_formMainWindow *ui;
    QMenu *menu;
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *continueAction;
    QAction *hideAction;
    QAction *exitAction;


private slots:
    void slot_hide();
    void slot_setRecordIcon( bool );
    void slot_setSystrayIcon( bool );
    void slot_setPauseIcon( bool );


protected:
  
  
signals:
    void signal_SystemtrayIsClose();
    
};

#endif
