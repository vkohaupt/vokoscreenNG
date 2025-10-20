/* vokoscreenNG
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

#ifndef QVKPUSHBUTTON_H
#define QVKPUSHBUTTON_H

#include <QPushButton>
#include <QColor>
#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEnterEvent>

class QvkPushButton: public QPushButton

{
    Q_OBJECT
public:
    QvkPushButton( QColor color );
    virtual ~QvkPushButton();
    
public:


public slots:


private slots:


private:
    QColor color;
    bool mouseHover = false;
    bool mousePressed = false;

protected:
    void paintEvent( QPaintEvent *event );
    void enterEvent( QEnterEvent *event );
    void leaveEvent( QEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
};

#endif
