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

#include <QGuiApplication>
#include <QImageWriter>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QThread>

#include "global.h"
#include "QvkSnapshot.h"
#include "QvkSpezialSlider.h"
#include "QvkDirDialog.h"


QvkSnapshot::QvkSnapshot( QvkMainWindow *_vkMainWindow, Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;
    vkMainWindow = _vkMainWindow;
}


QvkSnapshot::~QvkSnapshot()
{
}


void QvkSnapshot::init()
{
    connect( ui->pushButtonScreencastSnapshot, SIGNAL( clicked() ), this,         SLOT( slot_newImage() ) );
    connect( ui->toolButtonSnapshotImagePath,  SIGNAL( clicked() ), this,         SLOT( slot_imagePath() ) );
    connect( ui->pushButtonSnapshotOpenfolder, SIGNAL( clicked() ), vkMainWindow, SLOT( slot_Folder() ) );
    connect( ui->pushButtonSnapshotSnapshot,   SIGNAL( clicked() ), this,         SLOT( slot_newImage() ) );

    supportedImageFormats();
    is_imageFolderExists_and_haveWritePermission();
}


void QvkSnapshot::supportedImageFormats()
{
    QList<QByteArray> listFormats = QImageWriter::supportedImageFormats();
    if ( listFormats.empty() == false )
    {
        for ( int x = 0; x < listFormats.count(); x++ ) {
            ui->comboBoxSnapshotImageFormats->addItem( QString( listFormats.at(x) ) );
        }
    }

    ui->comboBoxSnapshotImageFormats->setCurrentIndex( ui->comboBoxSnapshotImageFormats->findText( "png" ) );
}


void QvkSnapshot::slot_newImage()
{
    screen = QGuiApplication::screens();
    filename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss.zzz" ) + "." + ui->comboBoxSnapshotImageFormats->currentText().toUtf8();

    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        QImage image = screen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(0).toImage();
        bool bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

        if ( bo == false ) {
            qDebug().noquote() << global::nameOutput << "Failed to save image";
        } else {
            if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                vkMainWindow->vkSystray->showMessage( global::name, "<b>Fullscreen captured</b>", QIcon( "" ), 10000 );
            }
        }
    }

    if ( ui->radioButtonScreencastWindow->isChecked() == true )
    {
        if ( ui->pushButtonStart->isEnabled() == true )
        {
            vkWinInfo = new QvkWinInfo;
            disconnect( vkWinInfo, nullptr, nullptr, nullptr );
            connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_snapshotWindow( bool ) ) );
            vkWinInfo->slot_start();
        } else {
            WId xid = vkMainWindow->vkWinInfo->getWinID();
            QImage image = screen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(xid).toImage();
            bool bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

            if ( bo == false ) {
                qDebug().noquote() << global::nameOutput << "Failed to save image";
            } else {
                if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                    vkMainWindow->vkSystray->showMessage( global::name, "<b>Window captured</b>", QIcon( "" ), 10000 );
                }
            }
        }
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        if ( vkMainWindow->vkRegionChoise->recordemode == false )
        {
            vkMainWindow->vkRegionChoise->recordMode( true );
            QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
            QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000/10 );
        }

        int startx = vkMainWindow->vkRegionChoise->getXRecordArea();
        int starty = vkMainWindow->vkRegionChoise->getYRecordArea();
        int endx = vkMainWindow->vkRegionChoise->getWidth();
        int endy = vkMainWindow->vkRegionChoise->getHeight();

        QImage image = screen.at( ui->comboBoxScreencastScreenArea->currentIndex() )->grabWindow(0).toImage();

        QImage copyImage = image.copy( startx, starty, endx, endy );
        bool bo = copyImage.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

        if ( bo == false ) {
            qDebug().noquote() << global::nameOutput << "Failed to save image";
        } else {
            if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                vkMainWindow->vkSystray->showMessage( global::name, "<b>Area captured</b>", QIcon( "" ), 10000 );
            }
        }

        if ( ui->pushButtonStart->isEnabled() == true ) {
            vkMainWindow->vkRegionChoise->recordMode( false );
        }
    }
}


bool QvkSnapshot::is_imageFolderExists_and_haveWritePermission()
{
    // Create Folder if not exists
    QDir dir( ui->lineEditSnapshotImagePath->text() );
    if ( !dir.exists() )
    {
        // check of QStandardPaths::MoviesLocation
        QDir dir( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
        if ( !dir.exists() ) {
            bool myBool = dir.mkpath( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
            Q_UNUSED(myBool);
        }
        ui->lineEditSnapshotImagePath->setText( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
    } else {
        ui->lineEditSnapshotImagePath->setText( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
    }

    // Check write permission
    QString filename;
    filename = ui->lineEditSnapshotImagePath->text() + + "/vokoscreenNG-test-write.txt";
    bool value;
    QFileInfo fileInfo( filename );
    QFile file( filename );
    if ( file.open( QIODevice::ReadWrite ) )
    {
        QTextStream stream( &file );
        stream << "Test Test Test Test Test Test" << Qt::endl;
        file.close();
        qDebug().noquote();
        qDebug().noquote() << global::nameOutput << "Pictures permission: Can write in" << fileInfo.absolutePath();
        qDebug().noquote();
        file.remove();
        value = true;
    } else {
        qDebug().noquote();
        qDebug().noquote() << "Pictures permission: ERROR can not write in" << fileInfo.absolutePath();
        qDebug().noquote();
        QPixmap pixmap( ":/pictures/status/warning.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        QMessageBox *messageBox = new QMessageBox();
        QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowIcon( icon );
        messageBox->setWindowTitle( global::name + " " + global::version );
        messageBox->setIconPixmap( pixmap );
        messageBox->setTextFormat( Qt::RichText );
        messageBox->setText( ( "<b>No write access on pictures folder</b>" ) );
        messageBox->setInformativeText( "vokoscreenNG can not create a picture on<br>" + \
                                        fileInfo.absolutePath() + "<br><br>" + \
                                        "<b>Possible reasons:</b><br>" +
                                        "1. The folder does not exist<br>" +
                                        "2. The folder is read-only<br>" +
                                        "3. Operating system security settings<br>" +
                                        "4. Antivirus program prevents writing<br><br>" +
                                        "<b>Please fix the problem and restart vokoscreenNG<b>"
                                       );
        messageBox->exec();
        value = false;
    }
    return value;
}


void QvkSnapshot::slot_snapshotWindow( bool )
{
    QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
    QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000/10 );

    WId xid = vkWinInfo->activeWindow();
    QImage image = screen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(xid).toImage();
    bool bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

    if ( bo == false ) {
        qDebug().noquote() << global::nameOutput << "Failed to save image";
    } else {
        if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
            vkMainWindow->vkSystray->showMessage( global::name, "<b>Window captured</b>", QIcon( "" ), 10000 );
        }
    }
}


void QvkSnapshot::slot_imagePath()
{
    QApplication::setDesktopSettingsAware( false );

    QvkDirDialog vkDirDialog( this );
    if ( vkDirDialog.exec() == QDialog::Accepted )
    {
        if ( !vkDirDialog.selectedFiles().empty() ) {
            ui->lineEditSnapshotImagePath->setText( vkDirDialog.selectedFiles().at(0) );
        }
    }

    QApplication::setDesktopSettingsAware( true );
}
