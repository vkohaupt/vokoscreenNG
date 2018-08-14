#ifndef screencast_H 
#define screencast_H

#include <QMainWindow>
#include <QDebug>
#include <QDial>

#include "circleWidget.h"
#include "frameWidget.h"
#include "ninetyWidget.h"
#include "resizeTopRightWidget.h"
#include "settingsWidget.h"

class screencast: public QMainWindow
{
Q_OBJECT
public:    
    screencast();
    virtual ~screencast();
    
public:


public slots:

  
private:
  circleWidget *mirrorWidget;
  frameWidget *myFrameWidget;
  ninetyWidget *myNinetyWidget;
  settingsWidget *mySettingsWidget;
  resizeTopRightWidget *resizeTopRight;
  QLabel *label;
  QDial *rotateDial;
  QImage* qImage;

private slots:
  void mirrored( bool value );
  void frameOnOff( bool value );
  void switchRadioButton( QString value);

  
protected:
   void enterEvent( QEvent* );
   void leaveEvent( QEvent* );
   void resizeEvent ( QResizeEvent * );

  
};

#endif // screencast_H
