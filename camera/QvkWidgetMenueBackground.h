#ifndef QvkWidgetMenueBackground_H 
#define QvkWidgetMenueBackground_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class QvkWidgetMenueBackground: public QWidget
{
    Q_OBJECT
public:    
    virtual ~QvkWidgetMenueBackground();
    QvkWidgetMenueBackground( QWidget *parent );

    
public:


public slots:


private:


private slots:


protected:
    void paintEvent( QPaintEvent *event );


signals:

};

#endif // QvkWidgetMenueBackground_H
