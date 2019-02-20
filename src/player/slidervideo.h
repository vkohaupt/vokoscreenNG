#ifndef SLIDERVIDEO_H
#define SLIDERVIDEO_H

#include <QSlider>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>

// Hint: Defined in GUI as placeholder

class slidervideo:public QSlider
{
    Q_OBJECT

public:
    slidervideo(QWidget *parent=0)
    {
        Q_UNUSED(parent);
    }


public slots:


protected:
    void mousePressEvent ( QMouseEvent * event )
    {
        if ( event->button() == Qt::LeftButton )
        {
            if ( orientation() == Qt::Vertical )
            {
                setValue( minimum() + ( ( maximum()-minimum() ) * ( height()-event->y() ) ) / height() ) ;
            }
            else
            {
                setValue(minimum() + ( (maximum()-minimum() ) * event->x() ) / width() ) ;
            }
            event->accept();
        }
        QSlider::mousePressEvent(event);
    }


    void keyPressEvent( QKeyEvent *event )
    {
        if ( ( event->key() == Qt::Key_Right ) or ( event->key() == Qt::Key_Left  ) )
        {
            emit signal_sliderVideo_KeyRight_KeyLeft( value() );
            event->accept();
        }
        QSlider::keyPressEvent( event );
    }


signals:
    void signal_sliderVideo_KeyRight_KeyLeft( int value );


};
#endif // SLIDERVIDEO_H
