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

#include "QvkShowMessage.h"
#include "global.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QIcon>
#include <QPixmap>
#include <QTimer>

QvkShowMessage::QvkShowMessage()
{
    setFixedSize( 300, 130 );
    setWindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
    setWindowFlag( Qt::Tool, true );
    setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle( global::name + " " + global::version );

    hBoxLayoutWindow = new QHBoxLayout;
    setLayout( hBoxLayoutWindow );

    vBoxLayoutIcon = new QVBoxLayout;
    hBoxLayoutWindow->addLayout( vBoxLayoutIcon );

    vBoxLayoutTextImage = new QVBoxLayout;
    hBoxLayoutWindow->addLayout( vBoxLayoutTextImage );
    hBoxLayoutWindow->insertStretch(1);


    labelIcon = new QLabel;
    vBoxLayoutIcon->addWidget( labelIcon );

    labelText = new QLabel;
    labelText->setAlignment( Qt::AlignCenter );
    QFont font;
    font.setBold( true );
    labelText->setFont( font );
    vBoxLayoutTextImage->addWidget( labelText );

    labelImage = new QLabel;
    labelImage->setAlignment( Qt::AlignCenter );
    vBoxLayoutTextImage->addWidget( labelImage );
    hBoxLayoutWindow->addStretch();

    hide();
}


QvkShowMessage::~QvkShowMessage()
{}


void QvkShowMessage::showMessage( QString text, QImage image )
{
    QPixmap pixmap( ":/pictures/status/information.png" );
    pixmap = pixmap.scaled( 48, 48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    labelIcon->setPixmap( pixmap );

    labelText->setText( text );

    QPixmap pixmapImage;
    pixmapImage = pixmapImage.fromImage( image );
    pixmapImage = pixmapImage.scaled( 300, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    labelImage->setPixmap( pixmapImage );

    QPoint globalCursorPos = QCursor::pos();
    myScreen = QGuiApplication::screenAt( globalCursorPos );
    if ( myScreen ) {
    } else {
        // screenAt found no screen. Without a return the Application is crashed
        return;
    }

    // move is global
    move( myScreen->geometry().x() + myScreen->geometry().width() - width(),
          myScreen->geometry().y() + myScreen->geometry().height() - height() );

    QTimer::singleShot( 10000, this, SLOT( close() ) );

    show();
}


void QvkShowMessage::closeEvent( QCloseEvent *event )
{
    event->accept();
}
