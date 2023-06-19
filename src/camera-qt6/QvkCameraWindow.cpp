/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2023 Volker Kohaupt
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

#include "QvkCameraWindow.h"
#include "global.h"

#include <QDebug>
#include <QBitmap>


QvkCameraWindow::QvkCameraWindow( Ui_formMainWindow *ui_surface, QCheckBox *checkBox, QLabel *m_labelCameraWindowSize )
{
    ui = ui_surface;
    checkBoxCameraWindowFrame = checkBox;
    labelCameraWindowSize = m_labelCameraWindowSize;

    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
    setWindowIcon( QIcon( QString::fromUtf8( ":/pictures/logo/logo.png" ) ) );
    setWindowTitle( QString( tr( "Camera") ) + " " + global::version );
    resize( 100, 100 );
    move( 100, 100 );
}


QvkCameraWindow::~QvkCameraWindow()
{
}


void QvkCameraWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event)

    if ( image.format() == QImage::Format_Invalid ) {
        if ( error > "" ) {
            int w = 480;
            int h = 300;
            int pointSize = 14;
            int frameWidth = 8;
            resize( w, h );
            QPainter painter;
            painter.begin( this );
            painter.setRenderHints( QPainter::Antialiasing, true );

            // Background
            QBrush brush( Qt::white );
            painter.fillRect( 0, 0, w, h, brush );

            // Frame
            if ( checkBoxCameraWindowFrame->isChecked() == true )
            {
                QPen pen;
                pen.setWidth( frameWidth );
                pen.setColor( QString( "#9EBBD8" ) );
                painter.setPen( pen );
                painter.drawRect( frameWidth/2, frameWidth/2, w-frameWidth, h-frameWidth );
            }

            // Text
            QFont font;
            font.setPointSize( pointSize );
            painter.setFont( font );
            QPen pen;
            pen.setColor( Qt::black );
            painter.setPen( pen );
            painter.drawText( QRect( 0, 0, w, h/2 ), Qt::AlignCenter, global::name + " " + global::version );
            painter.drawText( QRectF( 0, 0, w, h ), Qt::AlignCenter, error );

            // Close button
            if ( checkBoxCameraWindowFrame->isChecked() == true )
            {
                int width = 20;
                int height = 20;
                int distanceToFrame = 3;
                rectCloseButton.setRect( w - frameWidth - distanceToFrame - width, frameWidth + distanceToFrame, width, height );
                QPen pen;
                pen.setWidth( 2 );
                pen.setColor( Qt::red );
                painter.setPen( pen );
                painter.drawLine( rectCloseButton.topRight(), rectCloseButton.bottomLeft() );
                painter.drawLine( rectCloseButton.topLeft(), rectCloseButton.bottomRight() );
            }

            painter.end();
            return;
        }
        return;
    }

    int displayedWidth = 0;
    int displayedHeight = 0;
    if ( isFullScreen() == true ) {
        displayedWidth = width();
        displayedHeight = height();
        image = image.scaled( displayedWidth, displayedHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    } else {
        displayedWidth = image.width();
        displayedHeight = image.height();
    }

    QPixmap pixmap( displayedWidth, displayedHeight );
    if ( checkBoxCameraWindowFrame->isChecked() == true ) {
        if ( isFullScreen() == true ) {
            pixmap.fill( Qt::black );
        } else {
            pixmap.fill( Qt::transparent );
        }
    } else {
        pixmap.fill( Qt::black );
    }

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHint( QPainter::Antialiasing, true );
    painterPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painterPixmap.drawImage( ( width() - image.width() ) / 2 , 0, image );
    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painter.drawPixmap( QPoint( 0, 0 ), pixmap );
    painter.end();

    if ( checkBoxCameraWindowFrame->isChecked() == true ) {
        setMask( pixmap.mask() );
    } else {
        clearMask();
    }

}


void QvkCameraWindow::setNewImage( QImage _image )
{
    image = _image;
    repaint();
}


void QvkCameraWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    emit signal_cameraWindow_close( false );
}


void QvkCameraWindow::resizeEvent( QResizeEvent *event )
{
    Q_UNUSED(event);
    if ( isFullScreen() == true ) {
        //        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Window" ) );
    } else {
        //        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Fullscreen" ) );
        //        vkCameraSettingsDialog->ui->labelCameraWindowSize->setText( QString::number( width() ) + "x" + QString::number( height() ) );
        labelCameraWindowSize->setText( QString::number( width() ) + "x" + QString::number( height() ) );
    }
}


void QvkCameraWindow::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape ) {
        showNormal();
    }

    if ( ( event->key() == Qt::Key_F11 ) or ( event->key() == Qt::Key_F ) ) {
        if ( isFullScreen() == true ) {
            showNormal();
        } else {
            setWindowState( Qt::WindowFullScreen );
        }
    }
}


void QvkCameraWindow::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        if ( isFullScreen() == true ) {
            showNormal();
        } else {
            setWindowState( Qt::WindowFullScreen );
            //            vkCameraSettingsDialog->close();
        }
    }
}


void QvkCameraWindow::mousePressEvent( QMouseEvent *event )
{
    if ( rectCloseButton.contains( event->pos() ) ) {
        close();
        return;
    }
/*
    if ( event->button() == Qt::RightButton )
    {
        if ( vkCameraSettingsDialog->isVisible() ) {
            vkCameraSettingsDialog->close();
        } else {
            vkCameraSettingsDialog->show();
            if ( isFullScreen() == true )
            {
                vkCameraSettingsDialog->ui->widgetCameraWindowSize->hide();
                vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->hide();
            } else {
                vkCameraSettingsDialog->ui->widgetCameraWindowSize->show();
                vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->show();
            }
        }
        return;
    }

    if ( isFullScreen() == true ) {
        vkCameraSettingsDialog->close();
    }
*/
    if ( ( checkBoxCameraWindowFrame->isChecked() == true ) and ( event->button() == Qt::LeftButton ) and ( isFullScreen() == false ) ) {
        QPixmap pixmap( ":/pictures/cursor/size_all.png" );
        QCursor cursor( pixmap );
        setCursor( cursor );
        mousePressed = true;
        mouseLocal_X = event->position().x();
        mouseLocal_Y = event->position().y();
    }
}


void QvkCameraWindow::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    if ( ( checkBoxCameraWindowFrame->isChecked() == true ) and ( isFullScreen() == false ) ) {
        unsetCursor();
        mousePressed = false;
    }
}


void QvkCameraWindow::mouseMoveEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    if ( ( checkBoxCameraWindowFrame->isChecked() == true ) and ( mousePressed == true ) ) {
        move( QCursor::pos().x() - mouseLocal_X, QCursor::pos().y() - mouseLocal_Y );
    }
}

