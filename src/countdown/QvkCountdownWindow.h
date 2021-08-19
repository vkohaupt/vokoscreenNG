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

#ifndef QVKCOUNTDOWNWINDOW_H
#define QVKCOUNTDOWNWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QDialogButtonBox>

class QvkCountdownWindow: public QWidget
{
    Q_OBJECT
public:
    QvkCountdownWindow();
    virtual ~QvkCountdownWindow();
    void startCountdown(int value );
    int x;
    int y;
    int Width;
    int Height;

    
public slots:

  
private:
    int countValue;
    int oldCountValue;
    int gradValue;
    QTimer *timer;
    QTimer *animationTimer;
    QPainter painter;
    QString cancelText = "Cancel";
    QRectF rectCancel;


private slots:
   void slot_updateTimer();
   void slot_updateAnimationTimer();


protected:
    void paintEvent( QPaintEvent *event );
    void mousePressEvent(QMouseEvent *event);
  

signals:
    void signal_countDownfinish( bool value );
    void signal_countdownBegin( bool value );
    void signal_countDownCancel( bool );

};

#endif
