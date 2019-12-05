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

#include "QvkInformation.h"
#include "global.h"

#include <QTimer>
#include <QTime>
#include <QStorageInfo>
#include <QMessageBox>
#include <QDebug>

QvkInformation::QvkInformation( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *slider_count_down, QvkSpezialSlider *slider_Second_Wait_Before_Recording )
{
    ui = ui_mainwindow;
    sliderCountDown = slider_count_down;
    sliderSecondWaitBeforeRecording = slider_Second_Wait_Before_Recording;

    connect( mainWindow, SIGNAL( destroyed( QObject* ) ), this, SLOT( slot_cleanup() ) );

    ui->labelVideoSize->setText("");
    ui->labelFreeSize->setText("");
    ui->labelInfoRecordTime->setText("00:00:00");

    // VideoSize and FreeDiskSpace
    timerStorageInfo = new QTimer(this);
    timerStorageInfo->setTimerType( Qt::PreciseTimer );
    timerStorageInfo->setInterval( 1000 );
    connect( timerStorageInfo, SIGNAL( timeout() ), this, SLOT( slot_StorageInfo() ) );
    timerStorageInfo->start();

    // Recorded time
    elapsedTime = new QTime( 0, 0, 0, 0 );
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
}


QvkInformation::~QvkInformation()
{
}


void QvkInformation::slot_cleanup()
{
    timerStorageInfo->stop();
    timerRecord->stop();
}


void QvkInformation::slot_timeFirstStart()
{
    elapsedTime->restart();
    int_summed = 0;
    ui->labelInfoRecordTime->setText("00:00:00");
}


void QvkInformation::slot_summedTimeAfterPause()
{
    int_summed += elapsedTime->elapsed();
}


void QvkInformation::slot_timeContinueStart()
{
    elapsedTime->restart();
}


void QvkInformation::slot_newVideoFilename( QString filename )
{
    newVideoFilename = filename;
}


void QvkInformation::slot_StorageInfo()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();
    ui->labelFreeSize->setText( QString::number( storage.bytesAvailable()/1024/1024 ) );

    QDir dir( ui->lineEditVideoPath->text() );
    QStringList filters;
    filters << newVideoFilename;
    QStringList videoFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    if ( !videoFileList.empty() )
    {
        QString string;
        string.append( ui->lineEditVideoPath->text() );
        string.append( "/" );
        string.append( videoFileList.at( 0 ) );
        QFileInfo file( string );
        file.refresh();
        ui->labelVideoSize->setText( QString::number( file.size()/1024 ) );
    }
}


void QvkInformation::slot_displayRecordTime()
{

   if ( ( ui->pushButtonStop->isEnabled() == true ) and ( ui->checkBoxStartTime->checkState() == Qt::Checked ) )
   {
       QTime time( 0, 0, 0, 0 );
       ui->labelInfoRecordTime->setText( time.addMSecs( elapsedTime->elapsed() + int_summed ).toString( "hh:mm:ss" ) );
       return;
   }

   if ( ui->pushButtonStop->isEnabled() == true )
   {
      QTime time( 0, 0, 0, 0 );
      ui->labelInfoRecordTime->setText( time.addMSecs( elapsedTime->elapsed() + int_summed - sliderCountDown->value()*1000 - sliderSecondWaitBeforeRecording->value()*1000 ).toString( "hh:mm:ss" ) );
   }
}

