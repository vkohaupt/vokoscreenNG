#include "QvkWinInfo.h"

#include <QCoreApplication>
#include <QX11Info>
#include <X11/Xutil.h>

using namespace std;

QvkWinInfo::QvkWinInfo()
{
}


void QvkWinInfo::slot_start()
{
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

  resize ( 50, 50 );
  
  QRegion Fenster( 0, 
                   0, 
                   50,
                   50,
                   QRegion::Rectangle );

  QRegion mouseHole( 21,
		     21,
		     8,
		     8,
		     QRegion::Rectangle );

  QRegion Rechteck_LO( 0,
		       0,
		       23,
		       23,
                       QRegion::Rectangle );
		       
  QRegion Rechteck_RO( 27,
		       0,
		       23,
		       23,
                       QRegion::Rectangle );
		       
  QRegion Rechteck_LU( 0,
		       27,
		       23,
		       23,
                       QRegion::Rectangle );
		       
  QRegion Rechteck_RU( 27,
		       27,
		       23,
		       23,
                       QRegion::Rectangle );

  QRegion r1 = Fenster.QRegion::subtracted( mouseHole );
  QRegion r2 = r1.QRegion::subtracted( Rechteck_LO );
  QRegion r3 = r2.QRegion::subtracted( Rechteck_RO );
  QRegion r4 = r3.QRegion::subtracted( Rechteck_LU );
  QRegion r5 = r4.QRegion::subtracted( Rechteck_RU );

  this->setMask( r5 );

  lastWinID = this->winId();
  
  windowTimer = new QTimer( this );
  connect( windowTimer, SIGNAL( timeout() ), this, SLOT( selectWindow() ) );
  windowTimer->start( 500 );
  
  mouseTimer = new QTimer( this );
  connect( mouseTimer, SIGNAL( timeout() ), this, SLOT( mousePosition() ) );
  mouseTimer->start( 20 );

  show();
  emit signal_showCursor( true );
}


QvkWinInfo::~QvkWinInfo()
{  
}


void QvkWinInfo::paintEvent( QPaintEvent *event ) 
{
  (void)event;
  QPainter painter( this );
  painter.setPen( QPen( Qt::blue, 4 ) );
  painter.drawLine( 50/2, 0, 50/2, 50);
  painter.drawLine( 0, 50/2, 50, 50/2);
}


void QvkWinInfo::mousePosition()
{
  QCursor cursor;
  move( cursor.pos().x() - 25 , cursor.pos().y() - 25 );
}


WId QvkWinInfo::activeWindow()
{
  Window id;
  int dummy;
  XGetInputFocus( QX11Info::display() , &id, &dummy);
  return id;
}


void QvkWinInfo::selectWindow()
{
  newWinID = activeWindow();

  if ( lastWinID != newWinID )
  {
    windowTimer->stop();
    mouseTimer->stop();

    // Cursor resize does not show in video in the first Frames
    resize( 10, 10 );
    
    emit windowChanged( true );
    this->close();
  }
}


// Need for Screenshot
WId QvkWinInfo::getWinID()
{
  return newWinID;
}

