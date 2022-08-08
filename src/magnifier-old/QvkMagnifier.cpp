/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkMagnifier.h"

#include <QApplication>
#include <QDebug>

QvkMagnifier::QvkMagnifier()
{

  QList<QScreen *> screen = QGuiApplication::screens();
  rectVirtualScreen = screen.at(0)->availableVirtualGeometry();

  faktor = 2;
  label = new QLabel( this );

  slot_magnifier200x200();

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
  int newDistanceX = ( ( rectVirtualScreen.width() / 2 ) - cursor.pos().x() ) *
                 ( distanceX + ( width() / 2 ) ) /
                 ( rectVirtualScreen.width() / 2 - distanceX ) -
                 ( width() / 2 );
  return  newDistanceX;
}


int QvkMagnifier::NewDistanceXRight()
{
  QCursor cursor;
  int newDistanX = ( ( rectVirtualScreen.width() / 2 ) - cursor.pos().x() ) *
                 ( -distanceX - ( width() / 2 ) ) /
                 ( rectVirtualScreen.width() / 2 - distanceX ) -
                 ( width() / 2 );
  return  newDistanX;
}


void QvkMagnifier::setMagnifier()
{
  QCursor cursor;
  // Lupe an oberen linke Ecke setzen
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
  {
    move( 2 * distanceX,  2 * distanceY );
    return;
  }

  // Lupe obere rechte Ecke setzen
  if ( ( cursor.pos().x() > ( rectVirtualScreen.width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
  {
    move( rectVirtualScreen.width() - 2 * distanceX - width(), 2 * distanceY);
    return;
  }

  // Lupe am oberen Rand setzen
  // Linke Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) )
  {
    move( cursor.pos().x() + NewDistanceXLeft(), 2 * distanceY );
    return;
  }
  // Rechte Hälfte am oberen Rand
  if ( ( cursor.pos().y() < distanceY ) and ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) )
  {
    move( cursor.pos().x() - NewDistanceXRight() - width(), 2 * distanceY );
    return;
  }

  // Lupe an untere rechte Ecke setzen
  if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
  {
      move( rectVirtualScreen.width() - ( 2 * distanceX + width() ), rectVirtualScreen.height() - ( 2 * distanceY + height() ) );
      return;
  }

  // Lupe am rechten Rand setzen
  // Obere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) )// div 2
  {
    move( rectVirtualScreen.width() - ( 2 * distanceX + width() ), cursor.pos().y() + 1 * distanceY );
    return;
  }
  // untere Hälfte am rechten Rand
  if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
  {
    move( rectVirtualScreen.width() - ( 2 * distanceX + width() ), cursor.pos().y() - distanceY - height() );
    return;
  }

  // Lupe an linken unteren Ecke setzen
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
  {
    move( 2 * distanceX, rectVirtualScreen.height() - 2 * distanceY - height() );
    return;
  }

  // Lupe am unteren Rand setzen
  // Linke Hälfte unterer Rand
  if ( ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
  {
    move( cursor.pos().x() + NewDistanceXLeft(), rectVirtualScreen.height() - ( 2 * distanceY + height() ) );
    return;
  }
  // Rechte Hälfte unterer Rand
  if ( ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
  {
    move( cursor.pos().x() - NewDistanceXRight() - width(), rectVirtualScreen.height() - 2 * distanceY - height() );
    return;
  }

  // Lupe am linken Rand setzen
  // Obere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) ) // div 2

  {
    move( 2 * distanceX, cursor.pos().y() + distanceY );
    return;
  }
  // Untere Hälfte am linken Rand
  if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
  {
    move( 2 * distanceX, cursor.pos().y() - distanceY - height() );
    return;
  }

  // Linke obere Hälfte
  if ( ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) )
    move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() + distanceY );

  // Rechte obere Hälfte
  if ( ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() < rectVirtualScreen.height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanceXRight() - width(), cursor.pos().y() + distanceY );

  // Linke untere Hälfte
  if ( ( cursor.pos().x() < rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
    move( cursor.pos().x() + NewDistanceXLeft(), cursor.pos().y() - distanceY - height() );

  // Rechte untere Hälfte
  if ( ( cursor.pos().x() > rectVirtualScreen.width() / 2 ) and ( cursor.pos().y() > rectVirtualScreen.height() / 10 * 8 ) )
    move( cursor.pos().x() - NewDistanceXRight() -width(), cursor.pos().y() - distanceY - height() );
}


void QvkMagnifier::slot_mytimer()
{
    QCursor cursor;
    QPixmap pixmap = QPixmap();
    QList<QScreen *> screen = QGuiApplication::screens();

    setMagnifier();

    // Obere linke Ecke
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() <  distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           0,
                                           0,
                                           2 * distanceX,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Obere rechte Ecke
    if ( ( cursor.pos().x() > ( rectVirtualScreen.width() - distanceX ) ) and ( cursor.pos().y() < distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           rectVirtualScreen.width() - 2 * distanceX,
                                           0,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Linke untere Ecke
    if ( ( cursor.pos().x() < distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           0,
                                           rectVirtualScreen.height() - 2 * distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Rechte untere Ecke
    if ( ( cursor.pos().x() > rectVirtualScreen.width() - distanceX ) and ( cursor.pos().y() > rectVirtualScreen.height() - distanceY ) )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           rectVirtualScreen.width() - 2 * distanceX,
                                           rectVirtualScreen.height() - 2 * distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Unterer Rand
    if ( cursor.pos().y() > rectVirtualScreen.height() - distanceY )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           cursor.pos().x() - distanceX,
                                           rectVirtualScreen.height() - 2 * distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Oberen Rand
    if ( cursor.pos().y() < distanceY )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           cursor.pos().x() - distanceX,
                                           0,
                                           2 * distanceX,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Rechter Rand
    if ( cursor.pos().x() > rectVirtualScreen.width() - distanceX )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           rectVirtualScreen.width() - 2 * distanceX,
                                           cursor.pos().y() - distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Linker Rand
    if ( cursor.pos().x() < distanceX )
    {
        pixmap = screen.at(0)->grabWindow( 0,
                                           0,
                                           cursor.pos().y() - distanceY,
                                           2 * distanceX ,
                                           2 * distanceY );
        label->setPixmap( pixmap );
        return;
    }

    // Fläche
    pixmap = screen.at(0)->grabWindow( 0,
                                       cursor.pos().x() - distanceX,
                                       cursor.pos().y() - distanceY,
                                       2 * distanceX ,
                                       2 * distanceY );
    label->setPixmap( pixmap );
}
