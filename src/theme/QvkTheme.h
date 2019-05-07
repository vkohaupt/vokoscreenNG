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

#ifndef QVKTHEME_H 
#define QVKTHEME_H

#include "ui_formMainWindow.h"

class QvkTheme: public QObject
{ 
    Q_OBJECT
public:
    QvkTheme(Ui_formMainWindow *ui_formMainWindow );
    virtual ~QvkTheme();
    QIcon VK_getIcon( QString iconName, QString iconNameFallback );
    void makeAndSetValidIcon( QTabWidget *tabWidget, int index , QIcon icon );
    void vk_setCornerWidget( QTabWidget *tabWidget );


public slots:


private slots: 
    void slot_searchNewTheme();
    void slot_setNewTheme();


signals:
    void signal_newTheme();


protected:  


private:
    QString oldThemeName = "";
    Ui_formMainWindow *ui;

    QTimer *timer;

    void makeAndSetValidIconForSideBar( int index, QIcon icon );


};

#endif
