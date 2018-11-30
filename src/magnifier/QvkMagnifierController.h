#ifndef QvkMagnifierController_H
#define QvkMagnifierController_H

#include "ui_mainwindow.h"
#include "QvkMagnifier.h"

class QvkMagnifierController : public QObject
{
    Q_OBJECT

public:
   QvkMagnifierController( Ui_formMainWindow *ui_mainwindow );
   virtual ~QvkMagnifierController();

  
public slots:


private slots:


private:
  Ui_formMainWindow *ui_vokoscreen;
  QvkMagnifier *vkMagnifier;


protected:


signals:


};

#endif
