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

#include <QSettings>
#include <QList>
#include <QToolButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QLineEdit>
#include <QStandardPaths>
#include <QDateTime>

QvkSettings_wl::QvkSettings_wl()
{
    // Dient nur zum anlegen des Profils damit das log erstellt werden kann
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, folderName_wl, fileName_wl, Q_NULLPTR );
    settings.beginGroup( groupName_wl );
    settings.setValue( "Version", global::version );
    settings.endGroup();

    QDateTime time;
    time.setMSecsSinceEpoch( QDateTime::currentDateTime().currentMSecsSinceEpoch() );
    QSettings installSetting( QSettings::IniFormat, QSettings::UserScope, folderName_wl, QString( "InstallTime" ), Q_NULLPTR );
    installSetting.beginGroup( global::name );
    if ( installSetting.value( "version", "0.0.0" ).toString() != global::version ) {
        installSetting.setValue( "time", time.toString( "yyyy.MM.dd-hh:mm:ss:zzz" ) );
        installSetting.setValue( "version", global::version );
    }
    installSetting.endGroup();
}


QvkSettings_wl::~QvkSettings_wl(){}

void QvkSettings_wl::readAll( Ui_formMainWindow_wl *ui_mainwindow, QMainWindow *parent )
{
    Q_UNUSED(parent)
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, fileName_wl, Q_NULLPTR );
    settings.beginGroup( groupName_wl );

    QList<QToolButton *> listToolButton = ui_mainwindow->centralwidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        // We found a setting, then we want set or not.
        QToolButton *toolButton = listToolButton.at(i);
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( toolButton->objectName() == ui_mainwindow->toolButtonScreencast->objectName() ) and
            ( settings.value( toolButton->objectName(), true ).toBool() == true ) ) {
            toolButton->click();
            continue;
        }

        if ( ( toolButton->objectName() == ui_mainwindow->toolButtonSnapshot->objectName() ) or
             ( toolButton->objectName() == ui_mainwindow->toolButtonConvert->objectName() ) or
             ( toolButton->objectName() == ui_mainwindow->toolButtonCamera->objectName() ) or
             ( toolButton->objectName() == ui_mainwindow->toolButtonLog->objectName() )
             ) {
            ui_mainwindow->toolButtonScreencast->click();
            continue;
        }

        if ( settings.value( toolButton->objectName(), false ).toBool() == true ) {
            toolButton->click();
        }
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralwidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ ) {
        QRadioButton *radioButton = listRadiobuttons.at(i);
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( radioButton->objectName() == ui_mainwindow->radioButtonScreencastFullscreen->objectName() ) and
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
        QCheckBox *checkBox = listCheckBox.at(i);
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( checkBox->objectName() == "checkBoxShowInSystray" ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) ) {
            checkBox->click();
            continue;
        }
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( checkBox->objectName() == "checkBoxSnapshotShowBallonInSystray" ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) ) {
            checkBox->click();
            continue;
        }
        // We found a setting, then we want set or not.
        if ( settings.value( checkBox->objectName(), false ).toBool() == true ) {
            checkBox->click();
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralwidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ ) {
        QSlider *slider = listSlider.at(i);

        if ( slider->objectName() == "sliderFrames" ) {
            QVariant variant = settings.value( slider->objectName() );
            if ( variant.isValid() ) {
                slider->setValue( variant.toInt() );
            } else {
                slider->setValue( 25 );
            }
            continue;
        }

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

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralwidget->findChildren<QLineEdit *>( "lineEditVideoPath" );
    for ( int i = 0; i < listLineEdit.count(); i++ ) {
        QLineEdit *lineEdit = listLineEdit.at(i);
        QString valueText = settings.value( lineEdit->objectName(), "" ).toString();
        if ( valueText > "" ) {
            lineEdit->setText( valueText );
        } else {
            lineEdit->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
        }
    }

    settings.endGroup();
}


void QvkSettings_wl::saveAll( Ui_formMainWindow_wl *ui_mainwindow , QMainWindow *parent )
{
    Q_UNUSED(parent);
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, folderName_wl, fileName_wl, Q_NULLPTR );
    settings.beginGroup( groupName_wl );

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


QString QvkSettings_wl::getFileName()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, fileName_wl, Q_NULLPTR );
    return settings.fileName();
}


QString QvkSettings_wl::getVideoPath()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, fileName_wl, Q_NULLPTR );
    return settings.value( "lineEditVideoPath" ).toString();
}


QString QvkSettings_wl::getPicturePath()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, fileName_wl, Q_NULLPTR );
    return settings.value( "lineEditSnapshotImagePath" ).toString();
}


void QvkSettings_wl::saveAreaScreencast( qreal x, qreal y, qreal width, qreal height  )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, fileName_wl, Q_NULLPTR );
    settings.beginGroup( "AreaScreencast_wl" );
    settings.setValue( "X", QVariant(x).toInt() );
    settings.setValue( "Y", QVariant(y).toInt() );
    settings.setValue( "Width", QVariant(width).toInt() );
    settings.setValue( "Height", QVariant(height).toInt() );
    settings.endGroup();
}


void QvkSettings_wl::readAreaScreencast( QvkRegionChoise_wl *vkRegionChoise )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, fileName_wl, Q_NULLPTR );
    settings.beginGroup( "AreaScreencast_wl" );
    vkRegionChoise->setX( settings.value( "X", 200 ).toInt() );
    vkRegionChoise->setY( settings.value( "Y", 200 ).toInt() );
    vkRegionChoise->setWidth( settings.value( "Width", 320 ).toInt() );
    vkRegionChoise->setHeight( settings.value( "Height", 200 ).toInt() );
    settings.endGroup();
}
