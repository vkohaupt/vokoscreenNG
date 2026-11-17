#ifndef QVKMAGNIFIER_WL_H
#define QVKMAGNIFIER_WL_H

// include checked
#include <QWidget>
#include <QImage>
#include <QPen>

class QvkMagnifier_wl : public QWidget
{
    Q_OBJECT

public:
    QvkMagnifier_wl(QWidget *parent = nullptr);
    ~QvkMagnifier_wl();
    bool mouseUnderMagnifier = false;

private:
    QImage currentPicture;
    int mouse_x;
    int mouse_y;
    QPen circlePen;
    QPen magnifierPen;
    int circle;
    int b_c_m;
    QPixmap pixmap;
    int magnifierWidth;
    int magnifierHeight;
    int sourceWidth;
    int sourceHeight;

public slots:
    void slot_setCurrentPicture(QImage image);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:

};
#endif
