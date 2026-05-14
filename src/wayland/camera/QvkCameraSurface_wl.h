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
#include <QEnterEvent>
#include <QEvent>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include <QVideoFrame>
#include <QToolButton>
#include <QCheckBox>

class QvkCameraSurface_wl : public QWidget
{
    Q_OBJECT
public:
    QvkCameraSurface_wl();
    virtual ~QvkCameraSurface_wl();

    void set_toolButtonRectangle(QToolButton *button);
    void set_toolButtonElipse(QToolButton *button);
    void set_toolButtonCircle(QToolButton *button);
    QToolButton *toolButtonRectangle;
    QToolButton *toolButtonElipse;
    QToolButton *toolButtonCircle;

    void set_toolButtonCameraMirrorVertical(QToolButton *toolButton);
    void set_toolButtonCameraMirrorHorizontal(QToolButton *toolButton);
    QToolButton *toolButtonCameraMirrorHorizontal;
    QToolButton *toolButtonCameraMirrorVertical;


    QPixmap pixmap;
    QRect imageRect;

    void set_newImageRect();
    bool is_setNewImageRect = false;


private:
    int a = 0;
    QPixmap cameraImage;
    bool mousePressed = false;
    QPoint pointDiv;
    bool mouseHover = false;

private slots:
    void slot_workaroundForGnome_1();
    void slot_workaroundForGnome_2();


public slots:
    void slot_setCameraImage(QVideoFrame videoFrame);


signals:
    void signal_cameraSurfaceClose();


protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event );
    void closeEvent(QCloseEvent *event);

};

#endif // QVKCAMERASURFACE_WL_H
