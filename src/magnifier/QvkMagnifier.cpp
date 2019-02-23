#include "QvkMagnifier.h"

// Lupe Rund, Qadratisch, Oval

QvkMagnifier::QvkMagnifier()
{

  faktor = 2;
  label = new QLabel( this );

  slot_magnifier400x200();

  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip ); //With tooltip, no entry in Taskbar
  border = 3;

  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
  label->setAlignment( Qt::AlignCenter );
  label->setScaledContents( true );

  timer = new QTimer( this );
  connect( timer, SIGNAL( timeout() ), this, SLOT( slot_mytimer() ) );
}


QvkMagnifier::~QvkMagnifier()
{
}


void QvkMagnifier::slot_magnifierShow( bool value )
{
    if ( value == true )
    {
        show();
        timer->start( 40 );
    }

    if ( value == false )
    {
        close();
        timer->stop();
    }
}


void QvkMagnifier::slot_magnifier200x200()
{
  distanceX = 50;
  distanceY = 50;
  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::slot_magnifier400x200()
{
  distanceX = 100;
  distanceY = 50;
  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


void QvkMagnifier::slot_magnifier600x200()
{
  distanceX = 150;
  distanceY = 50;
  resize( 2 * distanceX * faktor, 2 * distanceY * faktor );
  label->setGeometry( QRect( 0 + border, 0 + border, this->width() - 2 * border, this->height() - 2 * border ) );
}


int QvkMagnifier::NewDistanceXLeft()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();

  int newDistanceX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) *
                 ( distanceX + ( width() / 2 ) ) /
                 ( desk->screenGeometry().width() / 2 - distanceX ) -
                 ( width() / 2 );
  return  newDistanceX;
}


int QvkMagnifier::NewDistanceXRight()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();

  int newDistanX = ( ( desk->screenGeometry().width() / 2 ) - cursor.pos().x() ) *
                 ( -distanceX - ( width() / 2 ) ) /
                 ( desk->screenGeometry().width() / 2 - distanceX ) -
                 ( width() / 2 );
  return  newDistanX;
}


void QvkMagnifier::setMagnifier()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();

  // Lupe an oberen linke Ecke setzen
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
  {
    move( 2 * distanceX,  2 * distanceY );
    return;
  }

  // Lupe obere rechte Ecke setzen
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
  {
    move( desk->screenGeometry().width() - 2 * distanceX - width(), 2 * distanceY);
    return;
  }

  // Lupe am oberen Rand setzen
  // Linke Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() + NewDistanceXLeft(), 2 * distanceY );
    return;
  }
  // Rechte Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) )
  {
    move( cursor.pos().x() - NewDistanceXRight() - width(), 2 * distanceY );
    return;
  }

  // Lupe an untere rechte Ecke setzen
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
      move( desk->screenGeometry().width() - ( 2 * distanceX + width() ), desk->screenGeometry().height() - ( 2 * distanceY + height() ) );
      return;
  }

  // Lupe am rechten Rand setzen
  // Obere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )// div 2
  {
    move( desk->screenGeometry().width() - ( 2 * distanceX + width() ), cursor.pos().y() + 1 * distanceY );
    return;
  }
  // untere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( desk->screenGeometry().width() - ( 2 * distanceX + width() ), cursor.pos().y() - distanceY - height() );
    return;
  }

  // Lupe an linken unteren Ecke setzen
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    move( 2 * distanceX, desk->screenGeometry().height() - 2 * distanceY - height() );
    return;
  }

  // Lupe am unteren Rand setzen
  // Linke Hälfte unterer Rand
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    move( cursor.pos().x() + NewDistanceXLeft(), desk->screenGeometry().height() - ( 2 * distanceY + height() ) );
    return;
  }
  // Rechte Hälfte unterer Rand
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    move( cursor.pos().x() - NewDistanceXRight() - width(), desk->screenGeometry().height() - 2 * distanceY - height() );
    return;
  }

  // Lupe am linken Rand setzen
  // Obere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2

  {
    move( 2 * distanceX, cursor.pos().y() + distanceY );
    return;
  }
  // Untere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
  {
    move( 2 * distanceX, cursor.pos().y() - distanceY - height() );
    return;
  }

  // Linke obere Hälfte
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) ) // div 2
    move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() + distanceY );

  // Rechte obere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() < desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanceXRight() - width(), cursor.pos().y() + distanceY );

  // Linke untere Hälfte
  if ( ( cursor.pos().x() < desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() - distanceY - height() );

  // Rechte untere Hälfte
  if ( ( cursor.pos().x() > desk->screenGeometry().width() / 2 ) and ( cursor.pos().y() > desk->screenGeometry().height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanceXRight() -width(), cursor.pos().y() - distanceY - width() );
}


void QvkMagnifier::slot_mytimer()
{
  QCursor cursor;
  QDesktopWidget *desk = QApplication::desktop();

  QPixmap originalPixmap = QPixmap();
  QScreen *screen = QGuiApplication::primaryScreen();
  
  setMagnifier();

  // Obere linke Ecke
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  0,
					  0,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Obere rechte Ecke
  if ( ( cursor.pos().x() > ( desk->screenGeometry().width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  desk->screenGeometry().width() - 2 * distanceX,
					  0,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Linke untere Ecke
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  0,
					  desk->screenGeometry().height() - 2 * distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Rechte untere Ecke
  if ( ( cursor.pos().x() > desk->screenGeometry().width() - distanceX ) and ( cursor.pos().y() > desk->screenGeometry().height() - distanceY ) )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  desk->screenGeometry().width() - 2 * distanceX,
					  desk->screenGeometry().height() - 2 * distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Unterer Rand
  if ( cursor.pos().y() > desk->screenGeometry().height() - distanceY )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  cursor.pos().x() - distanceX,
					  desk->screenGeometry().height() - 2 * distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Oberen Rand
  if ( cursor.pos().y() < distanceY )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  cursor.pos().x() - distanceX,
					  0,
					  2 * distanceX,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Rechter Rand
  if ( cursor.pos().x() > desk->screenGeometry().width() - distanceX )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  desk->screenGeometry().width() - 2 * distanceX,
					  cursor.pos().y() - distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Linker Rand
  if ( cursor.pos().x() < distanceX )
  {
    originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					  0,
					  cursor.pos().y() - distanceY,
					  2 * distanceX ,
					  2 * distanceY );
    label->setPixmap( originalPixmap );
    return;
  }

  // Fläche
  originalPixmap = screen->grabWindow( QApplication::desktop()->winId(),
					cursor.pos().x() - distanceX,
					cursor.pos().y() - distanceY,
					2 * distanceX ,
					2 * distanceY );
  label->setPixmap( originalPixmap );
}
