/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#ifndef QVKWININFO_H
#define QVKWININFO_H

#include <QTimer>
#include <QPainter>
#include <QWidget>

#ifdef Q_OS_LINUX
typedef QList<WId> WindowList;
#endif

class QvkWinInfo: public QWidget
{ 
    Q_OBJECT
public:
    QvkWinInfo();
    virtual ~QvkWinInfo();
    WId getWinID();
    static WId activeWindow();
    QRectF windowGeometryWithoutFrame( WId child);


public slots:
    void slot_start();


private slots: 
    void slot_selectWindow();
    void slot_mousePosition();


signals:
    void signal_windowChanged( bool value );
    void signal_showCursor( bool value );


protected:  
    void paintEvent( QPaintEvent *event );


private:
    QTimer *windowTimer;
    QTimer *mouseTimer;

    WId lastWinID;
    WId newWinID;
};

#endif // QVKWININFO_H
