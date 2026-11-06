#ifndef QVKMAGNIFIER_WL_H
#define QVKMAGNIFIER_WL_H

// include checked
#include <QWidget>
#include <QImage>

class QvkMagnifierPipeWire_wl;
class QMouseEvent;
class QPaintEvent;


class QvkMagnifier_wl : public QWidget
{
    Q_OBJECT

public:
    QvkMagnifier_wl(QWidget *parent = nullptr);
    ~QvkMagnifier_wl();
    QvkMagnifierPipeWire_wl *vkMagnifierPipeWire_wl = nullptr;

private:
    int m_x;
    int m_y;
    QImage currentPicture;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


};
#endif
