#ifndef resizeTopRightWidget_H 
#define resizeTopRightWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

class resizeTopRightWidget: public QWidget
{
Q_OBJECT
public:    
    virtual ~resizeTopRightWidget();
    resizeTopRightWidget( QWidget *parent );

    
public:

  
public slots:
  void setColor( const QColor color );

  
private:
  QColor widgetColor;
  bool set;

  
private slots:

  
protected:
  void paintEvent( QPaintEvent *event );
  
  
signals:

};

#endif // resizeTopRightWidget_H