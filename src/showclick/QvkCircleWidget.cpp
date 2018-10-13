#include "QvkCircleWidget.h"

#include <QRadialGradient>

QvkCircleWidget::QvkCircleWidget( QWidget *parent ) : QWidget( parent )
{
  parentWidget = parent;
}

QvkCircleWidget::~QvkCircleWidget()
{
}

void QvkCircleWidget::paintEvent( QPaintEvent *event )
{
  (void)event;

  QPainter painter;;
  painter.begin( this );
    painter.setRenderHints( QPainter::Antialiasing, true );

    QBrush brush;
    if ( radiant == false )
    {
      brush.setStyle( Qt::SolidPattern );
      brush.setColor( pointerColor );
    }
    else
    {
      QRadialGradient radialGradient( parentWidget->width()/2, parentWidget->height()/2, getDiameter()/2 );
      radialGradient.setColorAt( 0, getColor() );
      radialGradient.setColorAt( 1, Qt::transparent );
      QBrush brushRadial( radialGradient );
      brush.swap( brushRadial );
    }
    
    painter.setBrush( brush );
    painter.setPen( Qt::NoPen );
    painter.setOpacity( pointerOpacity );
    resize( parentWidget->width(), parentWidget->height());
    painter.drawEllipse( parentWidget->width()/2-diameter/2, parentWidget->height()/2-diameter/2, diameter, diameter );
  painter.end();
}

void QvkCircleWidget::setColor( QColor value )
{
  pointerColor = value;
  repaint();
}

QColor QvkCircleWidget::getColor()
{
  return pointerColor;
}

void QvkCircleWidget::setDiameter( int value )
{
  diameter = value;
  repaint();
}

int QvkCircleWidget::getDiameter()
{
  return diameter;
}

void QvkCircleWidget::setOpacity( double value )
{
  pointerOpacity = value;;
  repaint();
}

double QvkCircleWidget::getOpacity()
{
  return pointerOpacity;
}

void QvkCircleWidget::setRadiant( bool value )
{
  radiant = value;
  repaint();
}

bool QvkCircleWidget::getRadiant()
{
  return radiant;
}


