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

void QvkSettings_wl::saveAll( Ui_formMainWindow_wl *ui_mainwindow , QMainWindow *parent )
{
    Q_UNUSED(parent);
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( global::name + "_wl" );

    settings.remove( "" );
    settings.setValue( "Version", global::version );

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralwidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralwidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ ) {
        settings.setValue( listRadiobuttons.at(i)->objectName(), listRadiobuttons.at(i)->isChecked() );
    }

    QList<QToolButton *> listToolButton = ui_mainwindow->centralwidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        settings.setValue( listToolButton.at(i)->objectName(), listToolButton.at(i)->isChecked() );
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralwidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ ) {
        settings.setValue( listComboBox.at(i)->objectName(), listComboBox.at(i)->currentText() );
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralwidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ ) {
        settings.setValue( listSlider.at(i)->objectName(), listSlider.at(i)->value() );
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralwidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ ) {
        settings.setValue( listLineEdit.at(i)->objectName(), listLineEdit.at(i)->text() );
    }

    settings.endGroup();
}

