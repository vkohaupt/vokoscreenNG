#include "circleWidget.h"

circleWidget::circleWidget( QWidget *parent ) : QWidget( parent )
{
  set = false;
}

circleWidget::~circleWidget()
{
}

void circleWidget::paintEvent( QPaintEvent *event )
{
  (void)event;

  QRegion RegionWidget( 0, 0, width(), height() );
  QRegion RegionPaint( 0, 0, width(), height(), QRegion::Ellipse );
  QRegion RegionSpace = RegionWidget.subtracted( RegionPaint );
  QRegion RegionPaintWidget = RegionWidget.subtracted( RegionSpace );
  setMask(RegionPaintWidget);

  QPainter painter;;
  painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );
    QBrush brush( widgetColor, Qt::SolidPattern );
    painter.setBrush( brush );
    painter.setPen( widgetColor );  
    painter.drawEllipse ( QPoint( width()/2, height()/2), width()/2-1, height()/2-1 );
    
    if ( set == false )
    {
      QBrush brush_1( Qt::white, Qt::SolidPattern );
      painter.setBrush( brush_1 );
      painter.setPen( Qt::white );  
      painter.drawEllipse ( QPoint( width()/2, height()/2), width()/5, height()/5 );
    }
    
    if ( set == true )
    {
      QBrush brush_2( Qt::white, Qt::SolidPattern );
      painter.setBrush( brush_2 );
      painter.setPen( Qt::white );  
      painter.drawEllipse ( QPoint( width()/2, height()/2), width()/5, height()/5 );
      
      QBrush brush_3( Qt::black, Qt::SolidPattern );
      painter.setBrush( brush_3 );
      painter.setPen( Qt::black );  
      painter.drawEllipse ( QPoint( width()/2, height()/2), width()/5-2, height()/5-2 );
    }


  painter.end();
}

/*
 * Wird ausgefühert beim Losslassen der Maustaste
 */
void circleWidget::mouseReleaseEvent ( QMouseEvent * event )
{
  (void)event;

  if ( set == false )
    set = true;
  else
    set = false;
  
  emit clicked( set );
  repaint();
}

void circleWidget::setColor( const QColor color)
{
  widgetColor = color;
}

void circleWidget::unsetPoint()
{
  set = false;
  repaint();
}

void circleWidget::setPoint()
{
  set = true;
  repaint();
}
