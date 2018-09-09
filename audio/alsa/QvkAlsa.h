#ifndef QvkAlsa_H 
#define QvkAlsa_H

#include "ui_mainwindow.h"
#include <QObject>

class QvkAlsa: public QObject
{
    Q_OBJECT
    
public:
    QvkAlsa(  Ui_MainWindow *ui_mainwindow );
    virtual ~QvkAlsa();

    
public slots:
    void slot_getAlsaDevices( bool value );

  
private:
    Ui_MainWindow *ui;

    
private slots:


protected:
  
  
signals:

    
};

#endif
