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

#include "QvkSettings_wl.h"
#include "global.h"

QvkSettings_wl::QvkSettings_wl()
{
    // Dient nur zum anlegen des Profils damit das log erstellt werden kann
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( global::name + "_wl");
    settings.setValue( "Version", global::version );
    settings.endGroup();
}

QvkSettings_wl::~QvkSettings_wl(){}

void QvkSettings_wl::readAll( Ui_formMainWindow_wl *ui_mainwindow, QMainWindow *parent )
{
}

void QvkSettings_wl::saveAll( Ui_formMainWindow_wl *ui_mainwindow , QMainWindow *parent, bool log )
{
}

