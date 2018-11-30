#ifndef QvkGlobalShortcut_H
#define QvkGlobalShortcut_H

#include "ui_mainwindow.h"
#include "qglobalshortcut.h"

#include <QObject>
#include <QMainWindow>

class QvkGlobalShortcut: public QObject
{
    Q_OBJECT
    
public:
    QvkGlobalShortcut( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkGlobalShortcut();


public slots:

  
private:
    Ui_formMainWindow *ui;


private slots:
    void slot_pauseContinue();

    
protected:
  
    
signals:

    
};

#endif
