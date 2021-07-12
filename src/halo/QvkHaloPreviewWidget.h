/* vokoscreenNG
 * Copyright (C) 2017-2021 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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

#ifndef QVKHALOPREVIEWWIDGET_H
#define QVKHALOPREVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

class QvkHaloPreviewWidget: public QWidget
{
    Q_OBJECT
public:    
    virtual ~QvkHaloPreviewWidget();
    QvkHaloPreviewWidget( QWidget *parent, int diameterDefault );

    void setColor( QColor value );
    void setDiameter( int value );
    void setOpacity( double value );

    QColor getColor();

public slots:


private:
    QColor color;
    int diameter;
    double opacity;

    int getDiameter();
    double getOpacity();


private slots:


protected:
    void paintEvent( QPaintEvent *event );


signals:


};

#endif // QvkCircleWidget_H
