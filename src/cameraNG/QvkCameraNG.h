#ifndef QVKCAMERANG_H
#define QVKCAMERANG_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class QvkCameraNG;
}

class QvkCameraNG : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraNG(QWidget *parent = nullptr);
    ~QvkCameraNG();
    int get_camera_window_x();
    int get_camera_window_y();

private:
    Ui::QvkCameraNG *ui;
    QCheckBox *checkBox_frame_OnOff = nullptr;
    QWidget *m_parent = nullptr;

};

#endif // QVKCAMERANG_H
