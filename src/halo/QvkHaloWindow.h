/* vokoscreenNG
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#ifndef QvkHaloWindow_H
#define QvkHaloWindow_H

#include <QWidget>
#include <QTimer>

#include "ui_formMainWindow.h"
#include "QvkSpezialCheckbox.h"

class QvkHaloWindow: public QWidget

{
    Q_OBJECT
public:
    QvkHaloWindow(QWidget *parent, Ui_formMainWindow *ui_formMainWindow );
    void init();

    virtual ~QvkHaloWindow();
    
public:


public slots:
    void slot_haloOnOff( bool value );

    void slot_vkPushButton_halo_white();
    void slot_vkPushButton_halo_black();
    void slot_vkPushButton_halo_red();
    void slot_vkPushButton_halo_darkRed();
    void slot_vkPushButton_halo_green();
    void slot_vkPushButton_halo_darkGreen();
    void slot_vkPushButton_halo_blue();
    void slot_vkPushButton_halo_darkBlue();
    void slot_vkPushButton_halo_cyan();
    void slot_vkPushButton_halo_darkCyan();
    void slot_vkPushButton_halo_magenta();
    void slot_vkPushButton_halo_darkMagenta();
    void slot_vkPushButton_halo_yellow();
    void slot_vkPushButton_halo_darkYellow();
    void slot_vkPushButton_halo_gray();
    void slot_vkPushButton_halo_darkGray();


private:
    QvkSpezialCheckbox *vkSpezialCheckbox;
    int diameter;
    QColor color;
    qreal opacity;
    QTimer *timer;
    Ui_formMainWindow *ui;

    void setColorButtons();
    void setTimer();
    void setSpezialCheckBox();


private slots:
    void slot_followMouse();


protected:
    void paintEvent(QPaintEvent *event);


};

#endif // QvkHaloWindow
