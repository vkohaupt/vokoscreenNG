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

#include "ui_formMainWindow_wl.h"

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QEvent>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include <QVideoFrame>

class QvkCameraSurface_wl : public QWidget
{
    Q_OBJECT
public:
    QvkCameraSurface_wl(QString m_device);
    virtual ~QvkCameraSurface_wl();
    Ui_formMainWindow_wl *GuiUi;
    void set_GUIui(Ui_formMainWindow_wl *ui);
    QRect imageRect;
    void set_newImageRect();
    bool m_newImageRect = false;


private:
    QString device;
    QPixmap cameraImage;
    bool mousePressed = false;
    QPoint pointDiv;
    bool mouseHover = false;


private slots:
//    void slot_workaroundForGnome_1();
//    void slot_workaroundForGnome_2();


public slots:
    void slot_setCameraImage(QImage image);


signals:


protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

};

#endif // QVKCAMERASURFACE_WL_H
