/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2026 Volker Kohaupt
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

#include "QvkAudioPipewireController_wl.h"
#include "global.h"
//#include "QvkLevelMeterController.h"

#include <QDebug>
#include <QPainter>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>
#include <QHBoxLayout>
#include <QIcon>
#include <QSize>
#include <QList>
#include <QLineEdit>
#include <QFrame>

QvkAudioPipewireController_wl::QvkAudioPipewireController_wl(Ui_formMainWindow_wl *ui_mainwindow )
{
    ui = ui_mainwindow;

    global::listChildren = new QList<QLineEdit*>();

    ui->verticalLayoutAudioDevices->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    global::lineEditPipewireWatcher = new QLineEdit;
    connect(this,
            &QvkAudioPipewireController_wl::signal_haveAudioDeviceSelected,
            ui->labelAudioCodec,
            [=](bool value){
        ui->labelAudioCodec->setEnabled(value);
        ui->comboBoxAudioCodec->setEnabled(value);
        ui->checkBoxSeparatedAudioTracks->setEnabled(value);
    });
    connect(global::lineEditPipewireWatcher,
            &QLineEdit::textChanged,
            this,
            [=](QString value){
        slot_pluggedInOutDevice(value);
    });
    vkAudioPipewireWatcher_wl = new QvkAudioPipewireWatcher_wl( ui );
    slot_audioDeviceSelected();
}


QvkAudioPipewireController_wl::~QvkAudioPipewireController_wl()
{
}


void QvkAudioPipewireController_wl::slot_audioDeviceSelected()
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


void QvkAudioPipewireController_wl::slot_pluggedInOutDevice( QString string )
{
    QString deviceID    = string.section( ":::", 0, 0 ); // DeviceID
    QString description = string.section( ":::", 1, 1 ); // Beschreibung
    QString type        = string.section( ":::", 2, 2 ); // Microphone or speaker
    QString api         = string.section( ":::", 3, 3 ); // alsa
    QString action      = string.section( ":::", 4, 4 ); // Action: Added or removed
    QString device      = string.section( ":::", 5, 5 ); // DeviceName
    Q_UNUSED(api)

    if ( deviceID == "" ) {
        qDebug().noquote() << global::nameOutput << "[Audio] DeviceID is empty -> return";
        return;
    }

    if ( action == "[Audio-device-added]" ) {
        // Neues layout für CheckBox und ProgressBar
        QHBoxLayout *layout = new QHBoxLayout; // Für Checkbox und Progressbar
        layout->setObjectName( "vBoxLayoutAudioDevice-" + device );
        layout->setSpacing(0);
        layout->setContentsMargins( 0, 0, 0, 0 );

        QCheckBox *checkBox = new QCheckBox();
        connect(checkBox, &QCheckBox::clicked, this, [=](){slot_audioDeviceSelected();});
        checkBox->setAccessibleName( string );
        checkBox->setObjectName( "checkBoxAudioDevice-" + device );
        checkBox->setToolTip( tr ( "Select one or more devices" ) );
        checkBox->setText(description);
        checkBox->setToolTip(device);

        layout->addWidget( checkBox );

        if ( type == "Playback" ) {
            checkBox->setIconSize( QSize( 16, 16 ) );
            checkBox->setIcon( QIcon( ":/pictures/screencast/speaker.png" ) );
        }
        if ( type == "Source" ) {
            checkBox->setIconSize( QSize( 16, 16 ) );
            checkBox->setIcon( QIcon( ":/pictures/screencast/microphone.png" ) );
        }
//        QvkLevelMeterController *vkLevelMeterController = new QvkLevelMeterController;
        //vkLevelMeterController->add_ProgressBar( checkBox, layout );
//        ui->verticalLayoutAudioDevices->addWidget( frame  );
        ui->verticalLayoutAudioDevices->addWidget( checkBox ); // neu
        qDebug().noquote() << global::nameOutput << "[Audio-device-added]" << description << device;

        emit signal_newAudioDevice(checkBox);
    }

    if ( action == "[Audio-device-removed]" ) {
        // Die CheckBox beinhaltet das Gerät das in der GUI entfernt werden soll.
        // Und jede Checkbox, BoxLayout, Frame und ProgressBar wurde ein gleicher eindeutiger Wert<index> an den Objectnamen hinzugefügt.
        // Beispiel  ....-00, ...-01, ...-02, ...-03 usw.
        QString index;
        QString deviceID = string.section( ":::", 0, 0 );
        QList<QCheckBox *> listQCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        for ( int i = 0; i < listQCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listQCheckBox.at(i);
            if ( checkBox->accessibleName().section( ":::", 0, 0 ) == deviceID ) {
                index = checkBox->objectName().right(2);
                break;
            }
        }

        qDebug().noquote() << global::nameOutput << "[Audio-device-removed]" << description << device;

/*
        QList<QvkLevelMeterController *> listProgressBar = ui->scrollAreaAudioDevice->findChildren<QvkLevelMeterController *>();
        for ( int i = 0; i < listProgressBar.count(); i++ ) {
            QvkLevelMeterController *vkLevelMeterController = listProgressBar.at(i);
            if ( vkLevelMeterController->objectName().right(2) == index ) {
                qDebug().noquote() << global::nameOutput << "[Audio-device-removed]" << description << device;
                vkLevelMeterController->vkLevelMeter->stop();
                vkLevelMeterController->remove_LineEdit( index );
                vkLevelMeterController->deleteLater();
                break;
            }
        }

        QList<QFrame *> listFrame = ui->scrollAreaAudioDevice->findChildren<QFrame *>();
        for ( int i = 0; i < listProgressBar.count(); i++ ) {
            QFrame *frame = listFrame.at(i);
            if ( frame->objectName().right(2) == index ) {
                frame->deleteLater();
                break;
            }
        }
*/
    }
}
