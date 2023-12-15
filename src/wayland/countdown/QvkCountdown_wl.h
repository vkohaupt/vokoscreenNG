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

#ifndef QVKCOUNTDOWN_WL_H
#define QVKCOUNTDOWN_WL_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QCloseEvent>

class QvkCountdown_wl: public QDialog
{
    Q_OBJECT

public:
    QvkCountdown_wl();
    int drawWidth;
    int drawHeight;
    int countValue;
    int gradValue;
    QTimer *timer;
    QTimer *animationTimer;


public slots:


private:
    QString cancelText = "Cancel";
    QRectF rectCancel;


private slots:
    void slot_updateTimer();
    void slot_updateAnimationTimer();


protected:
    void paintEvent( QPaintEvent *event );
    void mousePressEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *);


signals:
    void signal_countDownCancel();

};

#endif
