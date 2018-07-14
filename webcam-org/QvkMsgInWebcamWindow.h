#ifndef QvkMsgInWebcamWindow_H 
#define QvkMsgInWebcamWindow_H

#include "QvkWebcamWindow.h"


class QvkMsgInWebcamWindow: public QLabel
{
  
Q_OBJECT

public:
  QvkMsgInWebcamWindow( QObject *parent, QWidget *webcamWindow );
  virtual ~QvkMsgInWebcamWindow();


public slots:
  void setMsg( QString value );

  
private slots:
  void setNewSize( QSize size );

  
signals:

    
protected:  

  
private:


};

#endif
