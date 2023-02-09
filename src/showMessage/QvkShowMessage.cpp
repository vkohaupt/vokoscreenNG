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

#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QIcon>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QDesktopServices>
#include <QMessageBox>

QvkShowMessage::QvkShowMessage()
{
    setFixedSize( 300, 130 );
    setWindowFlag( Qt::Window, true );
    setWindowFlag( Qt::WindowStaysOnTopHint, true );

    setAttribute( Qt::WA_DeleteOnClose );

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

    labelImageDuration = new QLabel;
    labelImageDuration->setAlignment( Qt::AlignTop );
    hBoxLayoutWindow->addWidget( labelImageDuration );

    timer = new QTimer();
    timer->setTimerType( Qt::PreciseTimer );
    timer->setInterval( timerInterval );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_durationButton() ) );
    slot_durationButton();

    hide();
}


QvkShowMessage::~QvkShowMessage()
{}


// If path given, then we can click on the message and a browser or filemanger will be opened
void QvkShowMessage::set_URL( QString url )
{
    path = url;
}


void QvkShowMessage::set_Image( QImage m_image )
{
    image = m_image;
}


void QvkShowMessage::set_StatusIcon( QString m_statusIcon )
{
    statusIcon = m_statusIcon;
}


void QvkShowMessage::set_WindowIcon( QIcon icon )
{
    setWindowIcon( icon );
}


void QvkShowMessage::set_WindowTitle( QString title)
{
    windowTitle = title;
    setWindowTitle( windowTitle );
}


void QvkShowMessage::set_timeOut( qreal value )
{
    timeOut = value;
}


void QvkShowMessage::showMessage( QString text )
{
    degreeStep = 360 / timeOut * timerInterval;

    if ( statusIcon > "" )
    {
        QPixmap pixmap( statusIcon );
        pixmap = pixmap.scaled( 48, 48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        labelIcon->setPixmap( pixmap );
    }

    labelText->setText( text );

    if ( image.isNull() == false ) {
        QPixmap pixmapImage;
        pixmapImage = pixmapImage.fromImage( image );
        pixmapImage = pixmapImage.scaled( 300, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        labelImage->setPixmap( pixmapImage );
    }

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

    timer->start();

    show();
}


void QvkShowMessage::closeEvent( QCloseEvent *event )
{
    event->accept();
}


void QvkShowMessage::slot_durationButton()
{
    if ( underMouse() == true ) {
        degree = degreeStep;
    }

    int h = 16;
    QPixmap pixmap( h+2, h+2 );
    pixmap.fill( Qt::transparent );

    QPainter painter;
    painter.begin( &pixmap );
    painter.setRenderHints( QPainter::Antialiasing, true );
    painter.setOpacity( 1.0 );

    QPen pen;
    pen.setColor( Qt::black );
    pen.setWidth( 1 );
    painter.setPen( pen );
    painter.drawEllipse( QRectF( 1, 1, h, h ) );

    pen.setColor( Qt::black );
    pen.setWidth( 1 );
    painter.setPen( pen );
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QString( "#3daee9" ) );
    painter.setBrush( brush );
    degree = degree - degreeStep;
    painter.drawPie( 1, 1, h, h, 90*16, degree*16 );
    painter.end();

    labelImageDuration->setPixmap( pixmap );

    if ( degree <= -360 ) {
        close();
    }
}


void QvkShowMessage::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    // If path given, then we can click on the message and a browser or filemanger will be opened
    if ( path > "" )
    {
        if ( QDesktopServices::openUrl( QUrl( "file:///" + path, QUrl::TolerantMode ) ) == false )
        {
            QPixmap pixmap( ":/pictures/status/information.png" );
            pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

            QMessageBox msgBox( this );
            msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
            msgBox.setWindowTitle( windowTitle );
            msgBox.setIconPixmap( pixmap );
            msgBox.exec();
        }
    }
}
