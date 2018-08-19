#include "QvkWidgetFrame.h"

#include <QDebug>
#include <QIcon>

frameWidget::frameWidget( QWidget *parent ) : QWidget( parent )
{
    set = false;
    iconMode = false;
}


frameWidget::~frameWidget()
{
}


void frameWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);
    QIcon icon = QIcon::fromTheme( "view-restore", QIcon( ":/pictures/webcam.png" ) );
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


void frameWidget::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event);

    if ( set == false )
      set = true;
    else
      set = false;

    emit clicked( set );
}


void frameWidget::enterEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = true;
}


void frameWidget::leaveEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = false;
}
