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

#ifndef QVKMAGNIFIER_H
#define QVKMAGNIFIER_H

#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QTimer>
#include <QWidget>

class QvkMagnifier: public QWidget
{ 
Q_OBJECT
public:
  QvkMagnifier();
  virtual ~QvkMagnifier();
  QPixmap pixmap;
  bool isToolButtonRectangle = false;
  bool isToolButtonElipse = false;
  bool isToolButtonCircle = false;


public slots:
  void slot_magnifierShow( bool value );
  void slot_magnifier200x200();
  void slot_magnifier400x200();
  void slot_magnifier600x200();


private slots:
  void slot_mytimer();
  

protected:  
  void paintEvent( QPaintEvent *event );

  
signals:
  void signal_magnifier_on_screen( QScreen *);
  

private:
  void setMagnifier();
  int border;
  int distanceX;
  int distanceY;
  int factor;
  QTimer *timer;
  QPoint globalCursorPos;
  QScreen *screen;
  QPoint screenCursorPos;
  QList<QScreen *> screenList;
  int screenIndex;
  int distanceCopyMagnifier;

  QRegion regionTopLeft;
  QRegion regionTopMiddle;
  QRegion regionTopRight;
  QRegion regionRightMiddle;
  QRegion regionBottomRight;
  QRegion regionBottomMiddle;
  QRegion regionBottomLeft;
  QRegion regionLeftMiddle;
  QRegion regionMiddle;
  QRegion valueRegion;

  enum region{ none, topLeft, topMiddle, topRight, rightMiddle, bottomRight, bottomMiddle, bottomLeft, leftMiddle, middle };
  region nameRegion;

  QString enumToString( region reg );


};


#endif
