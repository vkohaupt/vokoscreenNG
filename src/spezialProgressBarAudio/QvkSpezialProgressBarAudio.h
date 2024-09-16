
#ifndef QVKSPEZIALPROGRESSBARAUDIO_H
#define QVKSPEZIALPROGRESSBARAUDIO_H

#include <QProgressBar>
#include <QPainter>
#include <QPaintEvent>

#include <QMouseEvent>

class QvkSpezialProgressBarAudio : public QProgressBar
{
    Q_OBJECT

public:
    QvkSpezialProgressBarAudio(QProgressBar *parent = 0);
    void set_Text( QString text );
    QString get_Text();

public slots:


protected:
    void paintEvent( QPaintEvent *event ) override;

    void mouseReleaseEvent(QMouseEvent *event);



private:
    QString text;


signals:
    void signal_mouseReleaseEvent();
    
};
#endif
