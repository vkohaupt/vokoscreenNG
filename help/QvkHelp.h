#ifndef QvkHelp_H 
#define QvkHelp_H

#include "ui_mainwindow.h"
#include <QObject>

class QvkHelp: public QObject
{
    Q_OBJECT
    
public:
    QvkHelp(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkHelp();
    void initHelp();

    
public slots:

  
private:
    Ui_MainWindow *ui;

    
private slots:
    void slot_audioHelp();
    void slot_availableHelp();
    void slot_miscHelpVideoPath();
    void slot_miscHelpStartTime();


protected:
  
  
signals:

    
};

#endif
