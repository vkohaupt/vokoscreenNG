/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2024 Volker Kohaupt
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

#ifndef QVKCAMERASURFACE_WL_H
#define QVKCAMERASURFACE_WL_H

#include <QObject>
#include <QWidget>
#include <QGuiApplication>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>

class QvkCameraSurface_wl : public QWidget
{
    Q_OBJECT
public:
    QvkCameraSurface_wl();
    virtual ~QvkCameraSurface_wl();
    void set_CameraImage(QPixmap pixmap);

private:
    QPixmap pixmap;
    int a = 0;
    QPixmap cameraImage;
    QPixmap get_CameraImage();
    bool mousePressed = false;
    int currentMouse_X = 0;
    int currentMouse_Y = 0;

private slots:


public slots:



signals:


protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void leaveEvent( QEvent *event );
//    void keyPressEvent( QKeyEvent * event );

};

#endif // QVKCAMERASURFACE_WL_H
