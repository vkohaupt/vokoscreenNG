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
#include <QCheckBox>
#include <QList>

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

#ifdef Q_OS_WIN
    // Extra Verzeichnis für openh264 erstellen
    QSettings openh264Settings( QSettings::IniFormat, QSettings::UserScope, QString( global::name + "-" + "openh264" + "-" + global::version ), global::name, Q_NULLPTR );
    openh264Settings.beginGroup( global::name );
    openh264Settings.setValue( "Version", global::version );
    openh264Settings.endGroup();
#endif
}

QvkSettings::~QvkSettings(){}

void QvkSettings::readAll( Ui_formMainWindow *ui_mainwindow, QMainWindow *parent )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );

    // Clear all settings if checkBoxResetAtNextStart is set
    QList<QCheckBox *> listCheckBoxReset = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBoxReset.count(); i++ ) {
        QCheckBox *checkBox = listCheckBoxReset.at(i);
        if ( ( checkBox->objectName() == "checkBoxResetAtNextStart" ) and
             ( settings.value( checkBox->objectName(), false ).toBool() == true ) )
        {
            settings.clear();
        }
    }

    // We want block the signals for shortcut ComboBox signals and set the default value
    QList<QComboBox *> listComboBoxShortcut = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBoxShortcut.count(); i++ )
    {
        QComboBox *comboBox = listComboBoxShortcut.at(i);
        if ( comboBox->objectName().contains( "comboBox_shortcut", Qt::CaseInsensitive ) == true )
        {
            comboBox->blockSignals( true );

            if ( comboBox->objectName() == "comboBox_shortcut_start" ) {
                QString valueText = settings.value( comboBox->objectName(), "F10" ).toString();
                comboBox->setCurrentText( valueText );
            }
            if ( comboBox->objectName() == "comboBox_shortcut_pause" ) {
                QString valueText = settings.value( comboBox->objectName(), "F12" ).toString();
                comboBox->setCurrentText( valueText );
            }
            if ( comboBox->objectName() == "comboBox_shortcut_magnification" ) {
                QString valueText = settings.value( comboBox->objectName(), "F9" ).toString();
                comboBox->setCurrentText( valueText );
            }
            if ( comboBox->objectName() == "comboBox_shortcut_camera" ) {
                QString valueText = settings.value( comboBox->objectName(), "F8" ).toString();
                comboBox->setCurrentText( valueText );
            }
            if ( comboBox->objectName() == "comboBox_shortcut_showclick" ) {
                QString valueText = settings.value( comboBox->objectName(), "F7" ).toString();
                comboBox->setCurrentText( valueText );
            }
            if ( comboBox->objectName() == "comboBox_shortcut_halo" ) {
                QString valueText = settings.value( comboBox->objectName(), "F6" ).toString();
                comboBox->setCurrentText( valueText );
            }
            if ( comboBox->objectName() == "comboBox_shortcut_snapshot" ) {
                QString valueText = settings.value( comboBox->objectName(), "F5" ).toString();
                comboBox->setCurrentText( valueText );
            }
        }
    }

    // Then we set the values for all ComboBoxes
    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        QComboBox *comboBox = listComboBox.at(i);
        // Language
        if ( comboBox->objectName() == "comboBoxLanguage" ) {
            // listComboBox.at(i)->removeItem( 20 ); // Nur zum testen. Index muß zuvor ermittelt werden.
            int index;

            // Wird eine Sprache in den Settings gefunden, wird diese wenn in der ComboBox vorhanden gesetzt
            QString valueText = settings.value( comboBox->objectName(), "" ).toString();
            index = comboBox->findText( valueText );
            if ( index > -1 ) {
                comboBox->setCurrentIndex( index );
                continue;
            }

            // Systemsprache ist in der Combobox nicht vorhanden, es wird "en" gesetzt
            index = comboBox->findData( QLocale::system().name() );
            if ( index == -1 ) {
                int value = comboBox->findData( "en" );
                comboBox->setCurrentIndex( value );
                continue;
            }

            // Systemsprache wird in der Combobox gesetzt wenn diese in der Combobox gefunden wird.
            index = comboBox->findData( QLocale::system().name() );
            if ( index > -1 ) {
                comboBox->setCurrentIndex( index );
            }

            continue;
        }

        QString valueText = settings.value( comboBox->objectName(), "" ).toString();
        int valueInt = comboBox->findText( valueText );
        if ( valueInt > -1 ) {
            comboBox->setCurrentIndex( valueInt );
        }
    }

    // After set all comboBox values we set the shortcut ComboBox signals
    for ( int i = 0; i < listComboBoxShortcut.count(); i++ ) {
        QComboBox *comboBox = listComboBoxShortcut.at(i);
        if ( comboBox->objectName().contains( "comboBox_shortcut", Qt::CaseInsensitive ) == true ) {
            comboBox->blockSignals( false );
        }
    }

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        QRadioButton *radioButton = listRadiobuttons.at(i);

        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( radioButton->objectName() == "radioButtonScreencastFullscreen" ) and
             ( settings.value( radioButton->objectName(), true ).toBool() == true ) )
        {
            radioButton->click();
            continue;
        }

        // We set WASAPI as default
        if ( ( radioButton->objectName() == "radioButtonWASAPI" ) and
             ( settings.value( radioButton->objectName(), false ).toBool() == false ) )
        {
            radioButton->click();
            continue;
        }

        if ( ( radioButton->objectName() == "radioButton_cisco_on" ) and
             ( settings.value( radioButton->objectName(), true ).toBool() == true ) )
        {
            radioButton->click();
            continue;
        }

        bool value = settings.value( radioButton->objectName(), false ).toBool();
        if ( value == true )
        {
            radioButton->click();
        }
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        QCheckBox *checkBox = listCheckBox.at(i);

        // We have no settings-file(first start after install) but this object we want set as default.
        if ( ( checkBox->objectName() == "checkBoxShowInSystray" ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) )
        {
            checkBox->click();
            continue;
        }
        if ( ( checkBox->objectName() == "checkBoxShowInSystray" ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == false ) )
        {
            checkBox->click();
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName() == "checkBoxStartsMinimized" ) and
             ( settings.value( checkBox->objectName(), false ).toBool() == true ) )
        {
            parent->showMinimized();
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName().contains( "checkBoxLookForUpdates" ) ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) )
        {
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName().contains( "checkBoxShowInSystrayAlternative" ) ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) )
        {
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName().contains( "checkboxAudioDevice-" ) ) and
             ( settings.value( checkBox->objectName(), false ).toBool() == true ) )
        {
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName().contains( "checkBoxSnapshotShowBallonInSystray" ) ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) )
        {
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName().contains( "checkBoxGstreamerDebugLevel" ) ) and
             ( settings.value( checkBox->objectName(), false ).toBool() == true ) )
        {
            continue;
        }

        //
        if ( checkBox->objectName().contains( "checkBox_shortcut" ) and
           ( settings.value( checkBox->objectName(), false ).toBool() == true ) )
        {
            checkBox->setChecked( true );
            continue;
        }

        // Start/Stop
        if ( checkBox->objectName() == "checkBox_shortcut_start_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_start_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }

        // Pause/Continue
        if ( checkBox->objectName() == "checkBox_shortcut_pause_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_pause_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }

        // Magnification
        if ( checkBox->objectName() == "checkBox_shortcut_magnification_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_magnification_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }

        // Camera
        if ( checkBox->objectName() == "checkBox_shortcut_camera_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_camera_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }

        // Showclick
        if ( checkBox->objectName() == "checkBox_shortcut_showclick_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_showclick_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }

        // Halo
        if ( checkBox->objectName() == "checkBox_shortcut_halo_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_halo_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }

        // Snapshot
        if ( checkBox->objectName() == "checkBox_shortcut_snapshot_strg" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }
        if ( checkBox->objectName() == "checkBox_shortcut_snapshot_shift" ) {
             if ( settings.value( checkBox->objectName(), true ).toBool() == true ) {
                 checkBox->setChecked( true );
                 continue;
             }
        }


        if ( ( checkBox->objectName().contains( "checkBoxSnapshotHideBeforeRecording" ) ) and
             ( settings.value( checkBox->objectName(), true ).toBool() == true ) ) {
            checkBox->click();
            continue;
        }

        if ( ( checkBox->objectName().contains( "checkBoxGifLoop" ) ) and
            ( settings.value( checkBox->objectName(), true ).toBool() == true ) ) {
            checkBox->click();
            continue;
        }

        // We found a setting, then we want set or not.
        if ( settings.value( checkBox->objectName(), false ).toBool() == true )
        {
            checkBox->click();
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        QSlider *slider = listSlider.at(i);

        if ( slider->objectName() == "sliderVideo" ) {
            continue;
        }

        if ( slider->objectName() == "sliderGstDebugLevel" ) {
            continue;
        }

        QVariant variant = settings.value( slider->objectName() );
        if ( variant.isValid() ) {
            slider->setValue( variant.toInt() );
        }
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ ) {
        QLineEdit *lineEdit = listLineEdit.at(i);
        if ( lineEdit->objectName().contains( "lineEdit" ) ) {
            QString value = settings.value( lineEdit->objectName() ).toString();
            if ( !value.isEmpty() ) {
                lineEdit->setText( value );
            }
        }
    }

    // All TabWidget start with value=0, also if in Qt-Creator is the value an other
    QList<QTabWidget *> listTabWidget = ui_mainwindow->centralWidget->findChildren<QTabWidget *>();
    for ( int i = 0; i < listTabWidget.count(); i++ ) {
        QTabWidget *tabWidget = listTabWidget.at(i);
        tabWidget->setCurrentIndex( 0 );

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
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        QToolButton *toolButton = listToolButton.at(i);

        if ( toolButton->objectName().contains( "toolButtonMute" ) ) {
            QString value = settings.value( toolButton->objectName() ).toString();
            if ( value == "audio-volume-muted") {
                toolButton->click();
            }
        }

        if ( toolButton->objectName().contains( "toolButton_camera_view" ) ) {
            if ( settings.value( toolButton->objectName() ).toBool() == true ) {
                toolButton->click();
            }
        }

        if ( toolButton->objectName().contains( "toolButton_magnifier" ) ) {
            if ( settings.value( toolButton->objectName() ).toBool() == true ) {
                toolButton->click();
            }
        }
    }

    QList<QToolButton *> listToolButtonCameraView = ui_mainwindow->centralWidget->findChildren<QToolButton *>( "toolButton_camera_view_" );
    for ( int i = 0; i < listToolButtonCameraView.count(); i++ ) {
        QToolButton *toolButton = listToolButtonCameraView.at(i);
        if ( toolButton->objectName().contains( "toolButton_camera_view_Rectangle" ) ) {
            if ( settings.value( toolButton->objectName() ).toBool() == true ) {
                toolButton->click();
            }
        }
        if ( toolButton->objectName().contains( "toolButton_camera_view_Ellipse" ) ) {
            if ( settings.value( toolButton->objectName() ).toBool() == true ) {
                toolButton->click();
            }
        }
        if ( toolButton->objectName().contains( "toolButton_camera_view_Circle" ) ) {
            if ( settings.value( toolButton->objectName() ).toBool() == true ) {
                toolButton->click();
            }
        }
    }

    QList<QvkSpezialCheckbox *> listSpezialCheckbox = ui_mainwindow->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckbox.count(); i++ ) {
        QvkSpezialCheckbox *vkSpezialCheckbox = listSpezialCheckbox.at(i);
        if ( settings.value( vkSpezialCheckbox->objectName(), false ).toBool() == true ) {
            emit vkSpezialCheckbox->signal_clicked( true );
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
    if ( log == true ) {
        // do nothing
    } else {
        settings.clear();
    }

    settings.beginGroup( global::name );
    if ( log == true ) {
        // do nothing
    } else {
        settings.setValue( "Version", global::version );
    }
    settings.endGroup();

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ ) {
        if ( log == true ) {
            qDebug().noquote() << global::nameOutput << listRadiobuttons.at(i)->objectName() << "=" << listRadiobuttons.at(i)->isChecked();
        } else {
            settings.setValue( listRadiobuttons.at(i)->objectName(), listRadiobuttons.at(i)->isChecked() );
        }
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxStopRecordingAfter" ) or
             ( listCheckBox.at(i)->objectName() == "checkBoxStartTime" ) )
        {
            // We do not save
        } else {
            if ( log == true ) {
                qDebug().noquote() << global::nameOutput << listCheckBox.at(i)->objectName() << "=" << listCheckBox.at(i)->isChecked();
            } else {
                settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
            }
        }
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ ) {
        if ( log == true ) {
            qDebug().noquote() << global::nameOutput << listComboBox.at(i)->objectName() << "=" << listComboBox.at(i)->currentText();
        } else {
            settings.setValue( listComboBox.at(i)->objectName(), listComboBox.at(i)->currentText() );
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ ) {
        if ( log == true ) {
            qDebug().noquote() << global::nameOutput << listSlider.at(i)->objectName() << "=" << listSlider.at(i)->value();
        } else {
            settings.setValue( listSlider.at(i)->objectName(), listSlider.at(i)->value() );
        }
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ ) {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) ) {
            if ( log == true ) {
                qDebug().noquote() << global::nameOutput << listLineEdit.at(i)->objectName() << "=" << listLineEdit.at(i)->text();
            } else {
                settings.setValue( listLineEdit.at(i)->objectName(), listLineEdit.at(i)->text() );
            }
        }
    }

    QList<QToolButton *> listToolButton = ui_mainwindow->centralWidget->findChildren<QToolButton *>();
    for ( int i = 0; i < listToolButton.count(); i++ ) {
        if ( listToolButton.at(i)->objectName().contains( "toolButtonMute" ) ) {
            if ( listToolButton.at(i)->isChecked() == true ) {
                settings.setValue( listToolButton.at(i)->objectName(), "audio-volume-muted" );
            } else {
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
    for ( int i = 0; i < listSpezialCheckbox.count(); i++ ) {
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

void QvkSettings::saveCamera( int index, int x, int y )
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Camera-" + QString::number( index ) );
    settings.setValue( "X", x );
    settings.setValue( "Y", y );
    settings.endGroup();
}

void QvkSettings::readCamera( QList<QvkCameraSingle *> vkCameraSingle)
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    for ( int index = 0; index < vkCameraSingle.count(); index++ ) {
        settings.beginGroup( "Camera-" + QString::number( index ) );
           vkCameraSingle.at( index )->vkCameraWindow->move( settings.value( "X", 0 ).toInt(), settings.value( "Y", 0 ).toInt()  );
        settings.endGroup();
    }
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
    vkSystrayAlternative->vkSystrayAlternativeWindow->move( settings.value( "X", 30 ).toInt(), settings.value( "Y", 30 ).toInt() );
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


void QvkSettings::readPlayerPathOpenFile( int value, QvkPlayerController *vkplayer )
{
    Q_UNUSED(value)
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, global::name, global::name, Q_NULLPTR );
    settings.beginGroup( "Player" );
    vkplayer->pathOpenFile = settings.value( "Path", "" ).toString();
    settings.endGroup();
}

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
