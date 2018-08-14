#include "frameWidget.h"

frameWidget::frameWidget( QWidget *parent ) : QWidget( parent )
{
  set = false;
}

frameWidget::~frameWidget()
{
}

void frameWidget::paintEvent( QPaintEvent *event )
{
  (void)event;

  if ( set == false )
  {
    QPainter painter;;
    painter.begin( this );
      painter.setRenderHints( QPainter::Antialiasing, true );
      QBrush brush( widgetColor, Qt::SolidPattern );
      painter.setBrush( brush );
      painter.setPen( widgetColor ); 
      painter.drawRect ( 0, 0, width(), height() );

      QBrush brush_1( Qt::white, Qt::SolidPattern );
      painter.setBrush( brush_1 );
      painter.setPen( Qt::white ); 
      painter.drawRect ( 3, 7, width()-6, height()-10 );
    painter.end();
  }
  
  if ( set == true )
  {
    QPainter painter;
    painter.begin( this );
      painter.setRenderHints( QPainter::Antialiasing, true );
      QBrush brush( widgetColor, Qt::SolidPattern );
      painter.setBrush( brush );
      painter.setPen( widgetColor ); 
      painter.drawRect ( 2, 6, width()-4, height()-8 );

      QBrush brush_1( Qt::white, Qt::SolidPattern );
      painter.setBrush( brush_1 );
      painter.setPen( Qt::white ); 
      painter.drawRect ( 3, 7, width()-6, height()-10 );
    painter.end();
  }
}


void frameWidget::mouseReleaseEvent( QMouseEvent *event )
{
  (void)event;

  if ( set == false )
    set = true;
  else
    set = false;
  
  emit clicked( set );
  //repaint();
}

void frameWidget::setColor( const QColor color)
{
  widgetColor = color;
}

bool frameWidget::isFrameSet()
{
  return set;  
}
