#include "QvkCameraOneWindow.h"
#include "ui_QvkCameraOneWindow.h"

#include <QWidget>
#include <QCheckBox>
#include <QList>
#include <QLineEdit>
#include <QShowEvent>

QvkCameraOneWindow::QvkCameraOneWindow(QWidget *parent, QString deviceName) :
        QWidget(parent),
        ui(new Ui::QvkCameraOneWindow)
    {
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    m_parent = parent;
    m_deviceName = deviceName;
    setObjectName(objectName() + "_" + deviceName);
    checkBox_Frame_OnOff = m_parent->topLevelWidget()->findChild<QCheckBox *>("checkBox_Frame_OnOff_" + deviceName);

    if (checkBox_Frame_OnOff->isChecked() == false){
        Qt::WindowFlags flags;
        flags  = Qt::Window;
        flags |= Qt::WindowStaysOnTopHint;
        setWindowFlags(flags);
    }

    if (checkBox_Frame_OnOff->isChecked() == true){
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
    if (checkBox_Frame_OnOff->isChecked() == true){
        y = geometry().y();
    }
    // Rahmen wird gesetzt
    if (checkBox_Frame_OnOff->isChecked() == false){
        int pos_y = m_parent->topLevelWidget()->pos().y();
        int geometry_y = m_parent->topLevelWidget()->geometry().y();
        y = geometry().y() - (geometry_y - pos_y);
    }
    return y;
}
#endif


void QvkCameraOneWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    QLineEdit *lineEdit = m_parent->topLevelWidget()->findChild<QLineEdit *>("lineEditCameraTitel_" + m_deviceName);
    setWindowTitle(lineEdit->text());
}
