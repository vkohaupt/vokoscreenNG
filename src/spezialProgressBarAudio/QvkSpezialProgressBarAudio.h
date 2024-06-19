
#ifndef QVKSPEZIALPROGRESSBARAUDIO_H
#define QVKSPEZIALPROGRESSBARAUDIO_H

#include <QProgressBar>
#include <QPainter>
#include <QPaintEvent>

class QvkSpezialProgressBarAudio : public QProgressBar
{
    Q_OBJECT

public:
    QvkSpezialProgressBarAudio(QProgressBar *parent = 0);
    void set_Text( QString text );


public slots:


protected:
    void paintEvent( QPaintEvent *event ) override;


private:
    QString text;


signals:

    
};
#endif
