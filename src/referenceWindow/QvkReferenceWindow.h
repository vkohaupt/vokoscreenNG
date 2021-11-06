/* vokoscreenNG - A desktop recorder
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

#ifndef QvkReferenceWindow_H
#define QvkReferenceWindow_H

#include "ui_formMainWindow.h"

#include <QWindow>
#include <QShowEvent>
#include <QExposeEvent>

class QvkReferenceWindow : public QWindow
{
    Q_OBJECT

public:
    QvkReferenceWindow( QWidget *parent = 0 );
    virtual ~QvkReferenceWindow();
    void init( Ui_formMainWindow *vk_ui );


public slots:


private slots:
    void slot_make_picture_from_tab();
    void slot_make_picture_from_tabs();


signals:


protected:  


private:
    Ui_formMainWindow *ui = Q_NULLPTR;
    QWidget *widget = Q_NULLPTR;
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
    int counterFile = 0;

};

#endif
