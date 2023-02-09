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

#include "global.h"
#include "QvkImageFromTabs.h"

#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QPushButton>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <QDesktopServices>

QvkImageFromTabs::QvkImageFromTabs( QvkMainWindow *parent )
{
    myParent = parent;
    connect( myParent->ui->pushButtonImages, SIGNAL( clicked() ), this, SLOT( slot_make_picture_from_tabs() ) );
}


QvkImageFromTabs::~QvkImageFromTabs()
{}


void QvkImageFromTabs::slot_make_picture_from_tabs()
{
    myParent->ui->pushButtonImages->setEnabled( false );

    counterFile = 0;

    for ( int i = 0; i < myParent->ui->tabWidgetScreencast->tabBar()->count(); i++ )
    {
        myParent->ui->tabWidgetScreencast->setCurrentIndex( i );
        for ( int y = 0; y < 30; y++ ) {
            QCoreApplication::processEvents();
            QThread::msleep( 20 );
        }
        slot_make_picture_from_tab();
    }

    QList<QToolButton *> listToolButton = myParent->ui->widgetSidbar->findChildren<QToolButton *>();
    for ( int i = 1; i < listToolButton.count(); i++ ) {
        listToolButton.at(i)->click();
        for ( int y = 0; y < 30; y++ ) {
            QCoreApplication::processEvents();
            QThread::msleep( 20 );
        }
        slot_make_picture_from_tab();
    }

    listToolButton.at(0)->click();

    if ( QDesktopServices::openUrl( QUrl( "file:///" + QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ), QUrl::TolerantMode ) ) == false )
    {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox;
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }

    myParent->ui->pushButtonImages->setEnabled( true );
}


void QvkImageFromTabs::slot_make_picture_from_tab()
{
    QList<QScreen *> screenList = QGuiApplication::screens();

    int sumScreenWidth = 0;
    QMap<int, QString> map;
    for ( int i = 0; i < screenList.count(); i++ ) {
        sumScreenWidth = sumScreenWidth + screenList.at(i)->size().width();
        map.insert( screenList.at(i)->size().height(), QString::number( screenList.at(i)->size().height() ) );
    }

    QPixmap pixmapScreen( screenList.at(0)->grabWindow( 0, 0, 0, sumScreenWidth, map.values().last().toInt() ) );

    int left = myParent->windowHandle()->frameMargins().left();
    int top = myParent->windowHandle()->frameMargins().top();
    int right = myParent->windowHandle()->frameMargins().right();
    int bottom = myParent->windowHandle()->frameMargins().bottom();
    QPixmap windowPixmap = pixmapScreen.copy( myParent->mapToGlobal( QPoint(0,0) ).x() - left,
                                              myParent->mapToGlobal( QPoint(0,0) ).y() - top,
                                              myParent->size().width() + left + right,
                                              myParent->size().height() + top + bottom );

    windowPixmap.save( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) + "/" + "vokoscreenNG-" + QString::number( counterFile++ ) + ".png" );
    myParent->ui->label_save_image_path->setAlignment( Qt::AlignHCenter );
    myParent->ui->label_save_image_path->setText( "Images saved in: " + QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
}
