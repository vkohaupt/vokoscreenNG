#ifndef QvkCircleWidget_H 
#define QvkCircleWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class QvkCircleWidget: public QWidget
{
Q_OBJECT
public:    
    virtual ~QvkCircleWidget();
    QvkCircleWidget( QWidget *parent );
    QWidget *parentWidget;
    
public:

  
public slots:
  void setColor( QColor color );
  void setDiameter( int value );
  void setOpacity( double value );
  void setRadiant( bool value );

  QColor getColor();
  int getDiameter();
  double getOpacity();
  bool getRadiant();

private:
  QColor pointerColor;
  int diameter;
  double pointerOpacity;
  bool radiant;
  
private slots:

  
protected:
  void paintEvent( QPaintEvent *event );

  
signals:

  
};

#endif // QvkCircleWidget_H