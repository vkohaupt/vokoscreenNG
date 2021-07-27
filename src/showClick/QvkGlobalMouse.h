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

#ifndef QVKGLOBALMOUSE_H
#define QVKGLOBALMOUSE_H

#include <QObject>
#include <QTimer>

class QvkGlobalMouse: public QObject
{
    Q_OBJECT
public:    
    QvkGlobalMouse();
    virtual ~QvkGlobalMouse();
    QTimer *timer;


public slots:
//    void setCursorOn();
//    void setCursorOff();
    void slot_mousePressed();
    void slot_on( bool value );


private:
#ifdef Q_OS_LINUX
    int pressed = 0;
#endif

#ifdef Q_OS_WIN
    bool once = false;
#endif


private slots:


protected:


signals:
    void signal_mousePressed( int win_x, int win_y, QString mouseButton );

};

#endif // QVKGLOBALMOUSE_H
