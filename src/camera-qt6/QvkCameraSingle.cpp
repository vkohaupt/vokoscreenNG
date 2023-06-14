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
#include <QPainterPath>

#include <QVBoxLayout>
#include <QHBoxLayout>

QvkCameraSingle::QvkCameraSingle( Ui_formMainWindow *ui_surface, QCameraDevice m_cameraDevice )
{
    ui = ui_surface;
    cameraDevice = m_cameraDevice;
    qDebug().noquote() << global::nameOutput << "[Camera] Added:" << cameraDevice.description() << "Device:" << cameraDevice.id();

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
    connect( radioButtonCamera, SIGNAL( clicked( bool ) ), this, SLOT( slot_radioButtonCurrentCameraClicked( bool ) ) );

    checkBoxCameraOnOff = new QCheckBox;
    layoutCamera->addWidget( checkBoxCameraOnOff );
    checkBoxCameraOnOff->setText( cameraDevice.description() );
    checkBoxCameraOnOff->setAccessibleName( cameraDevice.id() );
    checkBoxCameraOnOff->setObjectName( "checkBoxCameraOnOff-" + QString::number( counter ) );
    connect( checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this, SLOT( slot_checkBoxCameraOnOff( bool ) ) );
    layoutCamera->setStretchFactor( checkBoxCameraOnOff, 2 );

    comboBoxCameraVideoFormat = new QComboBox;
    connect( comboBoxCameraVideoFormat, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_comboboxCameraInsertResolutions( int ) ) );
    connect( comboBoxCameraVideoFormat, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_comboboxCameraFormatCurrentIndexChanged( int ) ) );
    layoutCamera->addWidget( comboBoxCameraVideoFormat );

    comboBoxCameraResolution = new QComboBox;
    connect( comboBoxCameraResolution, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_setComboboxCameraFPS( QString ) ) );
    layoutCamera->addWidget( comboBoxCameraResolution );

    // Wird zur Zeit nicht benötigt da Informationen fehlen
    comboBoxCameraFrame = new QComboBox;
    layoutCamera->addWidget( comboBoxCameraFrame );
    comboBoxCameraFrame->hide();


    for ( int i = 0; i < cameraDevice.videoFormats().count(); i++ ) {
        QString format = QVideoFrameFormat::pixelFormatToString( cameraDevice.videoFormats().at(i).pixelFormat() ).toUpper();
        if ( comboBoxCameraVideoFormat->findText( format ) == -1 ) {
            comboBoxCameraVideoFormat->addItem( format, cameraDevice.videoFormats().at(i).pixelFormat() ); // bei der Philips wird der Wert 17 für YUYV eingetragen und für JPEG der Wert 29
        }
    }


    sliderCameraWindowSize = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_45->insertWidget( 0, sliderCameraWindowSize );
    sliderCameraWindowSize->setObjectName( "sliderCameraWindowSize-" + QString::number( counter ) );
    sliderCameraWindowSize->setMinimum( 0 );
    sliderCameraWindowSize->setMaximum( 1 );
    sliderCameraWindowSize->setValue( 0 );
    sliderCameraWindowSize->show();
    sliderCameraWindowSize->setShowValue( false );
    sliderCameraWindowSize->setBigHandel( true );
    sliderCameraWindowSize->setEnabled( true );

    sliderCameraWindowZoom = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_zoom->insertWidget( 0, sliderCameraWindowZoom );
    sliderCameraWindowZoom->setObjectName( "sliderCameraWindowZoom-" + QString::number( counter ) );
    sliderCameraWindowZoom->setMinimum( 0 );
    sliderCameraWindowZoom->setMaximum( 1 );
    sliderCameraWindowZoom->setValue( 0 );
    sliderCameraWindowZoom->show();
    sliderCameraWindowZoom->setShowValue( true );
    sliderCameraWindowZoom->setEnabled( true );

    toolButton_camera_view_rectangle = new QToolButton;
    toolButton_camera_view_rectangle->setObjectName( "toolButton_camera_view_rectangle-" + QString::number( counter ) );
    toolButton_camera_view_rectangle->setCheckable( true );
    toolButton_camera_view_rectangle->setChecked( true );
    toolButton_camera_view_rectangle->setAutoExclusive( true );
    toolButton_camera_view_rectangle->setAutoRaise( true );
    toolButton_camera_view_rectangle->setIconSize( QSize( 34, 34 ) );
    toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-off.png" ) );
    ui->horizontalLayout_65->insertWidget( 1, toolButton_camera_view_rectangle, 1 );
    connect( toolButton_camera_view_rectangle, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonClicked( bool ) ) );

    toolButton_camera_view_ellipse = new QToolButton;
    toolButton_camera_view_ellipse->setObjectName( "toolButton_camera_view_ellipse-" + QString::number( counter ) );
    toolButton_camera_view_ellipse->setCheckable( true );
    toolButton_camera_view_ellipse->setAutoExclusive( true );
    toolButton_camera_view_ellipse->setAutoRaise( true );
    toolButton_camera_view_ellipse->setIconSize( QSize( 34, 34 ) );
    toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-off.png" ) );
    ui->horizontalLayout_65->insertWidget( 2, toolButton_camera_view_ellipse, 1 );
    connect( toolButton_camera_view_ellipse, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonClicked( bool ) ) );

    toolButton_camera_view_circle = new QToolButton;
    toolButton_camera_view_circle->setObjectName( "toolButton_camera_view_circle-" + QString::number( counter ) );
    toolButton_camera_view_circle->setCheckable( true );
    toolButton_camera_view_circle->setAutoExclusive( true );
    toolButton_camera_view_circle->setAutoRaise( true );
    toolButton_camera_view_circle->setIconSize( QSize( 34, 34 ) );
    toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-off.png" ) );
    ui->horizontalLayout_65->insertWidget( 3, toolButton_camera_view_circle, 1 );
    connect( toolButton_camera_view_circle, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonClicked( bool ) ) );

    checkBoxCameraWindowFrame = new QCheckBox;
    checkBoxCameraWindowFrame->setText( tr( "Remove window frame" ) );
    checkBoxCameraWindowFrame->setObjectName( "11checkBoxCameraWindowFrame-" + QString::number( counter ) );
    ui->horizontalLayout_42->insertWidget( 0, checkBoxCameraWindowFrame, 1 );

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

    vkCameraWindow = new QvkCameraWindow( checkBoxCameraWindowFrame );
    connect( vkCameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
    connect( checkBoxCameraWindowFrame, SIGNAL( clicked( bool ) ), this, SLOT( slot_cameraWindowFrameOnOff( bool ) ) );

    slot_radioButtonCurrentCameraClicked( false );
}


QvkCameraSingle::~QvkCameraSingle()
{
}


void QvkCameraSingle::slot_comboboxCameraFormatCurrentIndexChanged( int value )
{
    if ( camera != Q_NULLPTR ) {
        slot_checkBoxCameraOnOff( false );
        slot_checkBoxCameraOnOff( true );
    }
}


void QvkCameraSingle::slot_comboboxCameraInsertResolutions( int value )
{
    Q_UNUSED(value)
    comboBoxCameraResolution->clear();
    const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
    for ( int i = 0; i < cameraFormatList.count(); i++ ) {
        if ( cameraFormatList.at(i).pixelFormat() == comboBoxCameraVideoFormat->currentData() ) {
            QString resolution;
            resolution.append( QString::number( cameraFormatList.at(i).resolution().width() ) );
            resolution.append( "x" );
            resolution.append( QString::number( cameraFormatList.at(i).resolution().height() ) );
            if ( comboBoxCameraResolution->findText( resolution ) == -1 ) {
                comboBoxCameraResolution->addItem( resolution, cameraFormatList.at(i).resolution() );
            }
        }
    }
}


// Es muß noch geklärt werden wie man die Zwischenwerte von z.b 5 bis 30 ermittelt und dann setzt
// Diese Funktion dient nur zum üben und hat keine funktion
void QvkCameraSingle::slot_setComboboxCameraFPS( QString value )
{
    Q_UNUSED(value)
    comboBoxCameraFrame->clear();

    QVideoFrameFormat::PixelFormat format;
    format = static_cast<QVideoFrameFormat::PixelFormat>( comboBoxCameraVideoFormat->currentData().toInt() );

    QSize resolution;
    resolution = comboBoxCameraResolution->currentData().toSize();

    for ( int i = 0; i < cameraDevice.videoFormats().count(); i++ ) {
//        qDebug() << cameraDevice.videoFormats().at(i).pixelFormat() << cameraDevice.videoFormats().at(i).resolution() << cameraDevice.videoFormats().at(i).minFrameRate() << cameraDevice.videoFormats().at(i).maxFrameRate();
    }
}


void QvkCameraSingle::slot_toolButtonClicked( bool value )
{
    Q_UNUSED(value)

    if ( toolButton_camera_view_rectangle->objectName().contains( "toolButton_camera_view_rectangle" ) ) {
        if ( toolButton_camera_view_rectangle->isChecked() == true ) {
            toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-on.png" ) );
            toolButton_camera_view_rectangle->setDown( true );
            toolButton_camera_view_ellipse->setDown( false );
            toolButton_camera_view_circle->setDown( false );
        } else {
            toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-off.png" ) );
        }
    }

    if ( toolButton_camera_view_ellipse->objectName().contains( "toolButton_camera_view_ellipse" ) ) {
        if ( toolButton_camera_view_ellipse->isChecked() == true ) {
            toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-on.png" ) );
            toolButton_camera_view_ellipse->setDown( true );
            toolButton_camera_view_rectangle->setDown( false );
            toolButton_camera_view_circle->setDown( false );
        } else {
            toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-off.png" ) );
        }
    }

    if ( toolButton_camera_view_circle->objectName().contains( "toolButton_camera_view_circle" ) ) {
        if ( toolButton_camera_view_circle->isChecked() == true ) {
            toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-on.png" ) );
            toolButton_camera_view_circle->setDown( true );
            toolButton_camera_view_rectangle->setDown( false );
            toolButton_camera_view_ellipse->setDown( false );
        } else {
            toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-off.png" ) );
        }
    }
}


void QvkCameraSingle::slot_checkBoxCameraOnOff( bool value )
{
    // Camera starten
    if ( value == true ) {
        camera = new QCamera( cameraDevice );
        connect( camera, SIGNAL( errorChanged() ), this, SLOT( slot_cameraError() ) );

        // Format und Resolution von Widget ermitteln und anwenden
        const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
        for ( int i = 0; i < cameraFormatList.count(); i++ ) {
            if ( cameraFormatList.at(i).pixelFormat() == comboBoxCameraVideoFormat->currentData() ) {
                if ( cameraFormatList.at(i).resolution() == comboBoxCameraResolution->currentData() ) {
                    camera->setCameraFormat( cameraFormatList.at(i) );
                    qDebug() << cameraFormatList.at(i).pixelFormat() << cameraFormatList.at(i).resolution();
                }
            }
        }

        videoSink = new QVideoSink;
        connect( videoSink, SIGNAL( videoFrameChanged( QVideoFrame ) ), this, SLOT( slot_videoFrameChanged( QVideoFrame ) ) );

        vkCameraWindow->show();

        captureSession = new QMediaCaptureSession;
        captureSession->setCamera( camera );
        captureSession->setVideoOutput( videoSink );

        camera->start();
    }

    // Camera stopen
    if ( value == false ) {
        camera->stop();
        delete camera;
        camera = Q_NULLPTR;

        delete captureSession;

        disconnect( videoSink );
        delete videoSink;

        vkCameraWindow->hide();
    }
}


void QvkCameraSingle::slot_cameraError()
{
    vkCameraWindow->error = camera->errorString();
    vkCameraWindow->repaint();
}


void QvkCameraSingle::slot_cameraWindowFrameOnOff( bool value )
{
    if ( vkCameraWindow->isFullScreen() == false )
    {
        Qt::WindowFlags flags;

#ifdef Q_OS_WIN
        if ( value == true ) {
            cameraWindow->setWindowFlag( Qt::Window, false );
            cameraWindow->setWindowFlag( Qt::ToolTip, true );
        }

        if ( value == false ) {
            cameraWindow->setWindowFlag( Qt::Window, true );
            cameraWindow->setWindowFlag( Qt::ToolTip, false );
        }
#endif

#ifdef Q_OS_LINUX
        if ( value == true ) {
            flags = Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
            vkCameraWindow->setWindowFlags( flags );
        }

        if ( value == false ) {
            flags = Qt::WindowStaysOnTopHint;
            vkCameraWindow->setWindowFlags( flags );
        }
#endif

        if ( checkBoxCameraOnOff->isChecked() == true ) {
            vkCameraWindow->show();
        }
    }
}


void QvkCameraSingle::slot_radioButtonCurrentCameraClicked( bool value )
{
    Q_UNUSED(value)

    // Set text from current camera
    QList<QLabel *> listLabel = ui->centralWidget->findChildren<QLabel *>( "labelCurrentCamera" );
    if ( radioButtonCamera->isChecked() == true ) {
        listLabel.at(0)->setText( checkBoxCameraOnOff->text() );
    }

    // Hide all Toolbutton from other camera
    QList<QToolButton *> listToolButton = ui->centralWidget->findChildren<QToolButton *>();
    if ( listToolButton.empty() == false ) {
        for ( int i = 0; i < listToolButton.count(); i++ ) {
            if ( listToolButton.at(i)->objectName().contains( "toolButton_camera_view_rectangle" ) ) {
                listToolButton.at(i)->hide();
            }
            if ( listToolButton.at(i)->objectName().contains( "toolButton_camera_view_ellipse" ) ) {
                listToolButton.at(i)->hide();
            }
            if ( listToolButton.at(i)->objectName().contains( "toolButton_camera_view_circle" ) ) {
                listToolButton.at(i)->hide();
            }
        }
    }

    // Hide all Checkbox from other camera
    QList<QCheckBox *> listCheckBox = ui->centralWidget->findChildren<QCheckBox *>();
    if ( listCheckBox.empty() == false ) {
        for ( int i = 0; i < listCheckBox.count(); i++ ) {
            if ( listCheckBox.at(i)->objectName().contains( "checkBoxCameraWindowFrame" ) ) {
                listCheckBox.at(i)->hide();
            }
            if ( listCheckBox.at(i)->objectName().contains( "checkBoxCameraMirrorVertical" ) ) {
                listCheckBox.at(i)->hide();
            }
            if ( listCheckBox.at(i)->objectName().contains( "checkBoxCameraMirrorHorizontal" ) ) {
                listCheckBox.at(i)->hide();
            }
            if ( listCheckBox.at(i)->objectName().contains( "checkBoxCameraInvert" ) ) {
                listCheckBox.at(i)->hide();
            }
            if ( listCheckBox.at(i)->objectName().contains( "checkBoxCameraGray" ) ) {
                listCheckBox.at(i)->hide();
            }
            if ( listCheckBox.at(i)->objectName().contains( "checkBoxCameraMono" ) ) {
                listCheckBox.at(i)->hide();
            }
        }
    }

    // Hide all Slider from Other camera
    QList<QvkSpezialSlider *> listSpezialSlider = ui->centralWidget->findChildren<QvkSpezialSlider *>();
    if ( listSpezialSlider.empty() == false ) {
        for ( int i = 0; i < listSpezialSlider.count(); i++ ) {
            if ( listSpezialSlider.at(i)->objectName().contains( "sliderCameraWindowSize" ) ) {
                listSpezialSlider.at(i)->hide();
            }
            if ( listSpezialSlider.at(i)->objectName().contains( "sliderCameraWindowZoom" ) ) {
                listSpezialSlider.at(i)->hide();
            }
        }
    }

    // If the config emmpty the last camera is set
    radioButtonCamera->setChecked( true );

    // Show all Widget from current camera
    sliderCameraWindowZoom->show();
    sliderCameraWindowSize->show();
    toolButton_camera_view_rectangle->show();
    toolButton_camera_view_ellipse->show();
    toolButton_camera_view_circle->show();
    checkBoxCameraWindowFrame->show();
    checkBoxCameraMirrorHorizontal->show();
    checkBoxCameraMirrorVertical->show();
    checkBoxCameraInvert->show();
    checkBoxCameraGray->show();
    checkBoxCameraMono->show();
}


void QvkCameraSingle::slot_videoFrameChanged( QVideoFrame videoFrame )
{
    if ( videoFrame.isValid() == false ) {
        return;
    }

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
        if ( vkCameraWindow->isFullScreen() == false )
        {
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            int w = ui->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            int h = ui->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;
            image = image.scaled( w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
            vkCameraWindow->setFixedSize( image.width(), image.height() );
            vkCameraWindow->setNewImage( image );
            return;
        } else {
            vkCameraWindow->setNewImage( image );
            return;
        }
    }
    // Rectangel end
*/

    // Ellipse
    if ( toolButton_camera_view_ellipse->isDown() == true )
    {
 /*       if ( vkCameraWindow->isFullScreen() == false ) {
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            int w = ui->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            int h = ui->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;
            image = image.scaled( w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        } */
        QPixmap pixmap( image.width(), image.height() );
        pixmap.fill( Qt::transparent );
        QPainter painter;
        painter.begin( &pixmap );
        painter.setRenderHints( QPainter::Antialiasing, true );
        painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
        QPainterPath path;
        path.addEllipse( 0, 0, image.width(), image.height() );
        painter.setClipPath( path );
        painter.drawImage( QPoint( 0, 0 ), image );
        painter.end();
        image = pixmap.toImage();
        if ( vkCameraWindow->isFullScreen() == false ) {
            vkCameraWindow->setFixedSize( image.width(), image.height() );
        }
    }
    // Ellipse end
/*
    // Circle
    if ( toolButton_camera_view_circle->isChecked() == true )
    {
        if ( vkCameraWindow->isFullScreen() == true ){
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

            vkCameraWindow->setFixedSize( h, h );

#endif

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

            vkCameraWindow->setFixedSize( h, h );

#endif
        }
    }
*/
    // Circle end

    vkCameraWindow->setNewImage( image );
}
