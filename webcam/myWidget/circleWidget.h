#ifndef circleWidget_H 
#define circleWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class circleWidget: public QWidget
{
Q_OBJECT
public:    
    virtual ~circleWidget();
    circleWidget( QWidget *parent );

    
public:

  
public slots:
  void setColor( const QColor color );
  void unsetPoint();
  void setPoint();

  
private:
  QColor widgetColor;
  bool set;

private slots:

  
protected:
  void paintEvent( QPaintEvent *event );
  void mouseReleaseEvent ( QMouseEvent * event );

  
signals:
  void clicked( bool value );

  
};

#endif // circleWidget_H