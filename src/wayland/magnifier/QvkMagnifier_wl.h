#ifndef QVKMAGNIFIER_WL_H
#define QVKMAGNIFIER_WL_H

// include checked
#include <QWidget>
#include <QImage>
#include <QPen>

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
    QImage currentPicture;

private:
    int m_x;
    int m_y;
    QPen m_bluePen;
    QPen m_grayPen;
    int circle;

public slots:
    void slot_setCurrentPicture(QImage image);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void signal_MagnifierClose();


};
#endif
