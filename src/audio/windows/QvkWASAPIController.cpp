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
        QList<QFrame *> listVBoxLayout = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QFrame *>();
        QString index;
        if ( listVBoxLayout.empty() ) {
            index = "00";
            qDebug().noquote() << global::nameOutput << "Index in List: List is empty" << "Count befor add:" << listVBoxLayout.count() << "New index:" << index ;
        } else {
            QStringList indexStringList;
            for ( int i = 0; i < listVBoxLayout.count(); i++ ) {
                QFrame *vBoxLayout = listVBoxLayout.at(i);
                indexStringList << vBoxLayout->objectName().right(2);
            }
            // Max 30 Audio Geräte
            for ( int x = 1; x < 30; x++ ) {
                if ( x < 10 ) {
                    index = "0" + QString::number(x);
                } else {
                    index = QString::number(x);
                }
                if ( indexStringList.contains(index) == false ) {
                    break;
                }
            }
            qDebug().noquote() << global::nameOutput << "Index in List" << indexStringList << "Count befor add:" << listVBoxLayout.count() << "New index:" << index ;
        }

        // Neues layout für CheckBox und ProgressBar
        QHBoxLayout *vBoxLayout = new QHBoxLayout; // Für Checkbox und Progressbar
        vBoxLayout->setObjectName( "vBoxLayoutAudioDevice-" + index );
        vBoxLayout->setSpacing(0);
        vBoxLayout->setContentsMargins( 0, 0, 0, 0 ); // neu

        QCheckBox *checkBox = new QCheckBox();
        connect( checkBox, SIGNAL( clicked( bool ) ), this, SLOT( slot_audioDeviceSelected() ) );
        checkBox->setAccessibleName( string );
        checkBox->setObjectName( "checkBoxAudioDevice-" + index );
        checkBox->setToolTip( tr ( "Select one or more devices" ) );

        vBoxLayout->addWidget( checkBox );

        // Ein QFrame das ein layout und die ProgressBar aufnimmt
        QFrame *frame = new QFrame;
        frame->setObjectName( "frameAudioDevice-" + index );
        frame->setLayout( vBoxLayout );

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
        ui->verticalLayoutAudioDevices->addWidget( frame  );
        qDebug().noquote() << global::nameOutput << "[Audio-device-added]" << name << device;
    }

    if ( action == "[Audio-device-removed]" ) {
        QString index = get_index_removed_device( string );
        QHBoxLayout *vBoxLayout = get_removed_vBoxLayout( index );
        QvkSpezialProgressBarAudio *progressBar = get_removed_ProgressBar( index );
        QCheckBox *checkBox = get_removed_checkBox( index );

        QList<QvkLevelMeterController *> listProgressBar = ui->scrollAreaAudioDevice->findChildren<QvkLevelMeterController *>();
        for ( int i = 0; i < listProgressBar.count(); i++ ) {
            QvkLevelMeterController *vkLevelMeterController = listProgressBar.at(i);
            if ( vkLevelMeterController->objectName().right(2) == index ) {
                qDebug().noquote() << global::nameOutput << "[Audio-device-removed]" << name << device;
                vkLevelMeterController->vkLevelMeter->stop();
                vkLevelMeterController->remove_LineEdit( checkBox ); // Statt checkbox index verwenden
                vkLevelMeterController->deleteLater();
                break;
            }
        }


//        delete checkBox;
//        ui->verticalLayoutAudioDevices->removeItem( vBoxLayout );
//        vBoxLayout->deleteLater();
    }
}


// Die CheckBox beinhaltet das Gerät das in der GUI entfernt werden soll.
// Und jede Checkbox, BoxLayout und ProgressBar wurde ein gleicher eindeutiger Wert<index> an den Objectnamen hinzugefügt.
// Beispiel  ....-00, ...-01, ...-02, ...-03 usw.
QString QvkWASAPIController::get_index_removed_device( QString string )
{
    QString index = "";
    QString device = string.section( ":::", 0, 0 );
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listQCheckBox.at(i);
        if ( checkBox->accessibleName().section( ":::", 0, 0 ) == device ) {
            index = checkBox->objectName().right(2);
            break;
        }
    }
    return index;
}


QCheckBox *QvkWASAPIController::get_removed_checkBox( QString index )
{
    QCheckBox *returnCheckBox = NULL;
    QList<QCheckBox *> list = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < list.count(); i++  ) {
        QCheckBox *checkBox = list.at(i);
        if ( checkBox->objectName().contains( "checkBoxAudioDevice-" + index ) ) {
            qDebug().noquote() << global::nameOutput << "Removed widget:" << checkBox->objectName();
            returnCheckBox = checkBox;
            break;
        }
    }
    return returnCheckBox;
}


QHBoxLayout *QvkWASAPIController::get_removed_vBoxLayout( QString index )
{
    QHBoxLayout *returnVBoxLayout = NULL;
    QList<QHBoxLayout *> list = ui->scrollAreaAudioDevice->findChildren<QHBoxLayout *>();
    for ( int i = 0; i < list.count(); i++  ) {
        QHBoxLayout *vBoxLayout = list.at(i);
        if ( vBoxLayout->objectName().contains( "vBoxLayoutAudioDevice-" + index ) ) {
            qDebug().noquote() << global::nameOutput << "Removed widget:" << vBoxLayout->objectName();
            returnVBoxLayout = vBoxLayout;
            break;
        }
    }
    return returnVBoxLayout;
}


QvkSpezialProgressBarAudio *QvkWASAPIController::get_removed_ProgressBar( QString index )
{
    QvkSpezialProgressBarAudio *returnProgressBar = NULL;
    QList<QvkSpezialProgressBarAudio *> list = ui->scrollAreaAudioDevice->findChildren<QvkSpezialProgressBarAudio *>();
    for ( int i = 0; i < list.count(); i++  ) {
        QvkSpezialProgressBarAudio *progressBar = list.at(i);
        if ( progressBar->objectName().contains( "progressBarAudioDevice-" + index ) ) {
            qDebug().noquote() << global::nameOutput << "Removed widget:" << progressBar->objectName();
            returnProgressBar = progressBar;
            break;
        }
    }
    return returnProgressBar;
}

