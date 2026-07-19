#include "QvkCameraOneWindow.h"
#include "ui_QvkCameraOneWindow.h"

#include <QString>
#include <QWidget>
#include <QCheckBox>
#include <QList>
#include <QLineEdit>
#include <QPainter>
#include <QToolButton>
#include <QCheckBox>
#include <QPoint>
#include <QImage>
#include <QPixmap>
#include <QCursor>

#include <QShowEvent>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QMouseEvent>


QvkCameraOneWindow::QvkCameraOneWindow(QWidget *parent, QString deviceName) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    m_parent = parent;
    m_deviceName = deviceName; // z.b /dev/video0
    setObjectName(objectName() + "_" + deviceName);

    toolButton_Frame_OnOff = topLevelWidget()->findChild<QToolButton *>("toolButtonCameraOneFramelessOnOff_" + deviceName);
    if (toolButton_Frame_OnOff->isChecked() == false){
        Qt::WindowFlags flags;
        flags  = Qt::Window;
        flags |= Qt::WindowStaysOnTopHint;
        setWindowFlags(flags);
    }

    if (toolButton_Frame_OnOff->isChecked() == true){
        // Mit WA_TranslucentBackground sehr gute aussehende Ränder bei Circle und setMask
        setAttribute(Qt::WA_TranslucentBackground, true);
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
    if (toolButton_Frame_OnOff->isChecked() == true){
        y = geometry().y();
    }
    // Rahmen wird gesetzt
    if (toolButton_Frame_OnOff->isChecked() == false){
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
    QLineEdit *lineEdit = m_parent->topLevelWidget()->findChild<QLineEdit *>("lineEditCameraOneTitel_" + m_deviceName);
    setWindowTitle(lineEdit->text());
}


void QvkCameraOneWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    QCheckBox *checkBox_Camera_OnOff = m_parent->topLevelWidget()->findChild<QCheckBox *>("checkBoxCameraOneOnOff_" + m_deviceName);
    checkBox_Camera_OnOff->click();
}


void QvkCameraOneWindow::set_newImage(QImage m_image)
{
    image = m_image;
    repaint();
}


void QvkCameraOneWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    //image = image.convertedTo(QImage::Format_Grayscale8);
    QImage m_image = image;

    // Bild muß hier und in QvkCameraOneSingle verworfen werden wenn invalid
    if (m_image.format() == QImage::Format_Invalid){
        return;
    }

    QPixmap pixmap(m_image.width(), m_image.height());
    pixmap.fill(Qt::transparent);

    QPainter painterPixmap;
    painterPixmap.begin(&pixmap);
    painterPixmap.setRenderHint(QPainter::Antialiasing, true);
    painterPixmap.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painterPixmap.drawImage((width() - m_image.width() ) / 2 , 0, m_image);
    painterPixmap.end();

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawImage((width() - m_image.width() ) / 2 , 0, m_image);
    painter.end();

    QToolButton *toolButtonFrameless = m_parent->topLevelWidget()->findChild<QToolButton *>("toolButtonCameraOneFramelessOnOff_" + m_deviceName);
    if (toolButtonFrameless != nullptr){
        // With frame
        if (toolButtonFrameless->isChecked() == false){
            QToolButton *toolButtonCircle = m_parent->topLevelWidget()->findChild<QToolButton *>("toolButtonCameraOneViewCircle_" + m_deviceName);
            if (toolButtonCircle->isChecked() == true){
                setFixedSize(m_image.height(), m_image.height());
            }else{
                setFixedSize(m_image.width(), m_image.height());
            }
        }
        // Frameless
        if (toolButtonFrameless->isChecked() == true){
            QToolButton *toolButtonCircle = m_parent->topLevelWidget()->findChild<QToolButton *>("toolButtonCameraOneViewCircle_" + m_deviceName);
            if (toolButtonCircle->isChecked() == true){
               setMask(pixmap.mask());
               setFixedSize(m_image.height(), m_image.height());
            }else{
                setMask(pixmap.mask());
                setFixedSize(m_image.width(), m_image.height());
            }
        }
    }
}


void QvkCameraOneWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        //emit signal_mousePressEvent( event );
    }

    QString name = "toolButtonCameraOneFramelessOnOff_" + m_deviceName;
    QToolButton *toolButton = m_parent->topLevelWidget()->findChild<QToolButton *>(name);
    if ((toolButton->isChecked() == true) and (event->button() == Qt::LeftButton)){
        QPixmap pixmap(":/pictures/cursor/size_all.png");
        QCursor cursor(pixmap);
        setCursor(cursor);
        mousePressed = true;
        mouseLocal_X = event->position().x();
        mouseLocal_Y = event->position().y();
    }
}


void QvkCameraOneWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QString name = "toolButtonCameraOneFramelessOnOff_" + m_deviceName;
    QToolButton *toolButton = m_parent->topLevelWidget()->findChild<QToolButton *>(name);
    if ((toolButton->isChecked() == true)){
        unsetCursor();
        mousePressed = false;
    }
}


void QvkCameraOneWindow::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QString name = "toolButtonCameraOneFramelessOnOff_" + m_deviceName;
    QToolButton *toolButton = m_parent->topLevelWidget()->findChild<QToolButton *>(name);
    if ((toolButton->isChecked() == true) and (mousePressed == true)){
        move(QCursor::pos().x() - mouseLocal_X, QCursor::pos().y() - mouseLocal_Y);
    }
}
