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

#include "QvkInformation_wl.h"
#include "global.h"
#include "QvkShowMessage.h"

#include <QTimer>
#include <QTime>
#include <QStorageInfo>
#include <QDebug>

QvkInformation_wl::QvkInformation_wl( QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *ui_mainwindow )
{
    mainWindow = vkMainWindow;
    ui = ui_mainwindow;
//    sliderCountDown = slider_count_down;
//    sliderSecondWaitBeforeRecording = slider_Second_Wait_Before_Recording;

//    connect( mainWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( slot_cleanup() ) );

    ui->labelVideoSize->setText("");
    ui->labelFreeSize->setText("");
    ui->labelInfoRecordTime->setText("00:00:00");

    // VideoSize and FreeDiskSpace
    timerStorageInfo = new QTimer(this);
    timerStorageInfo->setTimerType( Qt::PreciseTimer );
    timerStorageInfo->setInterval( 1000 );
    connect( timerStorageInfo, SIGNAL( timeout() ), this, SLOT( slot_StorageInfo() ) );
    timerStorageInfo->start();
/*
    // Recorded time
    elapsedTime = new QElapsedTimer();
    connect( ui->pushButtonStart,    SIGNAL( clicked( bool ) ), this, SLOT( slot_timeFirstStart() ) );
    connect( ui->pushButtonPause,    SIGNAL( clicked( bool ) ), this, SLOT( slot_summedTimeAfterPause() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this, SLOT( slot_timeContinueStart() ) );

    timerRecord = new QTimer(this);
    timerRecord->setTimerType( Qt::PreciseTimer );
    timerRecord->setInterval( 1000 );
    connect( ui->pushButtonStart,    SIGNAL( clicked( bool ) ), timerRecord, SLOT( start() ) );
    connect( timerRecord,            SIGNAL( timeout() ),       this,        SLOT( slot_displayRecordTime() ) );
    connect( ui->pushButtonStop,     SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );
    connect( ui->pushButtonPause,    SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), timerRecord, SLOT( start() ) );

    // Update
    ui->label_Upate_tab_1->clear();
    ui->label_Upate_tab_4->clear();
    connect( &version, SIGNAL( signal_newVersionAvailable( QString ) ), this, SLOT( slot_newVersionAvailable( QString ) ) );
    connect( ui->checkBoxLookForUpdates, SIGNAL( toggled( bool ) ), &version, SLOT( slot_doDownload( bool ) ) );
*/
    // Frames, Format, Codecs
    connect( ui->comboBoxFormat,       SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_Format( QString ) ) );
    connect( ui->comboBoxVideoCodec,   SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_Videocodec( QString ) ) );
    connect( ui->comboBoxAudioCodec,   SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_Audiocodec( QString ) ) );
    connect( mainWindow->sliderFrames, SIGNAL( valueChanged( int ) ),           this, SLOT( slot_Frames( int ) ) );

}


QvkInformation_wl::~QvkInformation_wl()
{
}


void QvkInformation_wl::slot_Format( QString value )
{
    ui->labelInfoFormat->setText( value );
    ui->labelInfoFrames->setText( QString::number( mainWindow->sliderFrames->value() ) );
}


void QvkInformation_wl::slot_Videocodec( QString value )
{
    ui->labelInfoVideocodec->setText( value );
}

/*
void QvkInformation_wl::slot_Audiocodec( QString value )
{
    bool bo = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        if ( listCheckBox.at(i)->checkState() == Qt::Checked ) {
            bo = true;
            break;
        }
    }

    if ( bo == false ) {
        ui->labelInfoAudiocodec->setText( "------" );
    } else {
        ui->labelInfoAudiocodec->setText( value );
    }
}
*/

void QvkInformation_wl::slot_Frames( int value )
{
    ui->labelInfoFrames->setText( QString::number( value ) );
}

/*
void QvkInformation_wl::slot_newVersionAvailable( QString update )
{
    if ( ui->checkBoxLookForUpdates->isChecked() == true ) {
        if ( global::version < update ) {
            QString string = "New Version available: " + update;
            ui->label_Upate_tab_1->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen-download.html'>" + string + "</a>" );
            ui->label_Upate_tab_4->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen-download.html'>" + string + "</a>" );

            QvkShowMessage *vkShowMessage = new QvkShowMessage;
            vkShowMessage->set_WindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
            vkShowMessage->set_WindowTitle( global::name + " " + global::version );
            vkShowMessage->set_URL( "https://linuxecke.volkoh.de/vokoscreen/vokoscreen.html" );
            vkShowMessage->set_StatusIcon( ":/pictures/status/information.png" );
            vkShowMessage->showMessage( "New Version available: " + update );
        } else {
            ui->label_Upate_tab_4->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen-download.html'>No update available</a>" );
        }
    } else {
        ui->label_Upate_tab_1->clear();
        ui->label_Upate_tab_4->clear();
    }
}
*/

void QvkInformation_wl::slot_cleanup()
{
    timerStorageInfo->stop();
    timerRecord->stop();
}

/*
void QvkInformation_wl::slot_timeFirstStart()
{
    elapsedTime->restart();
    int_summed = 0;
    ui->labelInfoRecordTime->setText("00:00:00");
}


void QvkInformation_wl::slot_summedTimeAfterPause()
{
    int_summed += elapsedTime->elapsed();
}


void QvkInformation_wl::slot_timeContinueStart()
{
    elapsedTime->restart();
}

*/
void QvkInformation_wl::slot_newVideoFilename( QString filename )
{
    newVideoFilename = filename;
}


void QvkInformation_wl::slot_StorageInfo()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();
    ui->labelFreeSize->setText( QString::number( storage.bytesAvailable()/1024/1024 ) );

    QDir dir( ui->lineEditVideoPath->text() );
    QStringList filters;
    filters << newVideoFilename;
    QStringList videoFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    if ( !videoFileList.empty() ) {
        QString string;
        string.append( ui->lineEditVideoPath->text() );
        string.append( "/" );
        string.append( videoFileList.at( 0 ) );
        QFileInfo file( string );
        file.refresh();
        ui->labelVideoSize->setText( QString::number( file.size()/1024 ) );
    }
}

/*
void QvkInformation_wl::slot_displayRecordTime()
{

   if ( ( ui->pushButtonStop->isEnabled() == true ) and ( ui->checkBoxStartTime->checkState() == Qt::Checked ) ) {
       QTime time( 0, 0, 0, 0 );
       ui->labelInfoRecordTime->setText( time.addMSecs( elapsedTime->elapsed() + int_summed ).toString( "hh:mm:ss" ) );
       return;
   }

   if ( ui->pushButtonStop->isEnabled() == true ) {
      QTime time( 0, 0, 0, 0 );
      ui->labelInfoRecordTime->setText( time.addMSecs( elapsedTime->elapsed() + int_summed - sliderCountDown->value()*1000 - sliderSecondWaitBeforeRecording->value()*1000 ).toString( "hh:mm:ss" ) );
   }
}
*/
