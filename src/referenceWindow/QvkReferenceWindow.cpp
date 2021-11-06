/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#include "global.h"
#include "QvkReferenceWindow.h"

#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QPushButton>
#include <QTimer>
#include <QThread>
#include <QMessageBox>

QvkReferenceWindow::QvkReferenceWindow( QvkMainWindow *parent )
{
    widget = parent;
    setTitle( "vokoscreenNG Referenz Window" );
    resize( 400, 300 );
}


void QvkReferenceWindow::init( Ui_formMainWindow *vk_ui )
{
    ui = vk_ui;
    connect( ui->pushButtonImages, SIGNAL( clicked() ), this, SLOT( slot_make_picture_from_tabs() ) );
}


QvkReferenceWindow::~QvkReferenceWindow()
{}


void QvkReferenceWindow::slot_make_picture_from_tabs()
{
    counterFile = 0;

    show();
    for ( int y = 0; y < 30; y++ )
    {
        QCoreApplication::processEvents();
        QThread::msleep( 100 );
    }
    left = frameMargins().left();
    top = frameMargins().top();
    right = frameMargins().right();
    bottom = frameMargins().bottom();
    close();

    for ( int i = 0; i < ui->tabWidgetScreencast->tabBar()->count(); i++ )
    {
        ui->tabWidgetScreencast->setCurrentIndex( i );
        for ( int y = 0; y < 30; y++ ) {
            QCoreApplication::processEvents();
            QThread::msleep( 10 );
        }
        slot_make_picture_from_tab();
    }

    QList<QToolButton *> listToolButton = ui->widgetSidbar->findChildren<QToolButton *>();
    for ( int i = 1; i < listToolButton.count(); i++ ) {
        listToolButton.at(i)->click();
        for ( int y = 0; y < 30; y++ ) {
            QCoreApplication::processEvents();
            QThread::msleep( 10 );
        }
        slot_make_picture_from_tab();
    }

    listToolButton.at(0)->click();

    if ( QDesktopServices::openUrl( QUrl( "file:///" + QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ), QUrl::TolerantMode ) ) == false )
    {
        QMessageBox msgBox;
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIcon( QMessageBox::Information );
        msgBox.exec();
    }
}


void QvkReferenceWindow::slot_make_picture_from_tab()
{
    QList<QScreen *> screenList = QGuiApplication::screens();

    int sumScreenWidth = 0;
    int largestScreen = 0;
    for ( int i = 0; i < screenList.count(); i++ )
    {
        sumScreenWidth = sumScreenWidth + screenList.at(i)->size().width();
    }

    QPixmap pixmapScreen( screenList.at(0)->grabWindow( 0, 0, 0, sumScreenWidth, 1080 ) );

    QPixmap windowPixmap = pixmapScreen.copy( widget->mapToGlobal( QPoint(0,0) ).x() - left,
                                              widget->mapToGlobal( QPoint(0,0) ).y() - top,
                                              widget->size().width() + left + right,
                                              widget->size().height() + top + bottom );

    windowPixmap.save( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) + "/" + "vokoscreenNG-" + QString::number( counterFile++ ) + ".png");
}
