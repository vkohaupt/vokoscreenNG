#include "QvkCameraWindow.h"
#include <QPoint>
#include <QDebug>
#include <QMouseEvent>

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


void QvkCameraWindow::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
        }
    }
}

void QvkCameraWindow::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape )
    {
        showNormal();
    }

    if ( event->key() == Qt::Key_F11 )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
        }
    }
}
