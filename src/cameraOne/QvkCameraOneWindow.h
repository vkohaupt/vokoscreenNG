#ifndef QVKCAMERAONEWINDOW_H
#define QVKCAMERAONEWINDOW_H

// Qt include checked
#include <QWidget>
class QToolButton;

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
    int mouseLocal_X = 0;
    int mouseLocal_Y = 0;


protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


signals:
    void signal_cameraOneWindowClosed(QString);


};

#endif // QVKCAMERAONEWINDOW_H
