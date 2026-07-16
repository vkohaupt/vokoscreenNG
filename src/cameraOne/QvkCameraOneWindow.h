#ifndef QVKCAMERAONEWINDOW_H
#define QVKCAMERAONEWINDOW_H

#include <QString>
#include <QWidget>
#include <QShowEvent>
#include <QCloseEvent>
#include <QToolButton>
#include <QImage>

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


private:
    Ui::QvkCameraOneWindow *ui;
    QToolButton *toolButton_Frame_OnOff = nullptr;
    QWidget *m_parent = nullptr;
    QString m_deviceName;
    QImage image;


protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);


signals:
    void signal_cameraOneWindowClosed(QString);


};

#endif // QVKCAMERAONEWINDOW_H
