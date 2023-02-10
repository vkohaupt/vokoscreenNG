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
#include <QDesktopServices>

#include "global.h"
#include "QvkSnapshot.h"
#include "QvkSpezialSlider.h"
#include "QvkDirDialog.h"
#include "QvkShowMessage.h"


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
    connect( ui->pushButtonScreencastSnapshot, SIGNAL( clicked() ), this, SLOT( slot_newImage() ) );
    connect( ui->toolButtonSnapshotImagePath,  SIGNAL( clicked() ), this, SLOT( slot_imagePath() ) );
    connect( ui->pushButtonSnapshotOpenfolder, SIGNAL( clicked() ), this, SLOT( slot_Folder() ) );
    connect( ui->pushButtonSnapshotSnapshot,   SIGNAL( clicked() ), this, SLOT( slot_newImage() ) );

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
    myScreen = QGuiApplication::screens();
    filename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss.zzz" ) + "." + ui->comboBoxSnapshotImageFormats->currentText().toUtf8();

    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        bool wasVisible = false;
        if ( ( ui->checkBoxSnapshotHideBeforeRecording->isChecked() == true ) and ( vkMainWindow->isMinimized() == false ) ) {
            wasVisible = true;
            vkMainWindow->showMinimized();
        }

        QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
        QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000/10 );

        QImage image = myScreen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(0).toImage();
        bool bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

        if ( bo == false ) {
            qDebug().noquote() << global::nameOutput << "Failed to save fullscreen snapshot";
        } else {
            qDebug().noquote() << global::nameOutput << "Fullscreen snapshot saved:" << ui->lineEditSnapshotImagePath->text() + "/" + filename;
            if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                QvkShowMessage *vkShowMessage = new QvkShowMessage;
                vkShowMessage->set_URL( "file://" + ui->lineEditSnapshotImagePath->text() );
                vkShowMessage->set_Image( image );
                vkShowMessage->set_StatusIcon( ":/pictures/status/information.png" );
                vkShowMessage->set_WindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
                vkShowMessage->set_WindowTitle( global::name + " " + global::version );
                vkShowMessage->set_timeOut( 10000 );
                vkShowMessage->showMessage( QString( "Fullscreen captured" ) );
            }
        }

        if ( ( ui->checkBoxSnapshotHideBeforeRecording->isChecked() == true ) and ( wasVisible == true ) ) {
            vkMainWindow->showNormal();
        }
    }

    if ( ui->radioButtonScreencastWindow->isChecked() == true )
    {
        if ( ui->pushButtonStart->isEnabled() == true )
        {
            vkWinInfo = new QvkWinInfo;
            disconnect( vkWinInfo, nullptr, nullptr, nullptr );
            // The slot is triggered if no recording is running
            connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this, SLOT( slot_snapshotWindow( bool ) ) );
            vkWinInfo->slot_start();
        } else {
            WId xid = vkMainWindow->vkWinInfo->getWinID();
            QImage image = myScreen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(xid).toImage();
            bool bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

            if ( bo == false ) {
                qDebug().noquote() << global::nameOutput << "Failed to save window snapshot during a recording is running";
            } else {
                qDebug().noquote() << global::nameOutput << "A recording is running and a Window Snapshot is saved:" << ui->lineEditSnapshotImagePath->text() + "/" + filename;
                if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                    QvkShowMessage *vkShowMessage = new QvkShowMessage;
                    vkShowMessage->set_URL( "file://" + ui->lineEditSnapshotImagePath->text() );
                    vkShowMessage->set_Image( image );
                    vkShowMessage->set_StatusIcon( ":/pictures/status/information.png" );
                    vkShowMessage->set_WindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
                    vkShowMessage->set_WindowTitle( global::name + " " + global::version );
                    vkShowMessage->set_timeOut( 10000 );
                    vkShowMessage->showMessage( "Window captured" );
                }
            }
        }
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        bool wasVisible = false;
        if ( ( ui->checkBoxSnapshotHideBeforeRecording->isChecked() == true ) and ( vkMainWindow->isMinimized() == false ) ) {
            wasVisible = true;
            vkMainWindow->showMinimized();
        }

        if ( vkMainWindow->vkRegionChoise->recordemode == false )
        {
            vkMainWindow->vkRegionChoise->recordMode( true );
            QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
            QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000/10 );
        }

        QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
        QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000/10 );

        int startx = vkMainWindow->vkRegionChoise->getXRecordArea();
        int starty = vkMainWindow->vkRegionChoise->getYRecordArea();
        int endx = vkMainWindow->vkRegionChoise->getWidth();
        int endy = vkMainWindow->vkRegionChoise->getHeight();

        QImage image = myScreen.at( ui->comboBoxScreencastScreenArea->currentIndex() )->grabWindow(0).toImage();

        QImage copyImage = image.copy( startx, starty, endx, endy );
        bool bo = copyImage.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

        if ( bo == false ) {
            qDebug().noquote() << global::nameOutput << "Failed to save aera snapshot";
        } else {
            qDebug().noquote() << global::nameOutput << "Aera Snapshot saved:" << ui->lineEditSnapshotImagePath->text() + "/" + filename;
            if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
                QvkShowMessage *vkShowMessage = new QvkShowMessage;
                vkShowMessage->set_URL( "file://" + ui->lineEditSnapshotImagePath->text() );
                vkShowMessage->set_Image( copyImage );
                vkShowMessage->set_StatusIcon( ":/pictures/status/information.png" );
                vkShowMessage->set_WindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
                vkShowMessage->set_WindowTitle( global::name + " " + global::version );
                vkShowMessage->set_timeOut( 10000 );
                vkShowMessage->showMessage( "Area captured" );
            }
        }

        if ( ui->pushButtonStart->isEnabled() == true ) {
            vkMainWindow->vkRegionChoise->recordMode( false );
        }

        if ( ( ui->checkBoxSnapshotHideBeforeRecording->isChecked() == true ) and ( wasVisible == true ) ) {
            vkMainWindow->showNormal();
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
                                        "<b>Possible Cause:</b><br>" +
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

// The slot is triggered if no recording is running
void QvkSnapshot::slot_snapshotWindow( bool )
{
    QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderWaitBeforeSnapshot" );
    QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000/10 );

    WId xid = vkWinInfo->activeWindow();
    QImage image = myScreen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(xid).toImage();
    bool bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + filename, ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

    if ( bo == false ) {
        qDebug().noquote() << global::nameOutput << "No recording and a Window Snapshot failed to save image";
    } else {
        qDebug().noquote() << global::nameOutput << "No Recording and a Window Snapshot is saved:" << ui->lineEditSnapshotImagePath->text() + "/" + filename;
        if ( ui->checkBoxSnapshotShowBallonInSystray->isChecked() == true ) {
            QvkShowMessage *vkShowMessage = new QvkShowMessage;
            vkShowMessage->set_URL( "file://" + ui->lineEditSnapshotImagePath->text() );
            vkShowMessage->set_Image( image );
            vkShowMessage->set_StatusIcon( ":/pictures/status/information.png" );
            vkShowMessage->set_WindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
            vkShowMessage->set_WindowTitle( global::name + " " + global::version );
            vkShowMessage->set_timeOut( 10000 );
            vkShowMessage->showMessage( "Window captured" );
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


void QvkSnapshot::slot_Folder()
{
    QString path = ui->lineEditSnapshotImagePath->text();

    if ( QDesktopServices::openUrl( QUrl( "file:///" + path, QUrl::TolerantMode ) ) == false )
    {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}
