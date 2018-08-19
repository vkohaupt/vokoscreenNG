#ifndef frameWidget_H 
#define frameWidget_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class frameWidget: public QWidget
{
    Q_OBJECT
public:    
    virtual ~frameWidget();
    frameWidget( QWidget *parent );

    
public:


public slots:


private:
    bool set;
    bool iconMode;


private slots:


protected:
    void paintEvent( QPaintEvent *event );
    void mouseReleaseEvent( QMouseEvent * event );
    void enterEvent( QEvent *event );
    void leaveEvent( QEvent *event );


signals:
    void clicked( bool set );

};

#endif // frameWidget_H
