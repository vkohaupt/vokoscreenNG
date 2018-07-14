#include "QvkWidgetHelp.h"

#include <QIcon>
#include <QDesktopServices>
#include <QDebug>

QvkWidgetHelp::QvkWidgetHelp( QWidget *parent ) : QWidget( parent )
{
    iconMode = false;
}


QvkWidgetHelp::~QvkWidgetHelp()
{
}


void QvkWidgetHelp::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);
    QIcon icon = QIcon::fromTheme( "dialog-information", QIcon( ":/pictures/webcam.png" ) );
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


void QvkWidgetHelp::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event);
    QDesktopServices::openUrl( QUrl( "http://linuxecke.volkoh.de/vokoscreen/help/3.0/audio.html", QUrl::TolerantMode ) );
}


void QvkWidgetHelp::enterEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = true;
}


void QvkWidgetHelp::leaveEvent( QEvent *event )
{
    Q_UNUSED(event);
    iconMode = false;
}
