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

#ifndef QVKSHOWCLICK_H
#define QVKSHOWCLICK_H

#include <QObject>

#include "ui_formMainWindow.h"
#include "QvkPreviewWidget.h"
#include "QvkSpezialSlider.h"
#include "QvkSpezialCheckbox.h"
#include "QvkGlobalMouse.h"

class QvkShowClick : public QWidget //QObject
{
    Q_OBJECT

public:
    QvkShowClick();
    void init( Ui_formMainWindow *ui_formMainWindow );
    QvkSpezialCheckbox *vkSpezialCheckbox;
    bool pointerOnOff;
    QvkPreviewWidget *vkCircleWidget_1;
    QvkPreviewWidget *vkCircleWidget_2;
    QvkPreviewWidget *vkCircleWidget_3;

public slots:


private slots:
    void slot_vkPushButton_white();
    void slot_vkPushButton_black();
    void slot_vkPushButton_red();
    void slot_vkPushButton_darkRed();
    void slot_vkPushButton_green();
    void slot_vkPushButton_darkGreen();
    void slot_vkPushButton_blue();
    void slot_vkPushButton_darkBlue();
    void slot_vkPushButton_cyan();
    void slot_vkPushButton_darkCyan();
    void slot_vkPushButton_magenta();
    void slot_vkPushButton_darkMagenta();
    void slot_vkPushButton_yellow();
    void slot_vkPushButton_darkYellow();
    void slot_vkPushButton_gray();
    void slot_vkPushButton_darkGray();

    void slot_valueChanged_SpezialSlider_Diameter( int value );
    void slot_valueChanged_SpezialSlider_Opacity( int value );

    void slot_spezialSlider_Diameter_Default();
    void slot_spezialSlider_Opacity_Default();
    void slot_spezialSlider_showTime_Default();

    void slot_pointerOnOff( bool value );
    void slot_mousePressed( int, int, QString );

protected:


private:
    Ui_formMainWindow *ui;

    QvkSpezialSlider *vkSpezialSliderCircle;
    QvkSpezialSlider *vkSpezialSliderOpacity;
    QvkSpezialSlider *vkSpezialSliderShowtime;

    void setColorButtons();
    void setSpezialSlider();
    void setSpezialCheckBox();
    void setToolButtonDefaultValues();
    void setCircleWidget();
    void setMouseClick();

    QvkGlobalMouse *vkGlobalMouse;

signals:

    
};
#endif
