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

#ifndef REGIONBUTTONARROW_H
#define REGIONBUTTONARROW_H

#include <QPainter>

class QvkRegionButtonArrow : public QObject
{ 
Q_OBJECT
public:
  QvkRegionButtonArrow();
  virtual ~QvkRegionButtonArrow();
  enum degreeArrow { topMiddle=0, topRight=45, rightMiddle=90, bottomRight=135,
                     bottomMiddel=180, bottomLeft=225, leftMiddel=270, topLeft=315 };

private:
  int penWidth = 2;
  int penWidthHalf = penWidth/2;
  int radius = 20;
  int diameter = 2 * radius;

public slots:
  QPixmap getPixmapHandle(QColor color, QColor colorSelected, degreeArrow degree );
  QPixmap getButton( QColor color );
  QPixmap getArrow(degreeArrow degree , QColor colorSelected);
  int getWithHalf();


private slots:


protected:

    


};
#endif
