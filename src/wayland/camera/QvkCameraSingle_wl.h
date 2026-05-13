#ifndef QVKCAMERASINGLE_WL_H
#define QVKCAMERASINGLE_WL_H

#include <QWidget>

namespace Ui {
class QvkCameraSingle_wl;
}

class QvkCameraSingle_wl : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraSingle_wl(QWidget *parent = nullptr);
    ~QvkCameraSingle_wl();
    Ui::QvkCameraSingle_wl *ui;

private:
};

#endif // QVKCAMERASINGLE_WL_H
