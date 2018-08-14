#include "resizeTopRightWidget.h"

resizeTopRightWidget::resizeTopRightWidget( QWidget *parent ) : QWidget( parent )
{
  set = false;
}

resizeTopRightWidget::~resizeTopRightWidget()
{
}

void resizeTopRightWidget::paintEvent( QPaintEvent *event )
{
  (void)event;
  
    QPainter painter;;
    painter.begin( this );
      painter.setRenderHints( QPainter::Antialiasing, true );
      QRectF rectangle(0.0, 0.0, width(), height() );
      int startAngle = 180 * 16;
      int spanAngle = 90 * 16;
      QBrush brush( widgetColor, Qt::SolidPattern );
      painter.setBrush( brush );
      painter.setPen( widgetColor ); 
      painter.drawPie(rectangle, startAngle, spanAngle);      
    painter.end();
}

void resizeTopRightWidget::setColor( const QColor color)
{
  widgetColor = color;
}