#include "QvkCameraNG.h"
#include "ui_QvkCameraNG.h"

#include <QCheckBox>

QvkCameraNG::QvkCameraNG(QWidget *parent, QCheckBox *checkBox_frame_OnOff) :
    QWidget(parent),
    ui(new Ui::QvkCameraNG)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

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
    qDebug() << "Delete Window";
}


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


