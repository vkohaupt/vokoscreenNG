
#ifndef QVKSPEZIALPROGRESSBARAUDIO_H
#define QVKSPEZIALPROGRESSBARAUDIO_H

#include <QProgressBar>
#include <QPainter>

class QvkSpezialProgressBarAudio : public QProgressBar
{
    Q_OBJECT

public:
    QvkSpezialProgressBarAudio(QProgressBar *parent = 0);
;


public slots:


protected:
    void paintEvent(QPaintEvent *event) override;


private:


signals:

    
};
#endif
