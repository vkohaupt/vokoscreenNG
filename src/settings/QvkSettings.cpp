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

#include "QvkSettings.h"
#include "global.h"
#include "ui_formMainWindow.h"

#include <QFileInfo>
#include <QMouseEvent>

QvkSettings::QvkSettings()
{
    // Dient nur zum anlegen des Profils damit das log erstellt werden kann
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( global::name );
    settings.setValue( "Version", global::version );
    settings.endGroup();
}

QvkSettings::~QvkSettings(){}

void QvkSettings::readAll( Ui_formMainWindow *ui_mainwindow, QMainWindow *parent )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );

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

    // These settings must be set in this order: Format, Videocodec, Audiocodec
    // 1. Format
    QList<QComboBox *> listComboBoxFormat = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBoxFormat.count(); i++ )
    {
        if ( listComboBoxFormat.at(i)->objectName() == "comboBoxFormat" )
        {
            QString valueText = settings.value( listComboBoxFormat.at(i)->objectName(), "" ).toString();
            int valueInt = listComboBoxFormat.at(i)->findText( valueText );
            if ( valueInt > -1 )
            {
                listComboBoxFormat.at(i)->setCurrentIndex( valueInt );
            }
        }
    }

    // 2. Videocodec
    QList<QComboBox *> listComboBoxVideo = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBoxVideo.count(); i++ )
    {
        if ( listComboBoxVideo.at(i)->objectName() == "comboBoxVideoCodec" )
        {
            QString valueText = settings.value( listComboBoxVideo.at(i)->objectName(), "" ).toString();
            int valueInt = listComboBoxVideo.at(i)->findText( valueText );
            if ( valueInt > -1 )
            {
                listComboBoxVideo.at(i)->setCurrentIndex( valueInt );
            }
        }
    }

    // 3. Audiocodec
    QList<QComboBox *> listComboBoxAudio = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBoxAudio.count(); i++ )
    {
        if ( listComboBoxAudio.at(i)->objectName() == "comboBoxAudioCodec" )
        {
            QString valueText = settings.value( listComboBoxAudio.at(i)->objectName(), "" ).toString();
            int valueInt = listComboBoxAudio.at(i)->findText( valueText );
            if ( valueInt > -1 )
            {
                listComboBoxAudio.at(i)->setCurrentIndex( valueInt );
            }
        }
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( listRadiobuttons.at(i)->objectName() == "radioButtonScreencastFullscreen" ) and
             ( settings.value( listRadiobuttons.at(i)->objectName(), true ).toBool() == true ) )
        {
            listRadiobuttons.at(i)->click();
            continue;
        }

        // We set WASAPI as default
        if ( ( listRadiobuttons.at(i)->objectName() == "radioButtonDirectSound" ) and
             ( settings.value( listRadiobuttons.at(i)->objectName(), false ).toBool() == false ) )
        {
            listRadiobuttons.at(i)->click();
            continue;
        }


        if ( ( listRadiobuttons.at(i)->objectName() == "radioButton_cisco_on" ) and
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
        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxShowInSystray" ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) )
        {
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

        if ( ( listCheckBox.at(i)->objectName().contains( "checkBoxLookForUpdates" ) ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) )
        {
            listCheckBox.at(i)->click();
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().contains( "checkBoxShowInSystrayAlternative" ) ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) )
        {
            listCheckBox.at(i)->click();
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().contains( "checkboxAudioDevice-" ) ) and
             ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true ) )
        {
            listCheckBox.at(i)->click();
            continue;
        }

        // We found a setting, then we want set or not.
        if ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true )
        {
            listCheckBox.at(i)->click();
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        if ( listSlider.at(i)->objectName() == "sliderVideo" )
        {
            continue;
        }

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

    QList<QToolButton *> listToolButton = ui_mainwindow->centralWidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ )
    {
        if ( listToolButton.at(i)->objectName().contains( "toolButtonMute" ) )
        {
            QString value = settings.value( listToolButton.at(i)->objectName() ).toString();
            if ( value == "audio-volume-muted")
            {
                listToolButton.at(i)->click();
                break;
            }
        }
    }
}

/*
 *
 */
void QvkSettings::saveAll(Ui_formMainWindow *ui_mainwindow , QMainWindow *parent, bool log )
{
    Q_UNUSED(parent);
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    if ( log == true )
    {
        // do nothing
    }
    else
        settings.clear();

    settings.beginGroup( global::name );
    if ( log == true )
    {
        // do nothing
    }
    else
        settings.setValue( "Version", global::version );
    settings.endGroup();

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        if ( log == true )
            qDebug().noquote().nospace() <<  listRadiobuttons.at(i)->objectName() << "=" << listRadiobuttons.at(i)->isChecked();
        else
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
            if ( log == true )
                qDebug().noquote().nospace() << listCheckBox.at(i)->objectName() << "=" << listCheckBox.at(i)->isChecked();
            else
                settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
        }
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        if ( log == true )
            qDebug().noquote().nospace() << listComboBox.at(i)->objectName() << "=" << listComboBox.at(i)->currentText();
        else
            settings.setValue( listComboBox.at(i)->objectName(), listComboBox.at(i)->currentText() );
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        if ( log == true )
            qDebug().noquote().nospace() << listSlider.at(i)->objectName() << "=" << listSlider.at(i)->value();
        else
            settings.setValue( listSlider.at(i)->objectName(), listSlider.at(i)->value() );
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ )
    {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) )
        {
            if ( log == true )
                qDebug().noquote().nospace() << listLineEdit.at(i)->objectName() << "=" << listLineEdit.at(i)->text();
            else
                settings.setValue( listLineEdit.at(i)->objectName(), listLineEdit.at(i)->text() );
        }
    }

    QList<QToolButton *> listToolButton = ui_mainwindow->centralWidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ )
    {
        if ( listToolButton.at(i)->objectName().contains( "toolButtonMute" ) )
        {
            if ( listToolButton.at(i)->isChecked() == true )
            {
                settings.setValue( listToolButton.at(i)->objectName(), "audio-volume-muted" );
            }
            else
            {
                settings.setValue( listToolButton.at(i)->objectName(), "audio-volume-high" );
            }
        }
    }
}

QString QvkSettings::getFileName()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    return settings.fileName();
}

QString QvkSettings::getVideoPath()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    return settings.value( "lineEditVideoPath" ).toString();
}

void QvkSettings::saveAreaScreencast( qreal x, qreal y, qreal width, qreal height  )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "AreaScreencast" );
    settings.setValue( "X", QVariant(x).toInt() );
    settings.setValue( "Y", QVariant(y).toInt() );
    settings.setValue( "Width", QVariant(width).toInt() );
    settings.setValue( "Height", QVariant(height).toInt() );
    settings.endGroup();
}

void QvkSettings::readAreaScreencast( QvkRegionChoise *vkRegionChoise )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "AreaScreencast" );
    vkRegionChoise->setX( settings.value( "X", 200 ).toInt() );
    vkRegionChoise->setY( settings.value( "Y", 200 ).toInt() );
    vkRegionChoise->setWidth( settings.value( "Width", 320 ).toInt() );
    vkRegionChoise->setHeight( settings.value( "Height", 200 ).toInt() );
    settings.endGroup();
}

void QvkSettings::saveCamera( int x, int y )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Camera" );
    settings.setValue( "X", x );
    settings.setValue( "Y", y );
    settings.endGroup();
}

void QvkSettings::readCamera( QvkCameraController *vkCameraController )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Camera" );
    vkCameraController->cameraWindow->move( settings.value( "X", 0 ).toInt(), settings.value( "Y", 0 ).toInt() );
    settings.endGroup();
}

void QvkSettings::saveSystrayAlternative( int x, int y )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "SystrayAlternative" );
    settings.setValue( "X", x );
    settings.setValue( "Y", y );
    settings.endGroup();
}

void QvkSettings::readSystrayAlternative( QvkSystrayAlternative *vkSystrayAlternative )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "SystrayAlternative" );
    vkSystrayAlternative->move( settings.value( "X", 0 ).toInt(), settings.value( "Y", 0 ).toInt() );
    settings.endGroup();
}

void QvkSettings::savePlayerPathOpenFile( QString pathFile )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Player" );
    QFileInfo fileInfo( pathFile );
    QString file = fileInfo.fileName();
    QString path = pathFile.replace( file, "" );
    settings.setValue( "Path", path );
    settings.endGroup();
}

void QvkSettings::readPlayerPathOpenFile( QvkPlayer *vkplayer )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Player" );
    vkplayer->pathOpenFile = settings.value( "Path", "" ).toString();
    settings.endGroup();
}

