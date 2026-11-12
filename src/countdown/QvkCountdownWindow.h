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

#ifndef QVKCOUNTDOWNWINDOW_H
#define QVKCOUNTDOWNWINDOW_H

// Qt include checked
// In einer .h zuerst Qt dann die eigenen
#include <QWidget>

class QvkCountdownWindow: public QWidget
{
    Q_OBJECT
public:
    explicit QvkCountdownWindow(QWidget *parent = nullptr);
    virtual ~QvkCountdownWindow();
    int x = 0;
    int y = 0;
    int Width = 0;
    int Height = 0;
    int countValue = 0;
    int gradValue = 0;

    
public slots:


private:
    int oldCountValue = 0;
    QString cancelText = "Cancel";
    QRectF rectCancel;


private slots:


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


signals:
    void signal_countDownCancel( bool );

};

#endif
