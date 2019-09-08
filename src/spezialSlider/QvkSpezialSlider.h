#ifndef QVKSPEZIALSLIDER_H
#define QVKSPEZIALSLIDER_H

#include <QWidget>
#include <QPainter>
#include <QSlider>
#include <QMouseEvent>

#include "slidervideo.h"

class QvkSpezialSlider : public QSlider
{
    Q_OBJECT

public:
    explicit QvkSpezialSlider( Qt::Orientation orientation );
    ~QvkSpezialSlider();


private:
    QPen pen;
    qreal radius;
    qreal lineHight;
    qreal distance;
    qreal handleRadius;
    bool mousePressed = false;
    QColor vk_get_color(QPalette::ColorRole colorRole);
    bool showValue = true;


public slots:
    void setShowValue( bool value );


protected:
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );

signals:
    void sliderMoved( int );
    void signal_sliderVideo_KeyRight_KeyLeft( int value );

};

#endif // QVKSPEZIALSLIDER_H
