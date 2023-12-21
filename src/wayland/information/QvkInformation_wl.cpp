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

#include <QTimer>
#include <QTime>
#include <QStorageInfo>
#include <QDebug>

QvkInformation_wl::QvkInformation_wl( QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *ui_mainwindow )
{
    mainWindow = vkMainWindow;
    ui = ui_mainwindow;
    sliderCountDown = vkMainWindow->sliderScreencastCountDown;
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
    timerRecord = new QTimer(this);
    timerRecord->setTimerType( Qt::PreciseTimer );
    timerRecord->setInterval( 1000 );
    connect( timerRecord,            SIGNAL( timeout() ),       this,        SLOT( slot_displayRecordTime() ) );
    connect( ui->pushButtonStop,     SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );

    // Frames, Format, Codecs
    connect( ui->comboBoxFormat,       SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_Format( QString ) ) );
    connect( ui->comboBoxVideoCodec,   SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_Videocodec( QString ) ) );
    connect( mainWindow->sliderFrames, SIGNAL( valueChanged( int ) ),           this, SLOT( slot_Frames( int ) ) );
}


QvkInformation_wl::~QvkInformation_wl()
{
}


void QvkInformation_wl::slot_beginRecordTime( QString m_beginTime )
{
    beginRecordTime = m_beginTime;
    timerRecord->start();
}


void QvkInformation_wl::slot_displayRecordTime()
{
    if ( ui->pushButtonStop->isEnabled() == true ) {
        QTime beginTime = QTime::fromString( beginRecordTime );
        int recordTime = beginTime.secsTo( QTime::currentTime() );

        QTime time( 0, 0, 0, 0 );
        ui->labelInfoRecordTime->setText( time.addMSecs( recordTime*1000 ).toString( "hh:mm:ss" ) );
        return;
    }
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


void QvkInformation_wl::slot_Frames( int value )
{
    ui->labelInfoFrames->setText( QString::number( value ) );
}


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
