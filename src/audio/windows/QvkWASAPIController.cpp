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
#include <QLineEdit>
#include <QFrame>


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
        QList<QFrame *> listFrame = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QFrame *>();
        QString index;
        if ( listFrame.empty() ) {
            index = "00";
            qDebug().noquote() << global::nameOutput << "Index in List: List is empty" << "Count befor add:" << listFrame.count() << "New index:" << index ;
        } else {
            QStringList indexStringList;
            for ( int i = 0; i < listFrame.count(); i++ ) {
                QFrame *frame = listFrame.at(i);
                indexStringList << frame->objectName().right(2);
            }

            // Max 30 Audio Geräte
            for ( int x = 0; x < 30; x++ ) {
                if ( x < 10 ) {
                    index = "0" + QString::number(x);
                } else {
                    index = QString::number(x);
                }
                if ( indexStringList.contains(index) == false ) {
                    break;
                }
            }

            qDebug().noquote() << global::nameOutput << "Index in List" << indexStringList << "Count befor add:" << listFrame.count() << "New index:" << index ;
        }

        // Neues layout für CheckBox und ProgressBar
        QHBoxLayout *layout = new QHBoxLayout; // Für Checkbox und Progressbar
        layout->setObjectName( "vBoxLayoutAudioDevice-" + index );
        layout->setSpacing(0);
        layout->setContentsMargins( 0, 0, 0, 0 ); // neu

        QCheckBox *checkBox = new QCheckBox();
        connect( checkBox, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        checkBox->setAccessibleName( string );
        checkBox->setObjectName( "checkBoxAudioDevice-" + index );
        checkBox->setToolTip( tr ( "Select one or more devices" ) );

        layout->addWidget( checkBox );

        // Ein QFrame das ein layout und die ProgressBar aufnimmt
        QFrame *frame = new QFrame;
        frame->setObjectName( "frameAudioDevice-" + index );
        frame->setLayout( layout );

        if ( type == "Playback" ) {
            checkBox->setIconSize( QSize( 16, 16 ) );
            checkBox->setIcon( QIcon( ":/pictures/screencast/speaker.png" ) );
        }
        if ( type == "Source" ) {
            checkBox->setIconSize( QSize( 16, 16 ) );
            checkBox->setIcon( QIcon( ":/pictures/screencast/microphone.png" ) );
        }
        QvkLevelMeterController *vkLevelMeterController = new QvkLevelMeterController;
        vkLevelMeterController->add_ProgressBar( checkBox, layout );
        ui->verticalLayoutAudioDevices->addWidget( frame  );
        qDebug().noquote() << global::nameOutput << "[Audio-device-added]" << name << device;
    }

    if ( action == "[Audio-device-removed]" ) {
        // Die CheckBox beinhaltet das Gerät das in der GUI entfernt werden soll.
        // Und jede Checkbox, BoxLayout, Frame und ProgressBar wurde ein gleicher eindeutiger Wert<index> an den Objectnamen hinzugefügt.
        // Beispiel  ....-00, ...-01, ...-02, ...-03 usw.
        QString index;
        QString device = string.section( ":::", 0, 0 );
        QList<QCheckBox *> listQCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
        for ( int i = 0; i < listQCheckBox.count(); i++ ) {
            QCheckBox *checkBox = listQCheckBox.at(i);
            if ( checkBox->accessibleName().section( ":::", 0, 0 ) == device ) {
                index = checkBox->objectName().right(2);
                break;
            }
        }

        QList<QvkLevelMeterController *> listProgressBar = ui->scrollAreaAudioDevice->findChildren<QvkLevelMeterController *>();
        for ( int i = 0; i < listProgressBar.count(); i++ ) {
            QvkLevelMeterController *vkLevelMeterController = listProgressBar.at(i);
            if ( vkLevelMeterController->objectName().right(2) == index ) {
                qDebug().noquote() << global::nameOutput << "[Audio-device-removed]" << name << device;
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
    }
}
