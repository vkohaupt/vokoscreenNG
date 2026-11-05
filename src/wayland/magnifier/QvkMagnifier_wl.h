#ifndef QVKMAGNIFIER_WL_H
#define QVKMAGNIFIER_WL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

class QvkMagnifier_wl : public QWidget
{
    Q_OBJECT

public:
    QvkMagnifier_wl(QWidget *parent = nullptr);
    ~QvkMagnifier_wl();

private:
    int m_x;
    int m_y;
    QImage currentPicture;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


};
#endif
