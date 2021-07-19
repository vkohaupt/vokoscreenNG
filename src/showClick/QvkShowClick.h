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
    QvkPreviewWidget *vkPreviewWidget;
    QColor colorDefault = Qt::red;


public slots:
    void slot_valueChanged_SpezialSlider_Diameter( int value );
    void slot_valueChanged_SpezialSlider_Opacity( int value );

    void slot_pointerOnOff( bool value );
    void slot_mousePressed( int, int, QString );


protected:


private:
    Ui_formMainWindow *ui;

    QvkSpezialSlider *vkSpezialSliderDiameter;
    QvkSpezialSlider *vkSpezialSliderOpacity;
    QvkSpezialSlider *vkSpezialSliderShowtime;

    void setColorButtons();
    void setSpezialSlider();
    void setSpezialCheckBox();
    void setToolButtonDefaultValues();
    void setPreviewWidget();
    void setGlobalMouse();

    QvkGlobalMouse *vkGlobalMouse;

    int diameterDefault = 60;
    int opacityDefault = 70;
    int timeDefault = 10;


signals:

    
};
#endif
