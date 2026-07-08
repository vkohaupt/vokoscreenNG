#ifndef QVKCAMERAONEWINDOW_H
#define QVKCAMERAONEWINDOW_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class QvkCameraOneWindow;
}

class QvkCameraOneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneWindow(QWidget *parent = nullptr);
    ~QvkCameraOneWindow();
    int get_camera_window_x();
    int get_camera_window_y();


private:
    Ui::QvkCameraOneWindow *ui;
    QCheckBox *checkBox_frame_OnOff = nullptr;
    QWidget *m_parent = nullptr;

};

#endif // QVKCAMERAONEWINDOW_H
