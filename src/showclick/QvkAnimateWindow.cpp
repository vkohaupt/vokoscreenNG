#include "QvkAnimateWindow.h"
#include <QPainter>
#include <QDebug>

QvkAnimateWindow::QvkAnimateWindow()
{
  setAttribute( Qt::WA_TranslucentBackground, true );
  
  resize( 110, 110 );
  
  QRegion window( 0, 
                  0, 
                  width(),
                  height(),
                  QRegion::Rectangle );

  QRegion mouseHole( width() / 2 - 1,
		     height() / 2 - 1,
		     2,
		     2,
		     QRegion::Rectangle );
  
  QRegion r1 = window.QRegion::subtracted( mouseHole );
  
  this->setMask( r1 );
}

QvkAnimateWindow::~QvkAnimateWindow()
{
}

void QvkAnimateWindow::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  QPainter painter( this );
  painter.setRenderHints( QPainter::Antialiasing, true );

    QBrush brush;
    if ( radiant == false )
    {
      brush.setStyle( Qt::SolidPattern );
      brush.setColor( color );
    }
    else
    {
      QRadialGradient radialGradient( width()/2, height()/2, diameter/2 );
      radialGradient.setColorAt( 0, color );
      radialGradient.setColorAt( 1, Qt::transparent );
      QBrush brushRadial( radialGradient );
      brush.swap( brushRadial );
    }
  
  painter.setPen( Qt::NoPen );
  painter.setBrush( brush );
  painter.setOpacity ( opacity );
  painter.drawEllipse ( QPoint( width()/2, height()/2 ), diameter/2, diameter/2 );
  
}

void QvkAnimateWindow::setRadiusColor( int valueDiameter, QColor valueColor )
{
  diameter = valueDiameter;
  color = valueColor;
  repaint();
}

void QvkAnimateWindow::setOpacity( double value )
{
  opacity = value;
}

void QvkAnimateWindow::setRadiant( bool value )
{
  radiant = value;
}
