/* vokoscreen - A desktop recorder
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

#include "QvkSettings.h"
#include "global.h"
#include "ui_formMainWindow.h"

QvkSettings::QvkSettings()
{
    QSettings settings( global::name, global::name);

    // Dient nur zum anlegen des Profils damit das log erstellt werden kann
    settings.beginGroup( global::name );
      settings.setValue( "Version", global::version );
    settings.endGroup();
}

QvkSettings::~QvkSettings(){}

void QvkSettings::readAll( Ui_formMainWindow *ui_mainwindow, QMainWindow *parent )
{
    QSettings settings( global::name, global::name );

    // Clear all settings if checkBoxResetAtNextStart is set
    QList<QCheckBox *> listCheckBoxReset = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBoxReset.count(); i++ )
    {
       if ( ( listCheckBoxReset.at(i)->objectName() == "checkBoxResetAtNextStart" ) and
            ( settings.value( listCheckBoxReset.at(i)->objectName(), false ).toBool() == true ) )
       {
           settings.clear();
       }
    }

    parent->move( settings.value( "MainWindow_X" ).toInt(), settings.value( "MainWindow_Y" ).toInt() );

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        QString valueText = settings.value( listComboBox.at(i)->objectName(), "" ).toString();
        int valueInt = listComboBox.at(i)->findText( valueText );
        if ( valueInt > -1 )
        {
            listComboBox.at(i)->setCurrentIndex( valueInt );
        }
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        // We have no settings-file(first start after install) but this object we want set as Standard.
        if ( ( listRadiobuttons.at(i)->objectName() == "radioButtonScreencastFullscreen" ) and
             ( settings.value( listRadiobuttons.at(i)->objectName(), true ).toBool() == true ) )
        {
            listRadiobuttons.at(i)->click();
            continue;
        }

        // We have no settings-file(first start after install) but this object we want set as Standard.
        if ( ( listRadiobuttons.at(i)->objectName() == "radioButtonScreenshotFullscreen" ) and
             ( settings.value( listRadiobuttons.at(i)->objectName(), true ).toBool() == true ) )
        {
            listRadiobuttons.at(i)->click();
            continue;
        }

        bool value = settings.value( listRadiobuttons.at(i)->objectName(), false ).toBool();
        if ( value == true )
        {
           listRadiobuttons.at(i)->click();
        }
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        // We have no settings-file(first start after install) but this object we want set as Standard.
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxShowInSystray" ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) )
        {
            listCheckBox.at(i)->click();
            continue;
        }

        // We have no settings-file(first start after install), audio is off and the tab have a red cross
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxAudioOnOff" ) and
             ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == false ) )
        {
            listCheckBox.at(i)->click();
            listCheckBox.at(i)->click();
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName() == "checkBoxStartsMinimized" ) and
            ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true ) )
        {
            parent->showMinimized();
            listCheckBox.at(i)->click();
            continue;
        }

        // We found a setting, then we want set or not.
        if ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true )
        {
            listCheckBox.at(i)->click();
        }
    }

    QList<QSpinBox *> listSpinBox = ui_mainwindow->centralWidget->findChildren<QSpinBox *>();
    for ( int i = 0; i < listSpinBox.count(); i++ )
    {
        int value = settings.value( listSpinBox.at(i)->objectName(), 0 ).toInt();
        if ( value > 0  )
        {
           listSpinBox.at(i)->setValue( value );
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        QVariant variant = settings.value( listSlider.at(i)->objectName() );

        if ( listSlider.at(i)->objectName() == "sliderCameraWindowSize" )
        {
            if ( variant.isValid() )
            {
                listSlider.at(i)->setValue( variant.toInt() );
                listSlider.at(i)->setMinimum(1);
                continue;
            }
            else
            {
                listSlider.at(i)->setValue(2);
                listSlider.at(i)->setMinimum(1);
                continue;
            }
        }

        if ( variant.isValid() )
        {
            listSlider.at(i)->setValue( variant.toInt() );
        }
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ )
    {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) )
        {
            QString value = settings.value( listLineEdit.at(i)->objectName() ).toString();
            if ( !value.isEmpty() )
            {
               listLineEdit.at(i)->setText( value );
            }
        }
    }

    // All TabWidget start with value=0, also if in Qt-Creator is the value an other
    QList<QTabWidget *> listTabWidget = ui_mainwindow->centralWidget->findChildren<QTabWidget *>();
    for ( int i = 0; i < listTabWidget.count(); i++ )
    {
        listTabWidget.at(i)->setCurrentIndex( 0 );
    }

}


void QvkSettings::saveAll(Ui_formMainWindow *ui_mainwindow , QMainWindow *parent)
{
    QSettings settings( global::name, global::name );
    settings.clear();

    settings.beginGroup( global::name );
       settings.setValue( "Version", global::version );
    settings.endGroup();

    settings.setValue( "MainWindow_X", parent->pos().x() );
    settings.setValue( "MainWindow_Y", parent->pos().y() );

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        settings.setValue( listRadiobuttons.at(i)->objectName(), listRadiobuttons.at(i)->isChecked() );
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxStopRecordingAfter" ) or
             ( listCheckBox.at(i)->objectName() == "checkBoxStartTime" ) )
        {
            // We do not save
        }
        else
        {
            settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
        }
    }

    QList<QSpinBox *> listSpinBox = ui_mainwindow->centralWidget->findChildren<QSpinBox *>();
    for ( int i = 0; i < listSpinBox.count(); i++ )
    {
        settings.setValue( listSpinBox.at(i)->objectName(), listSpinBox.at(i)->value() );
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        settings.setValue( listComboBox.at(i)->objectName(), listComboBox.at(i)->currentText() );
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        settings.setValue( listSlider.at(i)->objectName(), listSlider.at(i)->value() );
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ )
    {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) )
        {
            settings.setValue( listLineEdit.at(i)->objectName(), listLineEdit.at(i)->text() );
        }
    }
}

QString QvkSettings::getFileName()
{
    QSettings settings( global::name, global::name );
    return settings.fileName();
}


void QvkSettings::saveAreaScreencast( int x, int y, int width, int height  )
{
    QSettings settings( global::name, global::name );
    settings.beginGroup( "AreaScreencast" );
      settings.setValue( "X", x );
      settings.setValue( "Y", y );
      settings.setValue( "Width", width );
      settings.setValue( "Height", height );
    settings.endGroup();
}

void QvkSettings::readAreaScreencast( QvkRegionChoise *vkRegionChoise )
{
    QSettings settings( global::name, global::name );
    settings.beginGroup( "AreaScreencast" );
      vkRegionChoise->setX( settings.value( "X", 200 ).toInt() );
      vkRegionChoise->setY( settings.value( "Y", 200 ).toInt() );
      vkRegionChoise->setWidth( settings.value( "Width", 320 ).toInt() );
      vkRegionChoise->setHeight( settings.value( "Height", 200 ).toInt() );
    settings.endGroup();
}

void QvkSettings::saveCamera( int x, int y )
{
    QSettings settings( global::name, global::name );
    settings.beginGroup( "Camera" );
    settings.setValue( "X", x );
    settings.setValue( "Y", y );
    settings.endGroup();
}

void QvkSettings::readCamera( QvkCameraController *vkCameraController )
{
    QSettings settings( global::name, global::name );
    settings.beginGroup( "Camera" );
      vkCameraController->cameraWindow->move( settings.value( "X", 0 ).toInt(), settings.value( "Y", 0 ).toInt() );
    settings.endGroup();
}

