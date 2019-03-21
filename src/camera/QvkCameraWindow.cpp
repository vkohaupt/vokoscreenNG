#include "QvkCameraWindow.h"
#include <QPoint>
#include <QDebug>

QvkCameraWindow::QvkCameraWindow()
{
    setWindowFlags( Qt::WindowStaysOnTopHint );
    setStyleSheet( "background-color:black;" );
}


QvkCameraWindow::~QvkCameraWindow()
{
}


void QvkCameraWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_cameraWindow_close( false );
}
