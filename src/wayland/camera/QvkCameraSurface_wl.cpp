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
#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QPoint>
#include <QVideoFrame>
#include <QPainterPath>

QvkCameraSurface_wl::QvkCameraSurface_wl()
{
    setWindowTitle(QString(tr("Camera")));

    QIcon icon;
    icon.addFile(QString::fromUtf8( ":/pictures/logo/logo.png" ), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_QuitOnClose, false);
    showMaximized();
    setMouseTracking(true);
    imageRect.setX(100);
    imageRect.setY(100);
}

QvkCameraSurface_wl::~QvkCameraSurface_wl()
{
}


void QvkCameraSurface_wl::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_cameraSurfaceClose();
}


void QvkCameraSurface_wl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);

    QPainter painterPixmap;
    painterPixmap.begin(&image);
    {
        QPainterPath path;
        path.addEllipse( imageRect.x(),
                         imageRect.y(),
                         cameraImage.width(),
                         cameraImage.height());
        painterPixmap.setClipPath(path);


        painterPixmap.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painterPixmap.setRenderHint(QPainter::Antialiasing, true);
        painterPixmap.drawPixmap(imageRect.x(),
                                 imageRect.y(),
                                 cameraImage);
        /*
        if (mouseHover == true){
            QPen pen(Qt::red, 3);
            pen.setJoinStyle(Qt::MiterJoin);
            painterPixmap.setPen(pen);
            painterPixmap.drawEllipse(imageRect.x()+100,
                                      imageRect.y()+100,
                                      imageRect.width()-200,
                                      imageRect.height()-200);
        }

        QPen pen(Qt::red, 3);
        pen.setJoinStyle(Qt::MiterJoin);
        painterPixmap.setPen(pen);
        painterPixmap.drawRect(0, 0, width(), height());
*/
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
}


void QvkCameraSurface_wl::slot_setCameraImage(QVideoFrame videoFrame)
{
//    QImage image = videoFrame.toImage();
//    image = image.convertedTo( QImage::Format_ARGB32 );
//    cameraImage = pixmap.fromImage(image);
//    QPixmap pixmap(QString::fromUtf8( ":/pictures/logo/logo.png"));
//    cameraImage = pixmap;

    cameraImage = pixmap.fromImage(videoFrame.toImage());
    repaint();

    if(a == 0){
        setMask(pixmap.mask());
        a++;
    }
}


// Mausbutton wird gedrückt
void QvkCameraSurface_wl::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton){
        return;
    }

    QPixmap pixmap(":/pictures/cursor/size_all.png");
    QCursor cursor(pixmap);
    setCursor(cursor);

    mousePressed = true;

    QPoint mousePos = event->position().toPoint();
    pointDiv = mousePos - imageRect.topLeft();

    clearMask();
    // Ein repaint wird nicht benötigt da das Videobild immer wieder neu aufgebaut wird

    // Siehe Hinweis in mouseReleaseEvent
    resize(width()-1, height()-1);
}


// Mauszeiger bewegt sich im Objekt
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


// Mausbutton wird lossgelassen
void QvkCameraSurface_wl::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mousePressed = false;

    // Ein repaint wird nicht benötigt da das Videobild immer wieder neu aufgebaut wird
    setMask(pixmap.mask());

    // Unter Gnome wird "setMask( pixmap.mask() )" nicht ausgeführt.
    // Selbst nach hunderten Tests ist nicht ersichtlich warum.
    // Abhilfe schafft ein umschalten mithilfe setVisible(false/true)
    // setVisible( false );
    // setVisible( true );

    // Oder ein resize, das Fenster flackert nicht bzw. zeigt keine Anzeichen einer Animation,
    // wie bei setVisible(false/true)
    resize(width()+1, height()+1);

    unsetCursor();
}


// Mauzeiger tritt in das Objekt ein
void QvkCameraSurface_wl::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)
    mouseHover = true;
    // Ein repaint wird nicht benötigt da das Videobild immer wieder neu aufgebaut wird
}


// Mauszeiger verläßt das Objekt
void QvkCameraSurface_wl::leaveEvent(QEvent *event )
{
    Q_UNUSED(event)
    mouseHover = false;
    // Ein repaint wird nicht benötigt da das Videobild immer wieder neu aufgebaut wird
}
