#include "QvkWidgetExit.h"

#include <QIcon>

QvkWidgetExit::QvkWidgetExit( QWidget *parent ) : QWidget( parent )
{
    iconMode = false;
}


QvkWidgetExit::~QvkWidgetExit()
{
}


void QvkWidgetExit::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);
    QIcon icon = QIcon::fromTheme( "window-close", QIcon( ":/pictures/webcam.png" ) );
    QPixmap iconPixmap = icon.pixmap( QSize( width(), height() ) );

    QPainter painter;
    painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );
    if ( iconMode == false )
    {
        painter.drawPixmap( 0, 0, width(), height(), iconPixmap);
    }
    else
    {
        painter.drawPixmap( 0, 0, width()+5, height()+5, iconPixmap );
    }
    painter.end();
}


void QvkWidgetExit::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    emit clicked();
}


void QvkWidgetExit::enterEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = true;
}


void QvkWidgetExit::leaveEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = false;
}
