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

#include "QvkSettings.h"
#include "global.h"
#include "QvkSpezialCheckbox.h"

#include <QFileInfo>
#include <QMouseEvent>
#include <QColor>
#include <QDateTime>
#include <QDir>

QvkSettings::QvkSettings()
{
    // Dient nur zum anlegen des Profils damit das log erstellt werden kann
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( global::name );
    settings.setValue( "Version", global::version );
    settings.endGroup();

    QDateTime time;
    time.setMSecsSinceEpoch( QDateTime::currentDateTime().currentMSecsSinceEpoch() );
    QSettings installSetting( QSettings::IniFormat, QSettings::UserScope, global::name, QString( "InstallTime" ), Q_NULLPTR );
    installSetting.beginGroup( global::name );
    if ( installSetting.value( "version", "0.0.0" ).toString() != global::version ) {
        installSetting.setValue( "time", time.toString( "yyyy.MM.dd-hh:mm:ss:zzz" ) );
        installSetting.setValue( "version", global::version );
    }
    installSetting.endGroup();

    // Extra Verzeichnis für openh264 erstellen
    QSettings openh264Settings( QSettings::IniFormat, QSettings::UserScope, QString( global::name + "-" + "openh264" + "-" + global::version ), global::name, Q_NULLPTR );
    openh264Settings.beginGroup( global::name );
    openh264Settings.setValue( "Version", global::version );
    openh264Settings.endGroup();
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


    // We want block the signals for shortcut ComboBox signals and set the default value
    QList<QComboBox *> listComboBoxShortcut = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBoxShortcut.count(); i++ )
    {
        if ( listComboBoxShortcut.at(i)->objectName().contains( "comboBox_shortcut", Qt::CaseInsensitive ) == true )
        {
            listComboBoxShortcut.at(i)->blockSignals( true );

            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_start" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F10" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_pause" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F12" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_magnification" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F9" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_camera" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F8" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_showclick" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F7" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_halo" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F6" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
            if ( listComboBoxShortcut.at(i)->objectName() == "comboBox_shortcut_snapshot" ) {
                QString valueText = settings.value( listComboBoxShortcut.at(i)->objectName(), "F5" ).toString();
                listComboBoxShortcut.at(i)->setCurrentText( valueText );
            }
        }
    }

    // Then we set the values for all ComboBoxes
    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        // Language
        if ( listComboBox.at(i)->objectName() == "comboBoxLanguage" ) {


            // listComboBox.at(i)->removeItem( 20 ); // Nur zum testen. Index muß zuvor ermittelt werden.
            int index;

            // Wird eine Sprache in den Settings gefunden, wird diese wenn in der ComboBox vorhanden gesetzt
            QString valueText = settings.value( listComboBox.at(i)->objectName(), "" ).toString();
            index = listComboBox.at(i)->findText( valueText );
            if ( index > -1 ) {
                listComboBox.at(i)->setCurrentIndex( index );
                break;
            }

            // Systemsprache ist in der Combobox nicht vorhanden, es wird "en" gesetzt
            index = listComboBox.at(i)->findData( QLocale::system().name() );
            if ( index == -1 ) {
                int value = listComboBox.at(i)->findData( "en" );
                listComboBox.at(i)->setCurrentIndex( value );
                break;
            }

            // Systemsprache wird in der Combobox gesetzt wenn diese in der Combobox gefunden wird.
            index = listComboBox.at(i)->findData( QLocale::system().name() );
            if ( index > -1 ) {
                listComboBox.at(i)->setCurrentIndex( index );
            }

            break;
        }


        QString valueText = settings.value( listComboBox.at(i)->objectName(), "" ).toString();
        int valueInt = listComboBox.at(i)->findText( valueText );
        if ( valueInt > -1 )
        {
            listComboBox.at(i)->setCurrentIndex( valueInt );
        }
    }

    // After set all comboBox values we set the shortcut ComboBox signals
    for ( int i = 0; i < listComboBoxShortcut.count(); i++ )
    {
        if ( listComboBoxShortcut.at(i)->objectName().contains( "comboBox_shortcut", Qt::CaseInsensitive ) == true )
        {
            listComboBoxShortcut.at(i)->blockSignals( false );
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
        if ( ( listRadiobuttons.at(i)->objectName() == "radioButtonWASAPI" ) and
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
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxShowInSystray" ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == false ) )
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

        if ( ( listCheckBox.at(i)->objectName().contains( "checkBoxSnapshotShowBallonInSystray" ) ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) )
        {
            listCheckBox.at(i)->click();
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().contains( "checkBoxGstreamerDebugLevel" ) ) and
             ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true ) )
        {
            continue;
        }

        //
        if ( listCheckBox.at(i)->objectName().contains( "checkBox_shortcut" ) and
           ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true ) )
        {
            listCheckBox.at(i)->setChecked( true );
            continue;
        }

        // Start/Stop
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_start_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_start_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }

        // Pause/Continue
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_pause_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_pause_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }

        // Magnification
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_magnification_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_magnification_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }

        // Camera
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_camera_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_camera_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }

        // Showclick
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_showclick_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_showclick_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }

        // Halo
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_halo_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_halo_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }

        // Snapshot
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_snapshot_strg" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }
        if ( listCheckBox.at(i)->objectName() == "checkBox_shortcut_snapshot_shift" ) {
             if ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) {
                 listCheckBox.at(i)->setChecked( true );
                 continue;
             }
        }


        if ( ( listCheckBox.at(i)->objectName().contains( "checkBoxSnapshotHideBeforeRecording" ) ) and
             ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) ) {
            listCheckBox.at(i)->click();
            continue;
        }

        if ( ( listCheckBox.at(i)->objectName().contains( "checkBoxGifLoop" ) ) and
            ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) ) {
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

        if ( listSlider.at(i)->objectName() == "sliderGstDebugLevel" )
        {
            continue;
        }

        QVariant variant = settings.value( listSlider.at(i)->objectName() );
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

        QvkSettings vkSettingsGstDebug;
        QFileInfo fileInfo( vkSettingsGstDebug.getFileName() );
        QString pathAndFilename = fileInfo.absoluteFilePath();
        QSettings setingsGstDebug( pathAndFilename, QSettings::IniFormat );
        bool bo = setingsGstDebug.value( "checkBoxGstreamerDebugLevel" ).toBool();
        if ( bo == true ) {
            ui_mainwindow->tabWidgetScreencast->setCurrentIndex( 3 );
        }

    }

    QList<QToolButton *> listToolButton = ui_mainwindow->centralWidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ )
    {
        if ( listToolButton.at(i)->objectName().contains( "toolButtonMute" ) ) {
            QString value = settings.value( listToolButton.at(i)->objectName() ).toString();
            if ( value == "audio-volume-muted") {
                listToolButton.at(i)->click();
            }
        }

        if ( listToolButton.at(i)->objectName().contains( "toolButton_camera_view" ) ) {
            if ( settings.value( listToolButton.at(i)->objectName() ).toBool() == true ) {
                listToolButton.at(i)->click();
            }
        }

        if ( listToolButton.at(i)->objectName().contains( "toolButton_magnifier" ) ) {
            if ( settings.value( listToolButton.at(i)->objectName() ).toBool() == true ) {
                listToolButton.at(i)->click();
            }
        }
    }

    QList<QToolButton *> listToolButtonCameraView = ui_mainwindow->centralWidget->findChildren<QToolButton *>( "toolButton_camera_view_" );
    for ( int i = 0; i < listToolButtonCameraView.count(); i++ ) {
        if ( listToolButtonCameraView.at(i)->objectName().contains( "toolButton_camera_view_Rectangle" ) ) {
            if ( settings.value( listToolButtonCameraView.at(i)->objectName() ).toBool() == true ) {
                listToolButton.at(i)->click();
            }
        }
        if ( listToolButtonCameraView.at(i)->objectName().contains( "toolButton_camera_view_Ellipse" ) ) {
            if ( settings.value( listToolButtonCameraView.at(i)->objectName() ).toBool() == true ) {
                listToolButton.at(i)->click();
            }
        }
        if ( listToolButtonCameraView.at(i)->objectName().contains( "toolButton_camera_view_Circlee" ) ) {
            if ( settings.value( listToolButtonCameraView.at(i)->objectName() ).toBool() == true ) {
                listToolButtonCameraView.at(i)->click();
            }
        }
    }

    QList<QvkSpezialCheckbox *> listSpezialCheckbox = ui_mainwindow->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckbox.count(); i++ )
    {
        if ( settings.value( listSpezialCheckbox.at(i)->objectName(), false ).toBool() == true )
        {
            emit listSpezialCheckbox.at(i)->signal_clicked( true );
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
            qDebug().noquote() << global::nameOutput << listRadiobuttons.at(i)->objectName() << "=" << listRadiobuttons.at(i)->isChecked();
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
                qDebug().noquote() << global::nameOutput << listCheckBox.at(i)->objectName() << "=" << listCheckBox.at(i)->isChecked();
            else
                settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
        }
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        if ( log == true )
            qDebug().noquote() << global::nameOutput << listComboBox.at(i)->objectName() << "=" << listComboBox.at(i)->currentText();
        else
            settings.setValue( listComboBox.at(i)->objectName(), listComboBox.at(i)->currentText() );
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        if ( log == true )
            qDebug().noquote() << global::nameOutput << listSlider.at(i)->objectName() << "=" << listSlider.at(i)->value();
        else
            settings.setValue( listSlider.at(i)->objectName(), listSlider.at(i)->value() );
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ )
    {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) )
        {
            if ( log == true )
                qDebug().noquote() << global::nameOutput << listLineEdit.at(i)->objectName() << "=" << listLineEdit.at(i)->text();
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

        if ( listToolButton.at(i)->objectName().contains( "toolButton_camera_view" ) ) {
            settings.setValue( listToolButton.at(i)->objectName(), listToolButton.at(i)->isChecked() );
        }

        if ( listToolButton.at(i)->objectName().contains( "toolButton_magnifier" ) ) {
            settings.setValue( listToolButton.at(i)->objectName(), listToolButton.at(i)->isChecked() );
        }
    }

    QList<QvkSpezialCheckbox *> listSpezialCheckbox = ui_mainwindow->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckbox.count(); i++ )
    {
        settings.setValue( listSpezialCheckbox.at(i)->objectName(), listSpezialCheckbox.at(i)->isChecked() );
    }
}

QString QvkSettings::getFileName()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    return settings.fileName();
}


QString QvkSettings::getOpenh264ProfilePathWithFilename()
{
    QSettings openh264Settings( QSettings::IniFormat, QSettings::UserScope, QString( global::name + "-" + "openh264" + "-" + global::version ), global::name, Q_NULLPTR );
    QFileInfo fileInfo( openh264Settings.fileName() );
    return fileInfo.absoluteFilePath();
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
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // Qt5
    vkCameraController->vkCameraWindow->move( settings.value( "X", 0 ).toInt(), settings.value( "Y", 0 ).toInt() );
#else
    // Qt6
#endif
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
    vkSystrayAlternative->vkSystrayAlternativeWindow->move( settings.value( "X", 0 ).toInt(), settings.value( "Y", 0 ).toInt() );
    settings.endGroup();
}

#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    // Qt5
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
#else
    // Qt6
#endif

void QvkSettings::saveHaloColor( QColor value )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Halo" );
    settings.setValue( "Color", value );
    settings.endGroup();
}

void QvkSettings::readHaloColor( QvkHalo *vkHalo )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Halo" );
    QColor color = settings.value( "Color", vkHalo->colorDefault ).value<QColor>();
    vkHalo->vkHaloPreviewWidget->setColor( color );
    vkHalo->vkHaloWindow->setColor( color );
    settings.endGroup();
}

void QvkSettings::saveShowclickColor( QColor color )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "ShowClick" );
    settings.setValue( "Color", color );
    settings.endGroup();
}

void QvkSettings::readShowclickColor( QvkShowClick *vkShowClick )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "ShowClick" );
    QColor color = settings.value( "Color", vkShowClick->colorDefault ).value<QColor>();
    vkShowClick->vkPreviewWidget->setColor( color );
    settings.endGroup();
}
