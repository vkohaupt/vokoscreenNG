#ifndef QvkMagnifierController_H
#define QvkMagnifierController_H

#include "ui_formMainWindow.h"
#include "QvkMagnifier.h"

class QvkMagnifierController : public QObject
{
    Q_OBJECT

public:
   QvkMagnifierController( Ui_formMainWindow *ui_mainwindow );
   virtual ~QvkMagnifierController();

  
public slots:


private slots:
   void slot_valueChanged( int value );


private:
  QvkMagnifier *vkMagnifier;


protected:


signals:


};

#endif
