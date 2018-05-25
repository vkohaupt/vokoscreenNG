#ifndef QvkCountdown_H 
#define QvkCountdown_H

#include <QWidget>
#include <QPainter>

class QvkCountdown: public QWidget
{
    Q_OBJECT
public:
    QvkCountdown();
    virtual ~QvkCountdown();
    void startCountdown(int value );

    
public slots:

  
private:
    int countValue;
    int oldCountValue;
    int gradValue;
    QTimer *timer;
    QTimer *animationTimer;
    QPainter painter;
    int Width;
    int Height;
    int x;
    int y;

    
private slots:
   void updateTimer();
   void updateAnimationTimer();


protected:
    void paintEvent( QPaintEvent *event );
  
  
signals:
    void signal_countDownfinish( bool value );
    void signal_countdownBegin( bool value );

};

#endif
