/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
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

#ifndef REGIONCHOISE_WL_H
#define REGIONCHOISE_WL_H

#include "ui_formMainWindow_wl.h"

#include <QWidget>
#include <QScreen>
#include <QCloseEvent>

class QvkRegionChoise_wl : public QWidget
{ 
Q_OBJECT
public:
  QvkRegionChoise_wl( Ui_formMainWindow_wl *ui );
  virtual ~QvkRegionChoise_wl();
  int getX();
  int getY();
  qreal getHeight();
  qreal getWidth();
  qreal getXRecordArea();
  qreal getYRecordArea();

  void recordMode(bool value);
  void setX( int x );
  void setY( int y );
  void setWidth( int width );
  void setHeight( int height );

  void setFrameColor( QColor color );

  QScreen *screen;
  int screenWidth = 400; //Must set a value. Under windows comes a QPainter message
  int screenHeight = 400;


public slots:
  void slot_areaReset();
  void slot_init();


private slots:

    
protected:
  void paintEvent( QPaintEvent *event );
  void mouseMoveEvent( QMouseEvent *event );
  void mousePressEvent( QMouseEvent *event );
  void mouseReleaseEvent( QMouseEvent * event );
  void leaveEvent( QEvent *event );
  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );
  void closeEvent( QCloseEvent *event );
  

private:
  Ui_formMainWindow_wl *ui;
  QPixmap pixmap;

  enum Handle { NoHandle, TopLeft, TopMiddle, TopRight, RightMiddle, BottomRight, BottomMiddle, BottomLeft, LeftMiddle, Middle };
  Handle handlePressed;
  Handle handleUnderMouse;
  Handle handleKeyPressed;
  Handle HandleSelected;

  QColor HandleColorBackground;
  QColor HandleColorBackgroundSize;
  QColor HandleColorByMousePressed;
  QColor colorSelectedArrow;
  void setHandleColorBackground( QColor color );
  void setHandleColorByMousePressed( QColor color );
  void setHandleColorBackgroundSize( QColor color );

  int mous_delta_X_to_blueline;
  int mous_delta_Y_to_blueline;

  int old_Mouse_X;
  int old_Mouse_Y;
  int old_Frame_Width;
  int old_Frame_Height;
  int old_Frame_X2;
  int old_Frame_Y2;

  int framePenWidth;
  int framePenHalf;
  int radius;
  int diameter;
  int penWidth;
  int penHalf;

  int frame_X;
  int frame_Y;
  int frame_Width;
  int frame_height;
  int frame_min_width;
  int frame_min_height;

  int pixelWidth;
  int pixelHeight;

  void drawFrame( QPainter &painter );
  void printSize(QPainter &painter);
  void HandleRecord( QPainter &painter, int x, int y, int startAngle, int spanAngle );
  void HandleTopLeft(QPainter &painter );
  void HandleTopLeftSize(QPainter &painter);
  void HandleTopMiddle(QPainter &painter);
  void HandleTopMiddleSize(QPainter &painter);
  void HandleTopRight( QPainter &painter );
  void HandleTopRightSize(QPainter &painter);
  void HandleRightMiddle(QPainter &painter);
  void HandleRightMiddleSize(QPainter &painter);
  void HandleBottomRight(QPainter &painter);
  void HandleBottomRightSize(QPainter &painter);
  void HandleBottomMiddle(QPainter &painter);
  void HandleBottomMiddleSize( QPainter &painter );
  void HandleBottomLeft(QPainter &painter);
  void HandleBottomLeftSize( QPainter &painter );
  void HandleLeftMiddle(QPainter &painter);
  void HandleLeftMiddleSize( QPainter &painter );
  void HandleMiddle(QPainter &painter);

  void vk_setGeometry( int x, int y, int with, int height );

  bool recordemode = false;

  QColor getFrameColor();
  QColor frameColor;

};
#endif
