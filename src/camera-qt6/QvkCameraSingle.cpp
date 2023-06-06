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

#include "ui_formMainWindow.h"
#include "QvkCameraController.h"
#include "QvkCameraSingle.h"
#include "global.h"

#include <QDebug>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QCameraFormat>
#include <QRadioButton>
#include <QObject>
#include <QList>
#include <QVideoFrame>
#include <QImage>

#include <QVBoxLayout>
#include <QHBoxLayout>

QvkCameraSingle::QvkCameraSingle( Ui_formMainWindow *ui_surface, QCameraDevice m_cameraDevice )
{
    ui = ui_surface;
    cameraDevice = m_cameraDevice;
    qDebug().noquote() << global::nameOutput << "[Camera] ------------------Added:" << cameraDevice.description() << "Device:" << cameraDevice.id();

    QList<QVBoxLayout *> listQVBoxLayout = ui->centralWidget->findChildren<QVBoxLayout *>( "layoutAllCameras" );

    QHBoxLayout *layoutCamera = new QHBoxLayout;
    layoutCamera->setObjectName( "layoutCamera-" + QString::number( 0 ) );
    listQVBoxLayout.at(0)->addLayout( layoutCamera );

    QList<QRadioButton *> listRadioButton = ui->centralWidget->findChildren<QRadioButton *>();
    int counter = 0;
    for ( int i = 0; i < listRadioButton.count(); i++ ) {
        if ( listRadioButton.at(i)->objectName().contains( "radioButtonCamera-" ) ) {
            counter++;
        }
    }

    radioButtonCamera = new QRadioButton;
    layoutCamera->addWidget( radioButtonCamera );
    radioButtonCamera->setObjectName( "radioButtonCamera-" + QString::number( counter ) );
    radioButtonCamera->setAutoExclusive( true );
    connect( radioButtonCamera, SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonCameraTextClicked( bool ) ) );

    checkBoxCameraOnOff = new QCheckBox;
    layoutCamera->addWidget( checkBoxCameraOnOff );
    checkBoxCameraOnOff->setText( cameraDevice.description() );
    checkBoxCameraOnOff->setAccessibleName( cameraDevice.id() );
    checkBoxCameraOnOff->setObjectName( "checkBoxCameraOnOff-" + QString::number( counter ) );
    connect( checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this, SLOT( slot_checkBoxCameraOnOff( bool ) ) );

    layoutCamera->setStretchFactor( checkBoxCameraOnOff, 2 );

    checkBoxCameraWindowFrame = new QCheckBox;
    checkBoxCameraWindowFrame->setText( tr( "Remove window frame" ) );
    checkBoxCameraWindowFrame->setObjectName( "checkBoxCameraWindowFrame-" + QString::number( counter )  );
    ui->horizontalLayout_42->insertWidget( 0, checkBoxCameraWindowFrame, 1 );
    connect( checkBoxCameraWindowFrame, SIGNAL( clicked( bool ) ), this, SLOT( slot_frameOnOff( bool ) ) );

    checkBoxCameraMirrorVertical = new QCheckBox;
    checkBoxCameraMirrorVertical->setText( tr( "Flip vertical" ) );
    checkBoxCameraMirrorVertical->setObjectName( "checkBoxCameraMirrorVertical-" + QString::number( counter )  );
    ui->horizontalLayout_12->insertWidget( 0, checkBoxCameraMirrorVertical, 1 );

    checkBoxCameraMirrorHorizontal = new QCheckBox;
    checkBoxCameraMirrorHorizontal->setText( tr( "Flip horizontal" ) );
    checkBoxCameraMirrorHorizontal->setObjectName( "checkBoxCameraMirrorHorizontal-" + QString::number( counter )  );
    ui->horizontalLayout_12->insertWidget( 1, checkBoxCameraMirrorHorizontal, 2 );

    checkBoxCameraInvert = new QCheckBox;
    checkBoxCameraInvert->setText( tr( "Invert" ) );
    checkBoxCameraInvert->setObjectName( "checkBoxCameraInvert-" + QString::number( counter )  );
    ui->horizontalLayout_14->insertWidget( 0, checkBoxCameraInvert, 1 );

    checkBoxCameraGray = new QCheckBox;
    checkBoxCameraGray->setText( tr( "Gray" ) );
    checkBoxCameraGray->setObjectName( "checkBoxCameraGray-" + QString::number( counter )  );
    ui->horizontalLayout_14->insertWidget( 1, checkBoxCameraGray, 1 );

    checkBoxCameraMono = new QCheckBox;
    checkBoxCameraMono->setText( tr( "Black-and-white" ) );
    checkBoxCameraMono->setObjectName( "checkBoxCameraMono-" + QString::number( counter )  );
    ui->horizontalLayout_14->insertWidget( 2, checkBoxCameraMono, 2 );

    slot_radioButtonCameraTextClicked( false );
}


QvkCameraSingle::~QvkCameraSingle()
{
}


void QvkCameraSingle::slot_checkBoxCameraOnOff( bool value )
{
    // Camera starten
    if ( value == true )
    {
        camera = new QCamera( cameraDevice );
        const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
        camera->setCameraFormat( cameraFormatList.at(0) );
        qDebug() << cameraFormatList.count() << cameraFormatList.at(0).pixelFormat() ;

        videoSink = new QVideoSink;
        connect( videoSink, SIGNAL( videoFrameChanged( QVideoFrame ) ), this, SLOT( slot_videoFrameChanged( QVideoFrame ) ) );

        vkCameraWindow = new QvkCameraWindow;
        connect( vkCameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), checkBoxCameraOnOff, SLOT( toggle() ) );
        vkCameraWindow->show();

        captureSession = new QMediaCaptureSession;
        captureSession->setCamera( camera );
        captureSession->setVideoOutput( videoSink );

        camera->start();
    }

    // Camera stopen
    if ( value == false )
    {
        camera->stop();
        camera->deleteLater();
        captureSession->deleteLater();
        disconnect( videoSink );
        videoSink->deleteLater();
        vkCameraWindow->deleteLater();
    }
}


void QvkCameraSingle::slot_frameOnOff( bool value )
{
    if ( vkCameraWindow->isFullScreen() == false )
    {
        Qt::WindowFlags flags;

#ifdef Q_OS_WIN
        if ( value == true )
        {
            cameraWindow->setWindowFlag( Qt::Window, false );
            cameraWindow->setWindowFlag( Qt::ToolTip, true );
        }

        if ( value == false )
        {
            cameraWindow->setWindowFlag( Qt::Window, true );
            cameraWindow->setWindowFlag( Qt::ToolTip, false );
        }
#endif

#ifdef Q_OS_LINUX
        if ( value == true )
        {
            flags = Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
            vkCameraWindow->setWindowFlags( flags );
        }

        if ( value == false )
        {
            flags = Qt::WindowStaysOnTopHint;
            vkCameraWindow->setWindowFlags( flags );
        }
#endif

        if ( checkBoxCameraOnOff->isChecked() == true )
        {
            vkCameraWindow->show();
        }
    }
}


void QvkCameraSingle::slot_radioButtonCameraTextClicked( bool value )
{
    Q_UNUSED(value)
    QList<QLabel *> listLabel = ui->centralWidget->findChildren<QLabel *>( "labelCurrentCamera" );
    if ( radioButtonCamera->isChecked() == true ) {
        listLabel.at(0)->setText( checkBoxCameraOnOff->text() );
    }

    QList<QCheckBox *> list = ui->centralWidget->findChildren<QCheckBox *>();
    if ( list.empty() == false ) {
        for ( int i = 0; i < list.count(); i++ ) {
            if ( list.at(i)->objectName().contains( "checkBoxCameraWindowFrame" ) ) {
                list.at(i)->hide();
            }
            if ( list.at(i)->objectName().contains( "checkBoxCameraMirrorVertical" ) ) {
                list.at(i)->hide();
            }
            if ( list.at(i)->objectName().contains( "checkBoxCameraMirrorHorizontal" ) ) {
                list.at(i)->hide();
            }
            if ( list.at(i)->objectName().contains( "checkBoxCameraInvert" ) ) {
                list.at(i)->hide();
            }
            if ( list.at(i)->objectName().contains( "checkBoxCameraGray" ) ) {
                list.at(i)->hide();
            }
            if ( list.at(i)->objectName().contains( "checkBoxCameraMono" ) ) {
                list.at(i)->hide();
            }
        }
    }

    // If the config emmpty the last camera is set
    radioButtonCamera->setChecked( true );

    checkBoxCameraWindowFrame->show();
    checkBoxCameraMirrorHorizontal->show();
    checkBoxCameraMirrorVertical->show();
    checkBoxCameraInvert->show();
    checkBoxCameraGray->show();
    checkBoxCameraMono->show();
}


void QvkCameraSingle::slot_videoFrameChanged( QVideoFrame videoFrame )
{
    QImage image = videoFrame.toImage();
    image.convertTo( QImage::Format_RGB32 );

    if ( checkBoxCameraMirrorHorizontal->isChecked() == true ) {
        image = image.mirrored( true, false );
    }

    if ( checkBoxCameraMirrorVertical->isChecked() == true ) {
        image = image.mirrored( false, true );
    }

    if ( checkBoxCameraInvert->isChecked() == true ) {
        image.invertPixels( QImage::InvertRgb );
    }

    if ( checkBoxCameraGray->isChecked() == true ) {
        image = image.convertToFormat( QImage::Format_Grayscale8 );
    }

    if ( checkBoxCameraMono->isChecked() == true ) {
        image = image.convertToFormat( QImage::Format_Mono );
    }

    QPixmap pixmap;
    pixmap.convertFromImage( image );
    vkCameraWindow->setPixmap( pixmap );
    vkCameraWindow->setFixedSize( pixmap.width(), pixmap.height() );

    /*

    // Zoom
    if ( sliderCameraWindowZoom->value() > 0 )
    {
        qreal width = image.width();
        qreal height = image.height();
        qreal quotient = width / height;
        qreal minusPixel = sliderCameraWindowZoom->value();
        QImage image_zoom = image.copy( minusPixel,
                                       minusPixel / quotient,
                                       width - ( 2 * minusPixel ),
                                       height - ( 2 * minusPixel / quotient )
                                       );
        image = image_zoom.scaled( width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    }
    // Zoom end


    // Rectangel
    if ( ui->toolButton_camera_view_rectangle->isChecked() == true )
    {
        if ( cameraWindow->isFullScreen() == false )
        {
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            int w = ui->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            int h = ui->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;
            image = image.scaled( w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
            cameraWindow->setFixedSize( image.width(), image.height() );
            cameraWindow->setNewImage( image );
            return;
        } else {
            cameraWindow->setNewImage( image );
            return;
        }
    }
    // Rectangel end


    // Ellipse
    if ( ui->toolButton_camera_view_ellipse->isChecked() == true )
    {
        if ( cameraWindow->isFullScreen() == false ) {
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            int w = ui->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            int h = ui->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;
            image = image.scaled( w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        }
        QPixmap pixmap( image.width(), image.height() );
        pixmap.fill( Qt::transparent );
        QPainter painter;
        painter.begin( &pixmap );
        painter.setRenderHints( QPainter::Antialiasing, true );
        QPainterPath path;
        path.addEllipse( 0, 0, image.width(), image.height() );
        painter.setClipPath( path );
        painter.drawImage( QPoint( 0, 0 ), image );
        painter.end();
        image = pixmap.toImage();
        if ( cameraWindow->isFullScreen() == false ) {
            cameraWindow->setFixedSize( image.width(), image.height() );
        }
    }


    // Circle
    if ( ui->toolButton_camera_view_circle->isChecked() == true )
    {
        if ( cameraWindow->isFullScreen() == true ){
            int w = image.width();
            int h = image.height();
            QPixmap pixmap( w, h );
            pixmap.fill( Qt::transparent );
            QPainter painter;
            painter.begin( &pixmap );
            painter.setRenderHints( QPainter::Antialiasing, true );
            QPainterPath path;
            path.addEllipse( (w-h)/2, 0, h, h );
            painter.setClipPath( path );
            image = image.scaled( w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation );
            painter.drawImage( 0, 0, image );
            painter.end();
            image = pixmap.toImage();
        } else {
#ifdef Q_OS_LINUX
            // Under linux absolute perfect. Under Windows outer black line.
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            qreal w = ui->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            qreal h = ui->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;

            QPixmap pixmap( h, h );
            pixmap.fill( Qt::transparent );
            QPainter painter;
            painter.begin( &pixmap );
            painter.setRenderHints( QPainter::Antialiasing, true );
            painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

            QPainterPath path;
            path.addEllipse( 0, 0, h, h );
            painter.setClipPath( path );

            QRectF target( 0.0, 0.0, h, h );
            QRectF source( (w-h)/2, 0.0, image.height(), image.height() );
            painter.drawImage( target, image, source );

            painter.end();

            image = pixmap.toImage();

            cameraWindow->setFixedSize( h, h );
#endif \
            // Under Windows a bit better as the other code
#ifdef Q_OS_WIN
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            qreal w = ui->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            qreal h = ui->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;

            QPixmap pixmap( h, h );
            pixmap.fill( Qt::transparent );
            QPainter painter;
            painter.begin( &pixmap );
            painter.setRenderHint( QPainter::Antialiasing, true );
            painter.setRenderHint( QPainter::SmoothPixmapTransform, true );

            QRegion region( QRect( 0, 0, h, h ), QRegion::Ellipse );
            painter.setClipRegion( region );
            painter.drawImage( QPointF( 0.0, 0.0 ), image );

            QRectF target( 0.0, 0.0, h, h );
            QRectF source( (w-h)/2, 0.0, image.height(), image.height() );
            painter.drawImage( target, image, source );
            painter.end();

            image = pixmap.toImage();

            cameraWindow->setFixedSize( h, h );
#endif
        }
    }
    // Circle end
*/
}
