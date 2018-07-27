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

  show();
}


QvkRegionRecord::~QvkRegionRecord()
{
}



void QvkRegionRecord::paintEvent(QPaintEvent *event) 
{
}


void QvkRegionRecord::lockFrame( bool status )
{
//    frameLocked = status;
//    handlingFrameLock();
}


bool QvkRegionRecord::isFrameLocked()
{
//    return frameLocked;
}


void QvkRegionRecord::handlingFrameLock()
{
//  repaint();
//  update();
}


/**
 * Return x from record area
 */
int QvkRegionRecord::getXRecordArea()
{
}


/**
 * Return y from record area
 */
int QvkRegionRecord::getYRecordArea()
{
}


/**
 * Return height from record area
 */
int QvkRegionRecord::getHeight()
{
}


/**
 * Return width from record Area
 */
int QvkRegionRecord::getWidth()
{
}


