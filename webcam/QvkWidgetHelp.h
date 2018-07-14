#ifndef QvkWidgetHelp_H 
#define QvkWidgetHelp_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class QvkWidgetHelp: public QWidget
{
Q_OBJECT
public:    
    virtual ~QvkWidgetHelp();
    QvkWidgetHelp( QWidget *parent );

    
public:

  
public slots:

  
private:
    bool iconMode;

  
private slots:

  
protected:
  void paintEvent( QPaintEvent *event );
  void mouseReleaseEvent( QMouseEvent * event );
  void enterEvent( QEvent *event );
  void leaveEvent( QEvent *event );

  
signals:

    
};

#endif // QvkWidgetHelp_H
