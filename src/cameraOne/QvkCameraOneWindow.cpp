#include "QvkCameraOneWindow.h"
#include "ui_QvkCameraOneWindow.h"

#include <QWidget>
#include <QCheckBox>
#include <QList>

QvkCameraOneWindow::QvkCameraOneWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::QvkCameraOneWindow)
    {
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    m_parent = parent;

    QList<QCheckBox *> listCheckBox = m_parent->topLevelWidget()->findChildren<QCheckBox *>();
    for (int i = 0; i < listCheckBox.count(); i++){
        QCheckBox *checkBox = listCheckBox.at(i);
        if (checkBox->objectName() == "checkBox_Frame_OnOff"){
            checkBox_frame_OnOff = checkBox;
        }
    }

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

QvkCameraOneWindow::~QvkCameraOneWindow()
{
    delete ui;
}


#ifdef Q_OS_UNIX
int QvkCameraOneWindow::get_camera_window_x()
{
    int x = geometry().x();
    return x;
}

int QvkCameraOneWindow::get_camera_window_y()
{
    int y = geometry().y();
    return y;
}
#endif


#ifdef Q_OS_WIN
int QvkCameraOneWindow::get_camera_window_x()
{
    int x = geometry().x();
    return x;
}

int QvkCameraOneWindow::get_camera_window_y()
{
    int y = 0;
    // Rahmen wird entfernt
    if (checkBox_frame_OnOff->isChecked() == true){
        y = geometry().y();
    }
    // Rahmen wird gesetzt
    if (checkBox_frame_OnOff->isChecked() == false){
        int pos_y = m_parent->topLevelWidget()->pos().y();
        int geometry_y = m_parent->topLevelWidget()->geometry().y();
        y = geometry().y() - (geometry_y - pos_y);
    }
    return y;
}
#endif

