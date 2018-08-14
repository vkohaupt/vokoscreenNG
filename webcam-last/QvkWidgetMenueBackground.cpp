#include "QvkWidgetMenueBackground.h"

QvkWidgetMenueBackground::QvkWidgetMenueBackground( QWidget *parent ) : QWidget( parent )
{
}


QvkWidgetMenueBackground::~QvkWidgetMenueBackground()
{
}


void QvkWidgetMenueBackground::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);
    QPainter painter;
    painter.begin( this );
       painter.setRenderHints( QPainter::Antialiasing, true );
       QBrush brush( Qt::white, Qt::SolidPattern );
       painter.setOpacity( 0.4 );
       painter.setBrush( brush );
       painter.drawRect( 0,
                         0,
                         width(),
                         height() );
    painter.end();
}
