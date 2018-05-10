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

#ifndef QvkRegionRecord_H 
#define QvkRegionRecord_H

#include <QPainter>
#include <QDebug>
#include <QRegion>
#include <QCursor>
#include <QPainterPath>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>

#include <QLabel>

#include "QvkSettings.h"

class QvkRegionRecord : public QLabel
{ 
Q_OBJECT
public:
  QvkRegionRecord();
  QvkRegionRecord( QWidget *parent );

  virtual ~QvkRegionRecord();
  void lockFrame( bool status = true );
  bool isFrameLocked();

  int borderLeft;
  int borderTop;
  int borderRight;
  int borderBottom;
  int frameWidth;
  
public slots:
  int getX();
  int getY();
  int getHeight();
  int getWidth();
  int getXRecordArea();
  int getYRecordArea();

  void setRecordGeometry( int x, int y, int width, int height );
  
  
private slots:

  void HandleTopLeft();
  void HandleTopMiddle();
  void HandleTopRight();
  void HandleRightMiddle();
  void HandleBottomRight();
  void HandleBottomMiddle();
  void HandleBottomLeft();
  void HandleLeftMiddle();


protected:
  QPainter * painter;
    void paintEvent(QPaintEvent *);

private:
  enum Handle { NoHandle, TopLeft, TopMiddle, TopRight, RightMiddle, BottomRight, BottomMiddle, BottomLeft, LeftMiddle, Middle };

  int radius;
  int penWidth;
  int penHalf;
  bool frameLocked;
  void handlingFrameLock();
  
  int currentMouseLocalX;
  int currentMouseLocalY;
  int currentMouseRightLocalX;
  int currentMouseRightLocalY;
  
  int currentWidgetWidth;
  int currentWidgetHeight;
  int currentWidgetY;
  int currentbottomY;
  
  Handle handlePressed;
  Handle handleUnderMouse;
  
  QRect printSizeRectForMask;
  QRect HandleMiddleForMask;
  
  QvkSettings vkSettings;
  
};
#endif
