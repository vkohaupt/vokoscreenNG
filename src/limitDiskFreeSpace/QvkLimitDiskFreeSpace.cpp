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

#include "QvkLimitDiskFreeSpace.h"
#include "global.h"

#include <QDebug>
#include <QTimer>
#include <QStorageInfo>
#include <QMessageBox>

QvkLimitDiskFreeSpace::QvkLimitDiskFreeSpace( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;

    timerStorageSize = new QTimer(this);
    timerStorageSize->setTimerType( Qt::PreciseTimer );
    timerStorageSize->setInterval( 1000 );
    connect( timerStorageSize, SIGNAL( timeout() ), this, SLOT( slot_storageMessagBoxByRecord() ) );
}


QvkLimitDiskFreeSpace::~QvkLimitDiskFreeSpace()
{
}


bool QvkLimitDiskFreeSpace::isStorageOK()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop a record if disk space smaller 250MB(Default) 999MB(Max)
    if ( storage.bytesAvailable() <= ( ui->sliderLimitOfFreeDiskSpace->value() * 1024 * 1024 ) )
    {
        return false;
    }
    return true;
}


bool QvkLimitDiskFreeSpace::isStorageOKMessagBoxByStart()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop a record if disk space smaller 250MB(Default) 999MB(Max)
    if ( storage.bytesAvailable() <= ( ui->sliderLimitOfFreeDiskSpace->value() * 1024 * 1024 ) )
    {
        QMessageBox *messageBox = new QMessageBox();
        messageBox->setWindowIcon( QIcon::fromTheme( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowTitle( tr( "Warning" ) + " " + global::name + " " + global::version );
        messageBox->setText( tr( "The recording is not started, not enough free disk space." ) );
        messageBox->setInformativeText( tr( "The free disk space limit is reached." )
                                        + "\n"
                                        + "\n"
                                        + tr( "Limit of free disk space" ) + " " + QString::number( ui->sliderLimitOfFreeDiskSpace->value() ) + " " + tr( "Megabyte" )
                                      );
        messageBox->setStandardButtons( QMessageBox::Ok );
        messageBox->setIcon( QMessageBox::Warning );
        messageBox->show();

        return false;
    }

    timerStorageSize->start();
    return true;
}


void QvkLimitDiskFreeSpace::slot_storageMessagBoxByRecord()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop a record if disk space smaller 250MB(Default) 999MB(Max)
    if ( storage.bytesAvailable() <= ( ui->sliderLimitOfFreeDiskSpace->value() * 1024 * 1024 ) )
    {
        timerStorageSize->stop();
        ui->pushButtonStop->click();

        QMessageBox *messageBox = new QMessageBox();
        messageBox->setWindowIcon( QIcon::fromTheme( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowTitle( tr( "Warning" ) + " " + global::name + " " + global::version );
        messageBox->setText( tr( "The recording was stopped, not enough free disk space." ) );
        messageBox->setInformativeText( tr( "The free disk space limit is reached." )
                                        + "\n"
                                        + "\n"
                                        + tr( "Limit of free disk space" ) + " " + QString::number( ui->sliderLimitOfFreeDiskSpace->value() ) + " " + tr( "Megabyte" )
                                      );
        messageBox->setStandardButtons( QMessageBox::Ok );
        messageBox->setIcon( QMessageBox::Warning );
        messageBox->show();
    }
}
