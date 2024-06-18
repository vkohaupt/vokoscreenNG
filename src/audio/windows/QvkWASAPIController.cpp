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

#include "QvkWASAPIController.h"
#include "global.h"
#include "QvkLevelMeterController.h"

#include <QDebug>
#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>
#include <QHBoxLayout>
#include <QIcon>
#include <QSize>
#include <QList>
#include <QAudioDevice>
#include <QMediaDevices>

QvkWASAPIController::QvkWASAPIController( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    global::listChildren = new QList<QLineEdit*>();

    ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    global::lineEditWASAPIWatcher = new QLineEdit;
    connect( this, SIGNAL( signal_haveAudioDeviceSelected(bool) ), ui->labelAudioCodec,    SLOT( setEnabled(bool) ) );
    connect( this, SIGNAL( signal_haveAudioDeviceSelected(bool) ), ui->comboBoxAudioCodec, SLOT( setEnabled(bool) ) );
    connect( global::lineEditWASAPIWatcher, SIGNAL( textChanged(QString) ), this, SLOT( slot_pluggedInOutDevice(QString) ) );
    vkWASAPIWatcher = new QvkWASAPIWatcher( ui );
    slot_audioDeviceSelected();
}


QvkWASAPIController::~QvkWASAPIController()
{
}


void QvkWASAPIController::slot_audioDeviceSelected()
{
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listCheckBox.at(i);
        if ( checkBox->checkState() == Qt::Checked ) {
            value = true;
            break;
        }
    }
    emit signal_haveAudioDeviceSelected( value );
}


void QvkWASAPIController::slot_pluggedInOutDevice( QString string )
{
    QList<QLabel *> listLabel = ui->scrollAreaAudioDevice->findChildren<QLabel *>();
    for ( int i = 0; i < listLabel.count(); i++ ) {
        ui->verticalLayoutAudioDevices->removeWidget( listLabel.at(i) );
        delete listLabel.at(i);
    }

    QString device = string.section( ":::", 0, 0 ); // DeviceID
    QString name   = string.section( ":::", 1, 1 ); // Name
    QString type   = string.section( ":::", 2, 2 ); // Microphone or speaker
    QString api    = string.section( ":::", 3, 3 ); // WASAPI2
    QString action = string.section( ":::", 4, 4 ); // Action: Added or removed
    Q_UNUSED(api)

    if ( device == "" ) {
        qDebug().noquote() << global::nameOutput << "[Audio] Device is empty -> return";
        return;
    }

    if ( action == "[Audio-device-added]" ) {

        // Freier Index(xx) 00, 01, 02, xx, 04, 05 usw. ermitteln und diesen Index dem neuen Layout, CheckBox und ProgressBar hinzufügen
        QList<QHBoxLayout *> listVBoxLayout = ui->verticalLayoutAudioDevices->findChildren<QHBoxLayout *>();
        QString indexNumber;
        if ( listVBoxLayout.empty() ) {
            indexNumber = "00";
            qDebug().noquote() << global::nameOutput << "Index in List: List is empty" << "Count befor add:" << listVBoxLayout.count() << "New index:" << indexNumber ;
        } else {
            QStringList indexStringList;
            for ( int i = 0; i < listVBoxLayout.count(); i++ ) {
                QHBoxLayout *vBoxLayout = listVBoxLayout.at(i);
                indexStringList << vBoxLayout->objectName().right(2);
            }
            // Max 30 Audio Geräte
            for ( int x = 1; x < 30; x++ ) {
                if ( x < 10 ) {
                    indexNumber = "0" + QString::number(x);
                } else {
                    indexNumber = QString::number(x);
                }
                if ( indexStringList.contains(indexNumber) == false ) {
                    break;
                }
            }
            qDebug().noquote() << global::nameOutput << "Index in List" << indexStringList << "Count befor add:" << listVBoxLayout.count() << "New index:" << indexNumber ;
        }

        // Neues layout für CheckBox und ProgressBar
        QHBoxLayout *vBoxLayout = new QHBoxLayout; // Für Checkbox und Progressbar
        vBoxLayout->setObjectName( "vBoxLayoutAudioDevice-" + indexNumber );
        vBoxLayout->setSpacing(0);

        QCheckBox *checkBox = new QCheckBox();
        connect( checkBox, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        checkBox->setText( name.left(50) );
        checkBox->setAccessibleName( string );
        checkBox->setObjectName( "checkBoxAudioDevice-" + indexNumber );
        checkBox->setToolTip( tr ( "Select one or more devices" ) );

        vBoxLayout->addWidget( checkBox );

        if ( type == "Playback" ) {
            checkBox->setIconSize( QSize( 16, 16 ) );
            checkBox->setIcon( QIcon( ":/pictures/screencast/speaker.png" ) );
        }
        if ( type == "Source" ) {
            checkBox->setIconSize( QSize( 16, 16 ) );
            checkBox->setIcon( QIcon( ":/pictures/screencast/microphone.png" ) );
        }
        QvkLevelMeterController *vkLevelMeterController = new QvkLevelMeterController;
        vkLevelMeterController->add_ProgressBar( checkBox, vBoxLayout );
        ui->verticalLayoutAudioDevices->addLayout( vBoxLayout );
        qDebug().noquote() << global::nameOutput << "[Audio-device-added]" << name << device;
    }

    if ( action == "[Audio-device-removed]" ) {
        QString indexNumber = get_number_removed_device( string );
        QHBoxLayout *vBoxLayout = get_removed_vBoxLayout( indexNumber );
        QProgressBar *progressBar = get_removed_ProgressBar( indexNumber );
        QCheckBox *checkBox = get_removed_checkBox( indexNumber );

        QList<QvkLevelMeterController *> listProgressBar = ui->scrollAreaAudioDevice->findChildren<QvkLevelMeterController *>();
        for ( int i = 0; i < listProgressBar.count(); i++ ) {
            QvkLevelMeterController *vkLevelMeterController = listProgressBar.at(i);
            if ( vkLevelMeterController->objectName().right(2) == indexNumber ) {
                qDebug().noquote() << global::nameOutput << "[Audio-device-removed]" << name << device;
                vkLevelMeterController->remove_ProgressBar( checkBox );
                vkLevelMeterController->deleteLater();
            }
        }

        delete checkBox;
        ui->verticalLayoutAudioDevices->removeItem( vBoxLayout );
        vBoxLayout->deleteLater();
    }
}


// Die CheckBox beinhaltet das Gerät das in der GUI entfernt werden soll.
// Und jede Checkbox, BoxLayout und ProgressBar wurde ein gleicher eindeutiger Wert<indexNumber> an den Objectnamen hinzugefügt.
// Beispiel  ....-00, ...-01, ...-02, ...-03 usw.
QString QvkWASAPIController::get_number_removed_device( QString string )
{
    QString indexNumber = "";
    QString device = string.section( ":::", 0, 0 );
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        if ( checkBox->accessibleName().section( ":::", 0, 0 ) == device ) {
            indexNumber = checkBox->objectName().right(2);
        }
    }
    return indexNumber;
}


QCheckBox *QvkWASAPIController::get_removed_checkBox( QString indexNumber )
{
    QCheckBox *returnCheckBox = NULL;
    QList<QCheckBox *> list = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < list.count(); i++  ) {
        QCheckBox *checkBox = list.at(i);
        if ( checkBox->objectName().contains( "checkBoxAudioDevice-" + indexNumber ) ) {
            returnCheckBox = checkBox;
        }
    }
    return returnCheckBox;
}


QHBoxLayout *QvkWASAPIController::get_removed_vBoxLayout( QString indexNumber )
{
    QHBoxLayout *returnVBoxLayout = NULL;
    QList<QHBoxLayout *> list = ui->scrollAreaAudioDevice->findChildren<QHBoxLayout *>();
    for ( int i = 0; i < list.count(); i++  ) {
        QHBoxLayout *vBoxLayout = list.at(i);
        if ( vBoxLayout->objectName().contains( "vBoxLayoutAudioDevice-" + indexNumber ) ) {
            returnVBoxLayout = vBoxLayout;
        }
    }
    return returnVBoxLayout;
}


QProgressBar *QvkWASAPIController::get_removed_ProgressBar( QString indexNumber )
{
    QProgressBar *returnProgressBar = NULL;
    QList<QProgressBar *> list = ui->scrollAreaAudioDevice->findChildren<QProgressBar *>();
    for ( int i = 0; i < list.count(); i++  ) {
        QProgressBar *progressBar = list.at(i);
        if ( progressBar->objectName().contains( "progressBarAudioDevice-" + indexNumber ) ) {
            returnProgressBar = progressBar;
        }
    }
    return returnProgressBar;
}

