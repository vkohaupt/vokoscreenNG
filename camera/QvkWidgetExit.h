#ifndef QvkWidgetExit_H
#define QvkWidgetExit_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class QvkWidgetExit: public QWidget
{
    Q_OBJECT
public:    
    virtual ~QvkWidgetExit();
    QvkWidgetExit( QWidget *parent );

    
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
    void clicked();

};

#endif // QvkWidgetExit_H
