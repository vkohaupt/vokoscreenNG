/* vokoShowClick
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

#ifndef QvkAnimateWindow_H
#define QvkAnimateWindow_H

#include <QWidget>

class QvkAnimateWindow: public QWidget

{
    Q_OBJECT
public:
    QvkAnimateWindow( QWidget *parent );
    void init(int x, int y, int showTime, QString button, int valueDiameter , double valueOpacity, QColor valueColor);

    virtual ~QvkAnimateWindow();
    
public:


public slots:


private:
    int diameter;
    QColor color;
    double opacity;
    QString mouseButton;



private slots:


protected:
    void paintEvent(QPaintEvent *event);


};

#endif // QvkAnimateWindow_H
