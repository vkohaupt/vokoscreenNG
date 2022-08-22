/* vokoscreenNG
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#ifndef QVKHALO_H
#define QVKHALO_H

#include <QWidget>
#include <QTimer>

#include "QvkSpezialSlider.h"
#include "QvkHaloPreviewWidget.h"
#include "QvkSpezialCheckbox.h"
#include "QvkHaloWindow.h"

#include "ui_formMainWindow.h"

class QvkHalo: public QWidget

{
    Q_OBJECT
public:
    QvkHalo();
    virtual ~QvkHalo();
    void init( Ui_formMainWindow *ui_formMainWindow );
    QvkSpezialSlider *vkSpezialSliderDiameter;
    QvkSpezialSlider *vkSpezialSliderOpacity;
    QvkSpezialSlider *vkSpezialSliderHole;
    QvkHaloPreviewWidget *vkHaloPreviewWidget;
    QvkHaloWindow *vkHaloWindow;

    QColor colorDefault = Qt::green;


public slots:


private:
    Ui_formMainWindow *ui;
    QvkSpezialCheckbox *vkSpezialCheckbox;

    int diameterDefault = 70;
    int opacityDefault = 60;
    int holeDefault = 5;
    QTimer *timer;
    QPoint oldPos = QPoint( -1, -1 );

    void createHaloPreviewWidget();
    void createSpezialSlider();
    void createColorButtons();
    void createSpezialCheckBox();
    void setToolButtonDefaultValues();


private slots:
    void slot_valueChanged_SpezialSlider_Diameter( int value );
    void slot_valueChanged_SpezialSlider_Opacity( int value );
    void slot_valueChanged_SpezialSlider_Hole( int value );
    void slot_haloOnOff( bool value );
    void slot_mytimer();


protected:


};

#endif // QvkHalo
