#include "QvkCameraWindow.h"
#include <QPoint>
#include <QDebug>

QvkCameraWindow::QvkCameraWindow()
{
    setWindowFlags( Qt::WindowStaysOnTopHint );
    setStyleSheet( "background-color:black;" );
    setMinimumSize( QSize( 160, 120 ) );
    setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
}


QvkCameraWindow::~QvkCameraWindow()
{
}


void QvkCameraWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_cameraWindow_close( false );
}
