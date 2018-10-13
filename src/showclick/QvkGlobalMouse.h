#ifndef QvkGlobalMouse_H
#define QvkGlobalMouse_H

#include <QObject>

class QvkGlobalMouse: public QObject
{
Q_OBJECT
public:    
    QvkGlobalMouse();
    virtual ~QvkGlobalMouse();
    
    
public:


public slots:
  void setCursorOn();
  void setCursorOff();
  void mousePressed();

  
private:
  bool onOff;
  
  
private slots:
  
  
protected:

  
signals:
  void mousePressed( int win_x, int win_y );
  
};

#endif // QvkGlobalMouse_H
