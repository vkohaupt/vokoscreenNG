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
#include "QvkImageFromTabs_wl.h"

#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QPushButton>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <QDesktopServices>
#include <QBitmap>
#include <QColor>
#include <QPainter>
#include <QPainterPath>

QvkImageFromTabs_wl::QvkImageFromTabs_wl( QvkMainWindow_wl *parent )
{
    myParent = parent;
    connect( myParent->ui->pushButtonImages, SIGNAL( clicked() ), this, SLOT( slot_make_picture_from_tabs() ) );
}


QvkImageFromTabs_wl::~QvkImageFromTabs_wl()
{}


void QvkImageFromTabs_wl::slot_make_picture_from_tabs()
{
    myParent->ui->pushButtonImages->setEnabled( false );

    counterFile = 0;
    counter = 0;

    timer = new QTimer( this );
    timer->setTimerType( Qt::PreciseTimer );
    connect( timer, &QTimer::timeout, this, QOverload<>::of( &QvkImageFromTabs_wl::slot_make_picture_from_tab ) );
    connect( this, SIGNAL( signal_open_picture_folder() ), this, SLOT( slot_open_picture_folder() ) );
    timer->start( 600 );
}


void QvkImageFromTabs_wl::slot_make_picture_from_tab()
{
    if ( counter >= myParent->ui->tabWidgetScreencast->tabBar()->count() ) {
        timer->stop();
        myParent->ui->pushButtonImages->setEnabled( true );
        emit signal_open_picture_folder();
        disconnect( this, NULL, NULL, NULL );
        return;
    }

    myParent->ui->tabWidgetScreencast->setCurrentIndex( counter );
    if ( myParent->ui->tabWidgetScreencast->tabBar()->isTabVisible( counter ) == true ) {
        int titelBarHight = 32;
        counter++;

        // Grab window
        QPixmap windowPixmap;
        windowPixmap = myParent->ui->centralwidget->grab();

        // The new size for the painter
        QRect rect( 0, 0, windowPixmap.width(), windowPixmap.height() + titelBarHight );

        // The pixmap for the painter
        QPixmap pixmapImage( rect.width(), rect.height() );
        pixmapImage.fill( Qt::transparent );

        // Now we paint
        QPainter painter( &pixmapImage );
        painter.setRenderHint( QPainter::Antialiasing, true );
        painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

        // Set clipath with rounded corner
        QPainterPath clipPath;
        clipPath.addRoundedRect( rect, 5, 5 );
        painter.setClipPath( clipPath );

        // Draw titelbar
        painter.setBrush( Qt::darkGray );
        painter.setPen( Qt::darkGray );
        painter.drawRect( QRectF( 0, 0, windowPixmap.width(), titelBarHight ) );

        // Draw logo in titelbar
        QPixmap logoPixmap_1( ":/pictures/logo/logo.png" );
        QPixmap logoPixmap = logoPixmap_1.scaled( 26, 26, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        painter.drawPixmap( 3, 3, logoPixmap );

        // Draw text in titelbar
        QFont font;
        font.setPointSize( 11 );
        painter.setFont( font );
        QFontMetrics fontMetrics( font );
        int textWidth = fontMetrics.horizontalAdvance( global::name + " " + global::version );
        painter.setPen( Qt::white );
        painter.drawText( ( windowPixmap.width() / 2 ) - ( textWidth / 2 ), 21, global::name + " " + global::version );

        // Draw pixmap
        painter.drawPixmap( 0, titelBarHight, windowPixmap );
        painter.end();

        pixmapImage.save( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) + "/" + "vokoscreenNG-" + QString::number( counterFile++ ) + ".png" );
        myParent->ui->label_save_image_path->setAlignment( Qt::AlignHCenter );
        myParent->ui->label_save_image_path->setText( "Images saved in: " + QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
    }
}


void QvkImageFromTabs_wl::slot_open_picture_folder()
{
    // https://community.kde.org/Plasma/Wayland_Showstoppers
    // Sollte mit Qt 6.5 funktionieren
    if ( QDesktopServices::openUrl( QUrl( "file://" + QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ), QUrl::TolerantMode ) ) == false ) {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox;
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}
