#ifndef QVKCAMERAONEWINDOW_H
#define QVKCAMERAONEWINDOW_H

// Qt include checked
#include <QWidget>
#include <QToolButton>

namespace Ui {
class QvkCameraOneWindow;
}

class QvkCameraOneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneWindow(QWidget *parent = nullptr, QString deviceName = "");
    ~QvkCameraOneWindow();
    int get_camera_window_x();
    int get_camera_window_y();
    void set_newImage(QImage m_image);
    QImage image;


private:
    Ui::QvkCameraOneWindow *ui;
    QToolButton *toolButton_Frame_OnOff = nullptr;
    QWidget *m_parent = nullptr;
    QString m_deviceName;
    bool mousePressed = false;
    int mouseLocal_X;
    int mouseLocal_Y;


protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


signals:
    void signal_cameraOneWindowClosed(QString);


};

#endif // QVKCAMERAONEWINDOW_H
