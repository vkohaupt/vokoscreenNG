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
#include <QList>
#include <QCheckBox>
#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QPushButton>

QvkInformation_wl::QvkInformation_wl( QvkMainWindow_wl *vkMainWindow, Ui_formMainWindow_wl *ui_mainwindow )
{
    mainWindow = vkMainWindow;
    ui = ui_mainwindow;
    ui->labelVideoSize->setText("");
    ui->labelFreeSize->setText("");
    ui->labelInfoRecordTime->setText("00:00:00");

    // VideoSize and FreeDiskSpace
    timerStorageInfo = new QTimer(this);
    timerStorageInfo->setTimerType( Qt::PreciseTimer );
    timerStorageInfo->setInterval( 1000 );
    connect( timerStorageInfo, &QTimer::timeout, this, [this](){slot_StorageInfo();} );
    timerStorageInfo->start();

    // Recorded time
    timerRecord = new QTimer(this);
    timerRecord->setTimerType( Qt::PreciseTimer );
    timerRecord->setInterval( 1000 );
    connect(timerRecord,            &QTimer::timeout,      this, [=](){slot_displayRecordTime();});
    connect(ui->pushButtonStop,     &QPushButton::clicked, this, [=](){timerRecord->stop();});
    connect(ui->pushButtonPause,    &QPushButton::clicked, this, [=](){timerRecord->stop();});
    connect(ui->pushButtonContinue, &QPushButton::clicked, this, [=](){timerRecord->start();});

    // Frames, Format, Codecs
    connect(ui->comboBoxAudioCodec,   &QComboBox::currentTextChanged,  this, [this](QString value){slot_Audiocodec(value);});
    connect(mainWindow->sliderFrames, &QvkSpezialSlider::valueChanged, this, [this](int value)    {slot_Frames(value);});
}


QvkInformation_wl::~QvkInformation_wl()
{
}


void QvkInformation_wl::slot_beginRecordTime(QString beginTime )
{
    Q_UNUSED(beginTime)
    sumTime = 0;
    timerRecord->start();
}


void QvkInformation_wl::slot_displayRecordTime()
{
    QTime time( 0, 0, 0, 0 );
    sumTime = sumTime + 1000;
    ui->labelInfoRecordTime->setText( time.addMSecs( sumTime ).toString( "hh:mm:ss" ) );
    return;
}


void QvkInformation_wl::slot_Audiocodec( QString value )
{
    bool bo = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ ) {
        QCheckBox *checkBox = listCheckBox.at(i);
        if ( checkBox->checkState() == Qt::Checked ) {
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
