/* vokoscreenNG - A desktop recorder
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

#ifndef QVKSHOWMESSAGE_WL_H
#define QVKSHOWMESSAGE_WL_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QTimer>
#include <QPixmap>

class QvkShowMessage_wl: public QDialog
{
    Q_OBJECT

public:
    QvkShowMessage_wl();
    void showMessage( QString text );
    void set_StatusIcon( QString m_statusIcon );
    void set_Image( QString m_image );
    void set_timeOut( qreal value );


public slots:


private:
    int drawWindowWidth;
    int drawWindowHeight;
    QString statusIcon;
    QString image;
    QTimer *timer;
    qreal degree = 0;
    qreal degreeStep;
    QPixmap pixmapDuration;
    qreal timerInterval = 100;
    qreal timeOut = 10000;
    int margin = 10; // Abstand zum Bildschirmrand
    bool isOverCloseButton = false;

private slots:
    void slot_durationButton();


protected:
    void paintEvent( QPaintEvent *event );
    void leaveEvent( QEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );


signals:


};

#endif
