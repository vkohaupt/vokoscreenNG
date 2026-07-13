#ifndef QVKCAMERAONEWINDOW_H
#define QVKCAMERAONEWINDOW_H

#include <QWidget>
//#include <QCheckBox>
#include <QShowEvent>
#include <QCloseEvent>
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


private:
    Ui::QvkCameraOneWindow *ui;
//    QCheckBox *checkBox_Frame_OnOff = nullptr;
    QToolButton *toolButton_Frame_OnOff = nullptr;
    QWidget *m_parent = nullptr;
    QString m_deviceName;


protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);


signals:
    void signal_cameraOneWindowClosed(QString);


};

#endif // QVKCAMERAONEWINDOW_H
