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
    Q_UNUSED(parent)
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( global::name + "_wl" );

    QList<QToolButton *> listToolButton = ui_mainwindow->centralwidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        // We found a setting, then we want set or not.
        QToolButton *toolButton = listToolButton.at(i);
        if ( settings.value( toolButton->objectName(), false ).toBool() == true ) {
            // Toolbutton on Sidebar not clicked
            if ( ( toolButton->objectName() == "toolButtonScreencast" ) or
                 ( toolButton->objectName() == "toolButtonSnapshot" ) or
                 ( toolButton->objectName() == "toolButtonCamera" ) or
                 ( toolButton->objectName() == "toolButtonLog" )
                 ) {
                // do nothing
            } else {
                toolButton->click();
            }
        }
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralwidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ ) {
        QRadioButton *radioButton = listRadiobuttons.at(i);
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( radioButton->objectName() == "radioButtonScreencastFullscreen" ) and
            ( settings.value( radioButton->objectName(), true ).toBool() == true ) ) {
            radioButton->click();
            continue;
        }

        if ( settings.value( radioButton->objectName(), false ).toBool() == true ) {
            radioButton->click();
        }
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralwidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        // We found a setting, then we want set or not.
        QCheckBox *checkBox = listCheckBox.at(i);
        if ( settings.value( checkBox->objectName(), false ).toBool() == true ) {
            checkBox->click();
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralwidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ ) {
        QSlider *slider = listSlider.at(i);
        QVariant variant = settings.value( slider->objectName() );
        if ( variant.isValid() ) {
            slider->setValue( variant.toInt() );
        }
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralwidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ ) {
        QComboBox *comboBox = listComboBox.at(i);
        QString valueText = settings.value( comboBox->objectName(), "" ).toString();
        int valueInt = comboBox->findText( valueText );
        if ( valueInt > -1 ) {
            comboBox->setCurrentIndex( valueInt );
        }
    }

    settings.endGroup();
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
        QCheckBox *checkBox = listCheckBox.at(i);
        settings.setValue( checkBox->objectName(), checkBox->isChecked() );
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralwidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ ) {
        QRadioButton *radioButton = listRadiobuttons.at(i);
        settings.setValue( radioButton->objectName(), radioButton->isChecked() );
    }

    QList<QToolButton *> listToolButton = ui_mainwindow->centralwidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        QToolButton *toolButton = listToolButton.at(i);
        settings.setValue( toolButton->objectName(), toolButton->isChecked() );
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralwidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ ) {
        QComboBox *comboBox = listComboBox.at(i);
        settings.setValue( comboBox->objectName(), comboBox->currentText() );
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralwidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ ) {
        QSlider *slider = listSlider.at(i);
        settings.setValue( slider->objectName(), slider->value() );
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralwidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ ) {
        QLineEdit *lineEdit = listLineEdit.at(i);
        settings.setValue( lineEdit->objectName(), lineEdit->text() );
    }

    settings.endGroup();
}

