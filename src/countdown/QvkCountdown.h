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

#ifndef QVKCOUNTDOWN_H
#define QVKCOUNTDOWN_H

#include <QWidget>

#include "QvkCountdownWindow.h"

class QvkCountdown: public QWidget
{
    Q_OBJECT
public:
    QvkCountdown();
    virtual ~QvkCountdown();
    void init();
    void startCountdown(int value );
    int x;
    int y;
    int Width;
    int Height;

    
public slots:


private:
    QvkCountdownWindow *vkCountdownWindow;
    void createCountdownWindow();
    QTimer *timer;
    QTimer *animationTimer;


private slots:
    void slot_updateTimer();
    void slot_updateAnimationTimer();
    void slot_cancel( bool value );


protected:


signals:
    void signal_countDownfinish( bool value );
    void signal_countdownBegin( bool value );
    void signal_countDownCancel( bool value);

};

#endif
