#ifndef QvkAnimateControl_H
#define QvkAnimateControl_H

#include "QvkGlobalMouse.h"
#include "QvkAnimateWindow.h"

#include <QObject>

class QvkAnimateControl: public QObject
{
Q_OBJECT
public:    
   QvkAnimateControl( double time, int diameter, Qt::CheckState radiant, double opacity, QColor color );
   virtual ~QvkAnimateControl();
    

public slots:
  void animateWindowOn();
  void animateWindowOff();
  void setDiameterColor( int diameter, QColor color );
  void setShowTime( double value );
  void setOpacity( double value );
  void setRadiant( bool value );
  void pointerOnOff( bool value );

  
private:
  QvkGlobalMouse *globalMouse;
  QvkAnimateWindow *animateWindow;
  double showTime;

  
private slots:
  void mousePressed( int x, int y );
  void closeWindow();

  
protected:

  
};

#endif // QvkAnimateControl_H
