#include "QvkCameraNG.h"
#include "ui_QvkCameraNG.h"

#include <QCheckBox>

QvkCameraNG::QvkCameraNG(QWidget *parent, QCheckBox *m_checkBox_frame_OnOff) :
    QWidget(parent),
    ui(new Ui::QvkCameraNG)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    checkBox_frame_OnOff = m_checkBox_frame_OnOff;

    if (checkBox_frame_OnOff->isChecked() == false){
        Qt::WindowFlags flags;
        flags  = Qt::Window;
        flags |= Qt::WindowStaysOnTopHint;
        setWindowFlags(flags);
    }

    if (checkBox_frame_OnOff->isChecked() == true){
        Qt::WindowFlags flags;
        flags  = Qt::Window;
        flags |= Qt::FramelessWindowHint;
        flags |= Qt::WindowStaysOnTopHint;
        setWindowFlags(flags);
    }
}

QvkCameraNG::~QvkCameraNG()
{
    delete ui;
}

#ifdef Q_OS_UNIX
int QvkCameraNG::get_camera_window_x()
{
    int x = this->geometry().x();
    return x;
}

int QvkCameraNG::get_camera_window_y()
{
    int y = this->geometry().y();
    return y;
}
#endif


#ifdef Q_OS_WIN
int QvkCameraNG::get_camera_window_x()
{
    int x = geometry().x();
    return x;
}

int QvkCameraNG::get_camera_window_y()
{
    int y = 0;
    // Rahmen wird entfernt
    if (checkBox_frame_OnOff->isChecked() == true){
        y = geometry().y();
    }
    // Rahmen wird gesetzt
    if (checkBox_frame_OnOff->isChecked() == false){
        y = geometry().y() - 31;
    }
    return y;
}
#endif

