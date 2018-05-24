#ifndef QvkCountdown_H 
#define QvkCountdown_H

#include <QWidget>
#include <QPainter>

class QvkCountdown: public QWidget
{
    Q_OBJECT
public:
    QvkCountdown(int value );
    virtual ~QvkCountdown();

    
public slots:

  
private:
    int countValue;
    int oldCountValue;
    int gradValue;
    QTimer *timer;
    QTimer *animationTimer;
    QPainter painter;

    
private slots:
   void updateTimer();
   void updateAnimationTimer();


protected:
    void paintEvent( QPaintEvent *event );
  
  
signals:
    void signal_countDownfinish();
  
};

#endif
