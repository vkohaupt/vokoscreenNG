/* vokoscreen - A desktop recorder
 * Copyright (C) 2011-2014 Volker Kohaupt
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301 USA 
 */

#include "QvkRegionRecord.h" 

#include <QBitmap>

QvkRegionRecord::QvkRegionRecord()
{
  
  setAttribute ( Qt::WA_AlwaysShowToolTips );
  
  handlePressed = NoHandle;
  handleUnderMouse = NoHandle;
  painter =  new QPainter();
  
  setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );
  
  setAttribute( Qt::WA_TranslucentBackground, true );

  setMouseTracking( true );
  
  borderLeft = 20;
  borderTop = 20;
  borderRight = 20;
  borderBottom = 20;
 
  // Von außen bis Mitte blauer Rahmen
  // Breite blauer Rahmen
  frameWidth = 4;
  
  radius = 20;

  penWidth = 2;
  penHalf = penWidth / 2;

  show();
}


QvkRegionRecord::~QvkRegionRecord()
{
}


void QvkRegionRecord::setRecordGeometry( int x, int y, int width, int height )
{
  setGeometry( x, y, width + borderLeft + borderRight + frameWidth, height + borderTop + borderBottom + frameWidth );
}


void QvkRegionRecord::HandleTopLeft()
{
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 0 * 16;
  int spanAngle = 270 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleTopMiddle()
{
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 180 * 16;
  int spanAngle = -180 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleTopRight()
{
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, borderTop - radius + penHalf, 2 * radius, 2 * radius );
  int startAngle = 180 * 16;
  int spanAngle = -270 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleRightMiddle()
{
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius,  2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle = -180 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle ); 
}


void QvkRegionRecord::HandleBottomRight()
{
  QRectF rectangle = QRectF( width() - borderRight - radius - penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle = -270 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleBottomMiddle()
{
  QRectF rectangle = QRectF( ( width() - borderLeft - borderRight ) / 2 + borderLeft - radius, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = -180 * 16;
  int spanAngle = 180 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleBottomLeft()
{
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, height() - borderBottom - radius - penHalf, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle = 270 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::HandleLeftMiddle()
{
  QRectF rectangle = QRectF( borderLeft - radius + penHalf, ( height() - borderTop - borderBottom ) / 2 + borderTop - radius, 2 * radius, 2 * radius );
  int startAngle = 90 * 16;
  int spanAngle =  180 * 16;
  painter->drawPie( rectangle, startAngle, spanAngle );
}


void QvkRegionRecord::paintEvent(QPaintEvent *event) 
{
  QPixmap pixmap( width(), height() );
  pixmap.fill( Qt::transparent );
   
  painter->begin( &pixmap );
    painter->setRenderHints( QPainter::Antialiasing, true );
    painter->setBrush( QBrush( Qt::red, Qt::SolidPattern ) );
    painter->setPen( QPen( Qt::black, penWidth ) );
    HandleTopLeft();
    HandleTopMiddle();
    HandleTopRight();
    HandleRightMiddle();
    HandleBottomRight();
    HandleBottomMiddle();
    HandleBottomLeft();
    HandleLeftMiddle();
  
    // Blue Frame
    painter->setPen( QPen( Qt::blue, frameWidth ) );
    painter->setBrush( QBrush( Qt::transparent, Qt::SolidPattern ) );
    painter->drawRect( borderLeft,
                       borderTop, 
                       width() - borderRight- borderRight,
                       height() - borderTop - borderBottom );
  painter->end();

  painter->begin( this);
    painter->drawPixmap( QPoint( 0, 0 ), pixmap );
  painter->end();
  
  setMask( pixmap.mask() ); //Extracts a bitmap mask from the pixmaps alpha channel.

  event->accept();
}


void QvkRegionRecord::lockFrame( bool status )
{
    frameLocked = status;
    handlingFrameLock();
}


bool QvkRegionRecord::isFrameLocked()
{
    return frameLocked;
}


void QvkRegionRecord::handlingFrameLock()
{
  repaint();
  update();
}


/**
 * Return x from window
 */
int QvkRegionRecord::getX()
{
  return geometry().x(); 
}


/**
 * Return y from window
 */
int QvkRegionRecord::getY()
{
  return  geometry().y();
}


/**
 * Return x from record area
 */
int QvkRegionRecord::getXRecordArea()
{
  return geometry().x() + borderLeft + frameWidth / 2; 
}


/**
 * Return y from record area
 */
int QvkRegionRecord::getYRecordArea()
{
  return  geometry().y() + borderTop + frameWidth / 2;
}


/**
 * Return height from record area
 */
int QvkRegionRecord::getHeight()
{
  return height() - borderTop - borderBottom - frameWidth; 
}


/**
 * Return width from record Area
 */
int QvkRegionRecord::getWidth()
{
  return width() - borderLeft - borderRight - frameWidth; 
}


