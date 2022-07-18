/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "QvkCameraController.h"
#include "QvkCameraResolution.h"
#include "global.h"
#include "camerasettingsdialog.h"

#include <QCameraInfo>
#include <QPainterPath>
#include <QPaintDevice>
#include <QBitmap>

QvkCameraController::QvkCameraController( Ui_formMainWindow *ui_surface ):videoSurface(new QvkVideoSurface())
{
    ui_formMainWindow = ui_surface;

    sliderCameraWindowSize = new QvkSpezialSlider( Qt::Horizontal );
    ui_formMainWindow->horizontalLayout_45->insertWidget( 1, sliderCameraWindowSize );
    sliderCameraWindowSize->setObjectName( "sliderCameraWindowSize" );
    sliderCameraWindowSize->setMinimum( 0 );
    sliderCameraWindowSize->setMaximum( 1 );
    sliderCameraWindowSize->setValue( 0 );
    sliderCameraWindowSize->show();
    sliderCameraWindowSize->setShowValue( false );
    sliderCameraWindowSize->setBigHandel( true );
    sliderCameraWindowSize->setEnabled( true );

    sliderCameraWindowZoom = new QvkSpezialSlider( Qt::Horizontal );
    ui_formMainWindow->horizontalLayout_zoom->insertWidget( 1, sliderCameraWindowZoom );
    sliderCameraWindowZoom->setObjectName( "sliderCameraWindowZoom" );
    sliderCameraWindowZoom->setMinimum( 0 );
    sliderCameraWindowZoom->setMaximum( 1 );
    sliderCameraWindowZoom->setValue( 0 );
    sliderCameraWindowZoom->show();
    sliderCameraWindowZoom->setShowValue( true );
    sliderCameraWindowZoom->setEnabled( true );

    vkCameraSettingsDialog = new cameraSettingsDialog;
    connect( sliderCameraWindowSize, SIGNAL( rangeChanged( int, int ) ), vkCameraSettingsDialog->dialog_sliderCameraWindowSize, SLOT( setRange( int, int ) ) );
    connect( sliderCameraWindowSize, SIGNAL( valueChanged( int ) ),      vkCameraSettingsDialog->dialog_sliderCameraWindowSize, SLOT( setValue( int ) ) );
    connect( sliderCameraWindowZoom, SIGNAL( rangeChanged( int, int ) ), vkCameraSettingsDialog->dialog_sliderCameraWindowZoom, SLOT( setRange( int, int ) ) );
    connect( sliderCameraWindowZoom, SIGNAL( valueChanged( int ) ),      vkCameraSettingsDialog->dialog_sliderCameraWindowZoom, SLOT( setValue( int ) ) );
    connect( vkCameraSettingsDialog->dialog_sliderCameraWindowSize, SIGNAL( valueChanged( int ) ), sliderCameraWindowSize, SLOT( setValue( int ) ) );
    connect( vkCameraSettingsDialog->dialog_sliderCameraWindowZoom, SIGNAL( valueChanged( int ) ), sliderCameraWindowZoom, SLOT( setValue( int ) ) );

    QvkCameraResolution *vkCameraResolution = new QvkCameraResolution( ui_formMainWindow, vkCameraSettingsDialog );
    connect( ui_formMainWindow->comboBoxCamera, SIGNAL( currentIndexChanged( int ) ), vkCameraResolution, SLOT( slot_resolution( int ) ) );
    connect( ui_formMainWindow->comboBoxCameraResolution, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_resolutionChanged() ) );

    getAllDevices();

    cameraWindow = new QvkCameraWindow( ui_surface, vkCameraSettingsDialog );
    connect( cameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), ui_formMainWindow->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
    cameraWindow->setWindowTitle( QString( tr( "Camera") ) );
    cameraWindow->setAttribute( Qt::WA_TranslucentBackground, true );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    cameraWindow->setWindowIcon( icon );

    QvkCameraWatcher *cameraWatcher = new QvkCameraWatcher( ui_formMainWindow );

    connect( cameraWatcher, SIGNAL( signal_addedCamera(   QString, QString ) ), this, SLOT( slot_addedCamera( QString, QString ) ) );
    connect( cameraWatcher, SIGNAL( signal_removedCamera( QString ) ),          this, SLOT( slot_removedCamera( QString ) ) );

    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), ui_formMainWindow->comboBoxCamera, SLOT( setDisabled( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ), this, SLOT( slot_startCamera( bool ) ) );

    connect( videoSurface, SIGNAL( signal_newPicture( QImage ) ), this, SLOT( slot_setNewImage( QImage ) ) );

    connect( ui_formMainWindow->checkBoxCameraWindowFrame, SIGNAL( toggled( bool ) ), this, SLOT( slot_frameOnOff( bool ) ) );

    connect( this, SIGNAL( signal_setNewImage( QImage) ), cameraWindow, SLOT(slot_setNewImage( QImage ) ) );
}


QvkCameraController::~QvkCameraController()
{
}


void QvkCameraController::slot_resolutionChanged()
{
    int minimumSize = 100;
    sliderCameraWindowSize->setMaximum( ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - minimumSize);
    sliderCameraWindowZoom->setMaximum( ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() / 2 );

    if ( ui_formMainWindow->checkBoxCameraOnOff->checkState() == Qt::Checked )
    {

        if ( cameraWindow->isFullScreen() == false )
        {
            ui_formMainWindow->checkBoxCameraOnOff->click();
            ui_formMainWindow->checkBoxCameraOnOff->click();

            if ( vkCameraSettingsDialog->isVisible() == true )
            {
                vkCameraSettingsDialog->close();
                vkCameraSettingsDialog->show();
            }
        } else{
            cameraWindow->showNormal();
            ui_formMainWindow->checkBoxCameraOnOff->click();
            ui_formMainWindow->checkBoxCameraOnOff->click();

            if ( vkCameraSettingsDialog->isVisible() == true )
            {
                vkCameraSettingsDialog->close();
                vkCameraSettingsDialog->show();
            }
            cameraWindow->showFullScreen();
        }
    }
}


void QvkCameraController::getAllDevices()
{
    QList<QCameraInfo> camerasInfoList = QCameraInfo::availableCameras();
    if ( !camerasInfoList.empty() )
    {
        for ( int i = 0; i < camerasInfoList.count(); i++ )
        {
            if ( ( camerasInfoList.at(i).description() > "" ) and ( !camerasInfoList.at(i).description().contains( "@device:pnp" ) ) )
            {
                qDebug().noquote() << global::nameOutput << "[Camera] Found:" << camerasInfoList.at(i).description() << camerasInfoList.at(i).deviceName();
                slot_addedCamera( camerasInfoList.at(i).description(), camerasInfoList.at(i).deviceName() );
            }
        }
    }
}


void QvkCameraController::slot_frameOnOff( bool value )
{
    if ( cameraWindow->isFullScreen() == false )
    {
        Qt::WindowFlags flags;

#ifdef Q_OS_WIN
        if ( value == true )
        {
            flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }

        if ( value == false )
        {
            flags = Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }
#endif

#ifdef Q_OS_LINUX
        if ( value == true )
        {
            flags = Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }

        if ( value == false )
        {
            flags = Qt::WindowStaysOnTopHint;
            cameraWindow->setWindowFlags( flags );
        }
#endif

        if ( ui_formMainWindow->checkBoxCameraOnOff->isChecked() == true )
        {
            cameraWindow->show();
        }
    }
}


void QvkCameraController::slot_sliderMoved( int value )
{
    Q_UNUSED(value)
}


void QvkCameraController::slot_setNewImage( QImage image )
{
#ifdef Q_OS_LINUX
    if ( ui_formMainWindow->checkBoxCameraMirrorHorizontal->isChecked() == true )
        image = image.mirrored( true, false );

    if ( ui_formMainWindow->checkBoxCameraMirrorVertical->isChecked() == true )
        image = image.mirrored( false, true );
#endif

#ifdef Q_OS_WIN
    if ( ui_formMainWindow->checkBoxCameraMirrorHorizontal->isChecked() == true )
        image = image.mirrored( true, false );

    if ( ui_formMainWindow->checkBoxCameraMirrorVertical->isChecked() == false )
        image = image.mirrored( false, true );
#endif

    if ( ui_formMainWindow->checkBoxCameraInvert->isChecked() == true )
        image.invertPixels( QImage::InvertRgb );

    if ( ui_formMainWindow->checkBoxCameraGray->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Grayscale8 );

    if ( ui_formMainWindow->checkBoxCameraMono->isChecked() == true )
        image = image.convertToFormat( QImage::Format_Mono );


    // Rectangel
    if ( ui_formMainWindow->toolButton_camera_view_rectangle->isChecked() == true )
    {
        // Zoom
        qreal width = image.width();
        qreal height = image.height();
        qreal quotient = width / height;
        int minusPixel = sliderCameraWindowZoom->value();
        QImage image_zoom = image.copy( minusPixel,
                                        minusPixel / quotient,
                                        width - ( 2 * minusPixel ),
                                        height - ( 2 * minusPixel / quotient )
                                        );
        image = image_zoom.scaled( width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        // Zoom end

        int w = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
        int h = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value();
        image = image.scaled( w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        cameraWindow->setFixedSize( image.width(), image.height() );
    }
    // Rectangel end


    // Ellipse
    if ( ui_formMainWindow->toolButton_camera_view_ellipse->isChecked() == true )
    {
        // Zoom
        qreal width = image.width();
        qreal height = image.height();
        qreal quotient = width / height;
        int minusPixel = sliderCameraWindowZoom->value();
        QImage image_zoom = image.copy( minusPixel,
                                        minusPixel / quotient,
                                        width - ( 2 * minusPixel ),
                                        height - ( 2 * minusPixel / quotient )
                                        );
        image = image_zoom.scaled( width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        // Zoom end

        int w = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
        int h = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value();
        image = image.scaled( w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation );

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
        cameraWindow->setFixedSize( image.width(), image.height() );
    }


    // Circle
    if ( ui_formMainWindow->toolButton_camera_view_circle->isChecked() == true )
    {
        // Zoom
        qreal width = image.width();
        qreal height = image.height();
        qreal quotient = width / height;
        int minusPixel = sliderCameraWindowZoom->value();
        QImage image_zoom = image.copy( minusPixel,
                                        minusPixel / quotient,
                                        width - ( 2 * minusPixel ),
                                        height - ( 2 * minusPixel / quotient )
                                        );
        image = image_zoom.scaled( width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation );
        // Zoom end

        qreal w = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
        qreal h = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value();
        QPixmap pixmap( h, h );
        pixmap.fill( Qt::transparent );
        QPainter painter;
        painter.begin( &pixmap );
        painter.setRenderHints( QPainter::Antialiasing, true );
        QPainterPath path;
        path.addEllipse( 0, 0, h, h );
        painter.setClipPath( path );
        QRectF target( 0.0, 0.0, h, h );
        QRectF source( (w-h)/2, 0.0, image.height(),image.height() );
        painter.drawImage( target, image, source );
        painter.end();
        image = pixmap.toImage();
        cameraWindow->setFixedSize( h, h );
    }
    // Circle end

    emit signal_setNewImage( image );
}


void QvkCameraController::slot_addedCamera( QString description, QString device )
{
    ui_formMainWindow->checkBoxCameraOnOff->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->setEnabled( true );
    ui_formMainWindow->comboBoxCameraResolution->setEnabled( true );
    ui_formMainWindow->comboBoxCamera->addItem( description, device.toLatin1() );
    ui_formMainWindow->checkBoxCameraGray->setEnabled( true );
    ui_formMainWindow->checkBoxCameraInvert->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMirrorHorizontal->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMirrorVertical->setEnabled( true );
    ui_formMainWindow->checkBoxCameraMono->setEnabled( true );
    sliderCameraWindowSize->setEnabled( true );
    sliderCameraWindowZoom->setEnabled( true );

    ui_formMainWindow->checkBoxCameraWindowFrame->setEnabled( true );
}


void QvkCameraController::slot_removedCamera( QString device )
{
    if ( ( ui_formMainWindow->checkBoxCameraOnOff->isChecked() == true ) and ( ui_formMainWindow->comboBoxCamera->currentData().toString() == device ) )
    {
        cameraWindow->close();
    }

    int x = ui_formMainWindow->comboBoxCamera->findData( device.toLatin1() );
    ui_formMainWindow->comboBoxCamera->removeItem( x );

    if ( ui_formMainWindow->comboBoxCamera->count() == 0 )
    {
        ui_formMainWindow->checkBoxCameraOnOff->setEnabled( false );
        ui_formMainWindow->comboBoxCamera->setEnabled( false );
        ui_formMainWindow->comboBoxCameraResolution->clear();
        ui_formMainWindow->comboBoxCameraResolution->setEnabled( false );
        ui_formMainWindow->checkBoxCameraGray->setEnabled( false );
        ui_formMainWindow->checkBoxCameraInvert->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMirrorHorizontal->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMirrorVertical->setEnabled( false );
        ui_formMainWindow->checkBoxCameraMono->setEnabled( false );
        sliderCameraWindowSize->setEnabled( false );
        sliderCameraWindowZoom->setEnabled( false );

        ui_formMainWindow->checkBoxCameraWindowFrame->setEnabled( false );
    }
}


void QvkCameraController::slot_startCamera( bool value )
{
    if ( value == true )
    {
        QByteArray device = ui_formMainWindow->comboBoxCamera->currentData().toByteArray();
        delete camera;
        camera = new QCamera( device );
        camera->setCaptureMode( QCamera::CaptureViewfinder );
        connect( camera, SIGNAL( statusChanged( QCamera::Status ) ), this, SLOT( slot_statusChanged( QCamera::Status ) ) );
        connect( camera, SIGNAL( stateChanged( QCamera::State   ) ), this, SLOT( slot_stateChanged( QCamera::State ) )  );
        connect( camera, QOverload<QCamera::Error>::of( &QCamera::error ), this, [=]( QCamera::Error value ){ slot_error( value ); });

        int frameWidth = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt();
        int frameHeight = ui_formMainWindow->comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt();
        QCameraViewfinderSettings viewfinderSettings;
        viewfinderSettings.setResolution( frameWidth, frameHeight );
        viewfinderSettings.setMinimumFrameRate( 0.0 );
        viewfinderSettings.setMaximumFrameRate( 0.0 );
        camera->setViewfinderSettings( viewfinderSettings );

        camera->setViewfinder( videoSurface );
        cameraWindow->setStyleSheet( "background-color:black;" );
        cameraWindow->show();
#ifdef Q_OS_LINUX
        camera->load();
#endif
#ifdef Q_OS_WIN
        camera->start();
#endif
    }
    else
    {
        disconnect( camera, nullptr, nullptr, nullptr );
        camera->stop();
        camera->unload();
        cameraWindow->close();
    }
}


void QvkCameraController::slot_statusChanged( QCamera::Status status )
{
    switch ( status )
    {
    case QCamera::UnavailableStatus : { qDebug().noquote() << global::nameOutput << status; break; }// 0
    case QCamera::UnloadedStatus    : { qDebug().noquote() << global::nameOutput << status; break; }// 1
    case QCamera::LoadingStatus     : { qDebug().noquote() << global::nameOutput << status; break; }// 2
    case QCamera::UnloadingStatus   : { qDebug().noquote() << global::nameOutput << status; break; }// 3
    case QCamera::LoadedStatus      : { qDebug().noquote() << global::nameOutput << status;
#ifdef Q_OS_LINUX
        camera->start();
#endif
        break;
    }// 4
    case QCamera::StandbyStatus     : { qDebug().noquote() << global::nameOutput << status; break; }// 5
    case QCamera::StartingStatus    : { qDebug().noquote() << global::nameOutput << status; break; }// 6
    case QCamera::StoppingStatus    : { qDebug().noquote() << global::nameOutput << status; break; }// 7
    case QCamera::ActiveStatus      : { qDebug().noquote() << global::nameOutput << status; break; }// 8
    }
}


void QvkCameraController::slot_stateChanged( QCamera::State state )
{
    switch ( state )
    {
    case QCamera::UnloadedState : { qDebug().noquote() << global::nameOutput << state; break;  }// 0
    case QCamera::LoadedState   : { qDebug().noquote() << global::nameOutput << state; break;  }// 1
    case QCamera::ActiveState   : { qDebug().noquote() << global::nameOutput << state; break;  }// 2
    }
}


void QvkCameraController::slot_error( QCamera::Error error )
{
    switch ( error )
    {
    case QCamera::NoError:
    {
        break;
    }
    case QCamera::CameraError:
    {
        qDebug().noquote() << global::nameOutput << "General Camera error";
        //        cameraWindow->clear();
        cameraWindow->setStyleSheet( "background-color:white;" );
        //        cameraWindow->setText( "Camera is busy " );
        break;
    }
    case QCamera::InvalidRequestError:
    {
        qDebug().noquote() << global::nameOutput << "Camera invalid request error";
        break;
    }
    case QCamera::ServiceMissingError:
    {
        qDebug().noquote() << global::nameOutput << "Camera service missing error";
        break;
    }
    case QCamera::NotSupportedFeatureError :
    {
        qDebug().noquote() << global::nameOutput << "Camera not supported error";
        break;
    }
    }
}
