#ifndef ninetyWidget_H 
#define ninetyWidget_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "circleWidget.h"

class ninetyWidget: public QWidget
{
Q_OBJECT
public:    
    virtual ~ninetyWidget();
    ninetyWidget( QWidget *parent );

    
public:

  
public slots:
  void setColor( const QColor color);
  void switchTop();
  void switchRight();
  void switchBottom();
  void switchLeft();


  
private:
  circleWidget *ninetyWidgetTop;
  circleWidget *ninetyWidgetRight;
  circleWidget *ninetyWidgetBottom;
  circleWidget *ninetyWidgetLeft;
  int radioSize;
  
private slots:
  
protected:
  void resizeEvent ( QResizeEvent * event );

  
signals:
  void switchRadioButton( QString value );
  
};

#endif // ninetyWidget_H