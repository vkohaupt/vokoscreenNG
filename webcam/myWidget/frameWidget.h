#ifndef frameWidget_H 
#define frameWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>

class frameWidget: public QWidget
{
Q_OBJECT
public:    
    virtual ~frameWidget();
    frameWidget( QWidget *parent );

    
public:

  
public slots:
  void setColor( const QColor color );
  bool isFrameSet();

  
private:
  QColor widgetColor;
  bool set;

  
private slots:

  
protected:
  void paintEvent( QPaintEvent *event );
  void mouseReleaseEvent( QMouseEvent * event );
  
  
signals:
  void clicked( bool set );

};

#endif // frameWidget_H