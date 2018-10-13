#include "QvkAnimateControl.h"

#include <QTimer>
#include <QDebug>

QvkAnimateControl::QvkAnimateControl( double time, int diameter, Qt::CheckState radiant, double opacity, QColor color )
{
  showTime = time * 1000;
  
  globalMouse = new QvkGlobalMouse();
  connect( globalMouse, SIGNAL( mousePressed( int, int ) ), this, SLOT( mousePressed( int, int ) ) );
  
  animateWindow = new QvkAnimateWindow();
  setDiameterColor( diameter, color );
  
  if ( radiant == Qt::Unchecked )
    setRadiant( false );
  else
    setRadiant( true );

  setOpacity( opacity );
}

QvkAnimateControl::~QvkAnimateControl()
{
}

void QvkAnimateControl::pointerOnOff( bool value )
{
  if ( value == true )
    animateWindowOn();
  
  if ( value == false )
    animateWindowOff();
}

void QvkAnimateControl::animateWindowOn()
{
  globalMouse->setCursorOn();
  globalMouse->mousePressed();
}

void QvkAnimateControl::animateWindowOff()
{
  globalMouse->setCursorOff();
}

void QvkAnimateControl::mousePressed( int x, int y )
{
  animateWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
  animateWindow->move( x - animateWindow->width() / 2, y - animateWindow->height() / 2 );
  animateWindow->show();
  QTimer::singleShot( showTime, this, SLOT( closeWindow() ) );
}

void QvkAnimateControl::closeWindow()
{
  animateWindow->close();
}

void QvkAnimateControl::setDiameterColor( int diameter, QColor color)
{
  animateWindow->setRadiusColor( diameter, color );
}

void QvkAnimateControl::setShowTime( double value )
{
  showTime = value * 1000; 
}

void QvkAnimateControl::setOpacity( double value )
{
  animateWindow->setOpacity( value );
}

void QvkAnimateControl::setRadiant( bool value )
{
  animateWindow->setRadiant( value );
}
