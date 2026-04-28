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

#include "global.h"
#include "QvkCameraSurface_wl.h"

#include <QDebug>
#include <QIcon>
#include <QString>
#include <QSize>
#include <QImage>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QPoint>

QvkCameraSurface_wl::QvkCameraSurface_wl()
{
    setWindowTitle(QString(tr("Camera")));

    QIcon icon;
    icon.addFile(QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_QuitOnClose, false);
    setMouseTracking(true);
    showMaximized();

    QPixmap pixmap(QString::fromUtf8(":/pictures/logo/logo.png"));
    set_CameraImage(pixmap);

    // imageRect beinhaltet den Standort x, y und die Abmaße Breite Höhe des Bildes
    imageRect.setRect(200, 200, cameraImage.width(), cameraImage.height());
}

QvkCameraSurface_wl::~QvkCameraSurface_wl()
{
}


void QvkCameraSurface_wl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);

    QPainter painterPixmap;
    painterPixmap.begin(&image);
    {
        painterPixmap.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painterPixmap.drawPixmap(imageRect.x(),
                                 imageRect.y(),
                                 cameraImage);

        QPen pen(Qt::red, 3);
        pen.setJoinStyle(Qt::MiterJoin);
        painterPixmap.setPen(pen);
        painterPixmap.drawRect(0, 0, width(), height());
    }
    painterPixmap.end();

    pixmap = pixmap.fromImage(image);

    QPainter painter;
    painter.begin(this);
    {
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.drawPixmap(QPoint(0, 0), pixmap);
    }
    painter.end();

    if(a == 0){
        setMask(pixmap.mask());
        a++;
    }
}


void QvkCameraSurface_wl::set_CameraImage(QPixmap pixmap)
{
    cameraImage = pixmap;
}


void QvkCameraSurface_wl::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if(event->button() != Qt::LeftButton){
        return;
    }

    mousePressed = true;

    QPoint mousePos = event->position().toPoint();
    pointDiv = mousePos - imageRect.topLeft();

    clearMask();
    repaint();
    // Siehe Hinweis in mouseReleaseEvent
    resize(width()-1, height()-1);
}


void QvkCameraSurface_wl::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePressed == true){
        QPoint mousePos = event->position().toPoint();
        imageRect.setRect(mousePos.x() - pointDiv.x(),
                          mousePos.y() - pointDiv.y(),
                          cameraImage.width(),
                          cameraImage.height());
        repaint();
    }
}


void QvkCameraSurface_wl::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mousePressed = false;

    repaint();
    setMask(pixmap.mask());
    // Unter Gnome wird "setMask( pixmap.mask() )" nicht ausgeführt.
    // Selbst nach hunderten Tests ist nicht ersichtlich warum.
    // Abhilfe schafft ein umschalten mithilfe setVisible(false/true)
    // setVisible( false );
    // setVisible( true );

    // Oder ein resize, das Fenster flackert nicht bzw. zeigt keine Anzeichen einer Animation,
    // wie bei setVisible(false/true)
    resize(width()+1, height()+1);
}
