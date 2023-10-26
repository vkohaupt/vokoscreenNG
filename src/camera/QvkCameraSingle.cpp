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
#include <QIcon>
#include <QPainterPath>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRectF>

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

    labelCameraWindowSize = new QLabel;
    ui->horizontalLayout_66->addWidget( labelCameraWindowSize );
    labelCameraWindowSize->setObjectName( "labelCameraWindowSize-" + QString::number( counter ) );
    labelCameraWindowSize->setText( "---x---" );

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
    connect( checkBoxCameraOnOff, SIGNAL( clicked( bool ) ), this, SLOT( slot_checkBoxCameraOnOff( bool ) ) );
    layoutCamera->setStretchFactor( checkBoxCameraOnOff, 2 );

    comboBoxCameraVideoFormat = new QComboBox;
    comboBoxCameraVideoFormat->setMinimumWidth( 100 );
    comboBoxCameraVideoFormat->setObjectName( "comboBoxCameraVideoFormat-" + QString::number( counter ) );
    connect( comboBoxCameraVideoFormat, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_comboboxCameraResolutionsInsertValues( int ) ) ); // Wird im Slot dosconnected
    connect( comboBoxCameraVideoFormat, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_comboboxCameraFormatCurrentIndexChanged( int ) ) );
    layoutCamera->addWidget( comboBoxCameraVideoFormat );

    comboBoxCameraResolution = new QComboBox;
    comboBoxCameraResolution->setMinimumWidth( 100 );
    comboBoxCameraResolution->setObjectName( "comboBoxCameraResolution-" + QString::number( counter ) );
    connect( comboBoxCameraResolution,  SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_comboboxCameraResolutionsCurrentIndexChanged( int ) ) );
    layoutCamera->addWidget( comboBoxCameraResolution );

    toolButton_camera_view_rectangle = new QToolButton;
    toolButton_camera_view_rectangle->setObjectName( "toolButton_camera_view_rectangle-" + QString::number( counter ) );
    toolButton_camera_view_rectangle->setCheckable( true );
    toolButton_camera_view_rectangle->setChecked( true );
    toolButton_camera_view_rectangle->setAutoExclusive( true );
    toolButton_camera_view_rectangle->setAutoRaise( true );
    toolButton_camera_view_rectangle->setIconSize( QSize( 34, 34 ) );
    toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-off.png" ) );
    connect( toolButton_camera_view_rectangle, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonRectangleClicked( bool ) ) );

    toolButton_camera_view_ellipse = new QToolButton;
    toolButton_camera_view_ellipse->setObjectName( "toolButton_camera_view_ellipse-" + QString::number( counter ) );
    toolButton_camera_view_ellipse->setCheckable( true );
    toolButton_camera_view_ellipse->setAutoExclusive( true );
    toolButton_camera_view_ellipse->setAutoRaise( true );
    toolButton_camera_view_ellipse->setIconSize( QSize( 34, 34 ) );
    toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-off.png" ) );
    connect( toolButton_camera_view_ellipse, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonEllipseClicked( bool ) ) );

    toolButton_camera_view_circle = new QToolButton;
    toolButton_camera_view_circle->setObjectName( "toolButton_camera_view_circle-" + QString::number( counter ) );
    toolButton_camera_view_circle->setCheckable( true );
    toolButton_camera_view_circle->setAutoExclusive( true );
    toolButton_camera_view_circle->setAutoRaise( true );
    toolButton_camera_view_circle->setIconSize( QSize( 34, 34 ) );
    toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-off.png" ) );
    connect( toolButton_camera_view_circle, SIGNAL( clicked( bool ) ), this, SLOT( slot_toolButtonCircleClicked( bool ) ) );

    // Widget zur aufnahme der Toolbutton Rectangle, Ellipse und Circle
    widgetToolButton = new QWidget;
    widgetToolButton->setObjectName( "widgetToolButton-" + QString::number( counter ) );
    ui->horizontalLayout_65->addWidget( widgetToolButton );
    widgetToolButton->show();
    QHBoxLayout *widgetLayout = new QHBoxLayout( widgetToolButton );
    QSpacerItem *spacerItem_1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding );
    widgetLayout->addSpacerItem( spacerItem_1 );
    widgetLayout->addWidget( toolButton_camera_view_rectangle );
    QSpacerItem *spacerItem_2 = new QSpacerItem( 40, 20, QSizePolicy::Preferred );
    widgetLayout->addSpacerItem( spacerItem_2 );
    widgetLayout->addWidget( toolButton_camera_view_ellipse );
    QSpacerItem *spacerItem_3 = new QSpacerItem( 40, 20, QSizePolicy::Preferred );
    widgetLayout->addSpacerItem( spacerItem_3 );
    widgetLayout->addWidget( toolButton_camera_view_circle );
    QSpacerItem *spacerItem_4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding );
    widgetLayout->addSpacerItem( spacerItem_4 );
    // End

    checkBoxCameraWindowFrame = new QCheckBox;
    checkBoxCameraWindowFrame->setText( tr( "Remove window frame" ) );
    checkBoxCameraWindowFrame->setObjectName( "checkBoxCameraWindowFrame-" + QString::number( counter ) );
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

    vkCameraWindow = new QvkCameraWindow( ui, checkBoxCameraWindowFrame );
    vkCameraWindow->setObjectName( "vkCameraWindow-" + QString::number( counter ));
    connect( vkCameraWindow, SIGNAL( signal_cameraWindow_close( bool ) ), checkBoxCameraOnOff, SLOT( click() ) );
    connect( checkBoxCameraWindowFrame, SIGNAL( clicked( bool ) ), this, SLOT( slot_cameraWindowFrameOnOff( bool ) ) );

    vkCameraSettingsDialog = new cameraSettingsDialog;
    connect( sliderCameraWindowSize, SIGNAL( rangeChanged( int, int ) ), vkCameraSettingsDialog->dialog_sliderCameraWindowSize, SLOT( setRange( int, int ) ) );
    connect( sliderCameraWindowSize, SIGNAL( valueChanged( int ) ),      vkCameraSettingsDialog->dialog_sliderCameraWindowSize, SLOT( setValue( int ) ) );
    connect( sliderCameraWindowZoom, SIGNAL( rangeChanged( int, int ) ), vkCameraSettingsDialog->dialog_sliderCameraWindowZoom, SLOT( setRange( int, int ) ) );
    connect( sliderCameraWindowZoom, SIGNAL( valueChanged( int ) ),      vkCameraSettingsDialog->dialog_sliderCameraWindowZoom, SLOT( setValue( int ) ) );
    connect( vkCameraSettingsDialog->dialog_sliderCameraWindowSize, SIGNAL( valueChanged( int ) ), sliderCameraWindowSize, SLOT( setValue( int ) ) );
    connect( vkCameraSettingsDialog->dialog_sliderCameraWindowZoom, SIGNAL( valueChanged( int ) ), sliderCameraWindowZoom, SLOT( setValue( int ) ) );

    connect( checkBoxCameraMirrorHorizontal, SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMirrorHorizontal, SLOT( setChecked( bool ) ) );
    connect( checkBoxCameraMirrorVertical,   SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMirrorVertical,   SLOT( setChecked( bool ) ) );
    connect( checkBoxCameraInvert,           SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraInvert,           SLOT( setChecked( bool ) ) );
    connect( checkBoxCameraGray,             SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraGray,             SLOT( setChecked( bool ) ) );
    connect( checkBoxCameraMono,             SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMono,             SLOT( setChecked( bool ) ) );
    connect( checkBoxCameraWindowFrame,      SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame,      SLOT( setChecked( bool ) ) );
    connect( toolButton_camera_view_rectangle, SIGNAL( clicked( bool ) ), vkCameraSettingsDialog->ui->toolButton_camera_view_rectangle, SLOT( setChecked( bool )) );
    connect( toolButton_camera_view_ellipse,   SIGNAL( clicked( bool ) ), vkCameraSettingsDialog->ui->toolButton_camera_view_ellipse,   SLOT( setChecked( bool ) ) );
    connect( toolButton_camera_view_circle,    SIGNAL( clicked( bool ) ), vkCameraSettingsDialog->ui->toolButton_camera_view_circle,    SLOT( setChecked( bool ) ) );

    connect( vkCameraSettingsDialog->ui->checkBoxCameraMirrorHorizontal, SIGNAL( toggled( bool ) ), checkBoxCameraMirrorHorizontal, SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraMirrorVertical,   SIGNAL( toggled( bool ) ), checkBoxCameraMirrorVertical,   SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraInvert,           SIGNAL( toggled( bool ) ), checkBoxCameraInvert,           SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraGray,             SIGNAL( toggled( bool ) ), checkBoxCameraGray,             SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraMono,             SIGNAL( toggled( bool ) ), checkBoxCameraMono,             SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame,      SIGNAL( toggled( bool ) ), checkBoxCameraWindowFrame,      SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->comboBoxCameraResolution,  SIGNAL( currentIndexChanged( int ) ), comboBoxCameraResolution, SLOT( setCurrentIndex( int ) ) );
    connect( vkCameraSettingsDialog->ui->toolButton_camera_view_rectangle, SIGNAL( clicked( bool ) ), toolButton_camera_view_rectangle, SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->toolButton_camera_view_ellipse,   SIGNAL( clicked( bool ) ), toolButton_camera_view_ellipse,   SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->toolButton_camera_view_circle,    SIGNAL( clicked( bool ) ), toolButton_camera_view_circle,    SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->buttonBox,                       SIGNAL( accepted() ),       vkCameraSettingsDialog,           SLOT( close() ) );
    connect( vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen,    SIGNAL( clicked( bool ) ),  this,                             SLOT( slot_switchToFullscreen() ) );
    connect( vkCameraSettingsDialog->ui->comboBoxCameraVideoFormat,       SIGNAL( currentIndexChanged( int ) ), comboBoxCameraVideoFormat, SLOT( setCurrentIndex( int ) ) );

    connect( comboBoxCameraVideoFormat, SIGNAL( currentIndexChanged( int ) ), vkCameraSettingsDialog->ui->comboBoxCameraVideoFormat, SLOT( setCurrentIndex( int ) ) );

    connect( vkCameraWindow, SIGNAL( signal_mousePressEvent( QMouseEvent* ) ),        this,                  SLOT( slot_vkCameraSettingsDialogShow()) );
    connect( vkCameraWindow, SIGNAL( signal_changelabelCameraWindowSize( QString ) ), labelCameraWindowSize, SLOT( setText( QString) ) );
    connect( vkCameraWindow, SIGNAL( signal_changelabelCameraWindowSize( QString ) ), vkCameraSettingsDialog->ui->labelCameraWindowSize, SLOT( setText( QString) ) );

    for ( int i = 0; i < cameraDevice.videoFormats().count(); i++ ) {
        QString format = QVideoFrameFormat::pixelFormatToString( cameraDevice.videoFormats().at(i).pixelFormat() ).toUpper();
        if ( comboBoxCameraVideoFormat->findText( format ) == -1 ) {
            comboBoxCameraVideoFormat->addItem( format, cameraDevice.videoFormats().at(i).pixelFormat() ); // bei der Philips wird der Wert 17 für YUYV eingetragen und für JPEG der Wert 29
            vkCameraSettingsDialog->ui->comboBoxCameraVideoFormat->addItem( format, cameraDevice.videoFormats().at(i).pixelFormat() );
        }
    }


    slot_radioButtonCurrentCameraClicked( false );
}


QvkCameraSingle::~QvkCameraSingle()
{
}


void QvkCameraSingle::slot_cameraWindowFrameOnOff( bool value )
{
    if ( vkCameraWindow->isFullScreen() == false ) {
        Qt::WindowFlags flags;

// 1. Automatischer Start des Kamerafenster nach Start von vokoscreenNG im Vordergrund. OK
// 2. Fenster nach umschalten mittels frameless im Vordergrund. OK
// 3. Fenster beim einschalten der Kamera im Vordergrund. OK
// 4. Frameless Fenster wandert nach Neustart von vokoscreenNG nicht nach oben oder unten. Fehler------------------------
// 5. Umschalten von frameless in den window mode und umgekehrt kein gezappel. Fehler----------------------
#ifdef Q_OS_WIN
        if ( value == true ) {
            vkCameraWindow->setWindowFlag( Qt::FramelessWindowHint, true );
            qDebug().noquote() << global::nameOutput << "[Camera] Set window frameless";
        }

        if ( value == false ) {
            vkCameraWindow->setWindowFlag( Qt::FramelessWindowHint, false );
            qDebug().noquote() << global::nameOutput << "[Camera] Set window frame";
        }
#endif

// 1. Automatischer Start des Kamerafenster nach Start von vokoscreenNG im Vordergrund. OK
// 2. Fenster nach umschalten mittels frameless im Vordergrund. OK
// 3. Fenster beim einschalten der Kamera im Vordergrund. OK
// 4. Frameless Fenster wandert nach Neustart von vokoscreenNG nicht nach oben oder unten. OK
// 5. Fenster wandert nach Neustart von vokoscreenNG nicht nach oben oder unten. Fehler--------------------------
// 6. Umschalten von frameless in den window mode und umgekehrt kein gezappel. OK
#ifdef Q_OS_LINUX
// On Linux we need no more windowflags.
// In QvkCameraWindow::paintEvent() we set setMask() and all works fine.
#endif

        if ( checkBoxCameraOnOff->isChecked() == true ) {
            vkCameraWindow->show();
        }
    }
}


void QvkCameraSingle::slot_vkCameraSettingsDialogShow()
{
    vkCameraSettingsDialog->show();
    if ( vkCameraWindow->isFullScreen() == true ) {
        vkCameraSettingsDialog->ui->widgetCameraWindowSize->hide();
        vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->hide();
        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Window" ) );
    } else {
        vkCameraSettingsDialog->ui->widgetCameraWindowSize->show();
        vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->show();
        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Fullscreen" ) );
    }
}


void QvkCameraSingle::slot_switchToFullscreen()
{
    if ( vkCameraWindow->isFullScreen() == true ) {
        vkCameraSettingsDialog->close();
        vkCameraWindow->showNormal();
    } else {
        vkCameraSettingsDialog->close();
        vkCameraWindow->setWindowState( Qt::WindowFullScreen );
    }
}


void QvkCameraSingle::slot_comboboxCameraFormatCurrentIndexChanged( int value )
{
    Q_UNUSED(value)
    if ( camera != Q_NULLPTR ) {
        slot_checkBoxCameraOnOff( false );
        slot_checkBoxCameraOnOff( true );
    }
}


void QvkCameraSingle::slot_comboboxCameraResolutionsCurrentIndexChanged( int value )
{
    Q_UNUSED(value)

    sliderCameraWindowSize->setMaximum( comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - 100 );
    sliderCameraWindowZoom->setMaximum( comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() / 2 );

    if ( camera != Q_NULLPTR ) {
            slot_checkBoxCameraOnOff( false );
            slot_checkBoxCameraOnOff( true );
    }
}



void QvkCameraSingle::slot_comboboxCameraResolutionsInsertValues( int value )
{
    Q_UNUSED(value)

    disconnect( comboBoxCameraResolution, nullptr, nullptr, nullptr );

    comboBoxCameraResolution->clear();
    vkCameraSettingsDialog->ui->comboBoxCameraResolution->clear();
    const QList<QCameraFormat> cameraFormatList = cameraDevice.videoFormats();
    for ( int i = 0; i < cameraFormatList.count(); i++ ) {
        if ( cameraFormatList.at(i).pixelFormat() == comboBoxCameraVideoFormat->currentData() ) {
            QString resolution;
            resolution.append( QString::number( cameraFormatList.at(i).resolution().width() ) );
            resolution.append( "x" );
            resolution.append( QString::number( cameraFormatList.at(i).resolution().height() ) );
            if ( comboBoxCameraResolution->findText( resolution ) == -1 ) {
                comboBoxCameraResolution->addItem( resolution, cameraFormatList.at(i).resolution() );
                vkCameraSettingsDialog->ui->comboBoxCameraResolution->addItem( resolution, cameraDevice.videoFormats().at(i).pixelFormat() );
            }
        }
    }

    sliderCameraWindowSize->setMaximum( comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - 100 );
    sliderCameraWindowZoom->setMaximum( comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() / 2 );
    vkCameraSettingsDialog->dialog_sliderCameraWindowSize->setMaximum( comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - 100 );
    vkCameraSettingsDialog->dialog_sliderCameraWindowZoom->setMaximum( comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() / 2 );

    connect( comboBoxCameraResolution, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_comboboxCameraResolutionsCurrentIndexChanged( int ) ) );
    connect( comboBoxCameraResolution,  SIGNAL( currentIndexChanged( int ) ), vkCameraSettingsDialog->ui->comboBoxCameraResolution,  SLOT( setCurrentIndex( int ) ) );
}


void QvkCameraSingle::slot_toolButtonRectangleClicked( bool value )
{
    Q_UNUSED(value)

    toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-on.png" ) );
    toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-off.png" ) );
    toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-off.png" ) );
}


void QvkCameraSingle::slot_toolButtonEllipseClicked( bool value )
{
    Q_UNUSED(value)
    toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-on.png" ) );
    toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-off.png" ) );
    toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-off.png" ) );
}


void QvkCameraSingle::slot_toolButtonCircleClicked( bool value )
{
    Q_UNUSED(value)
    toolButton_camera_view_circle->setIcon( QIcon( ":/pictures/camera/window-circle-on.png" ) );
    toolButton_camera_view_rectangle->setIcon( QIcon( ":/pictures/camera/window-rectangle-off.png" ) );
    toolButton_camera_view_ellipse->setIcon( QIcon( ":/pictures/camera/window-ellipse-off.png" ) );
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
                    QString width  = QString::number( cameraFormatList.at(i).resolution().width() );
                    QString height = QString::number( cameraFormatList.at(i).resolution().height() );
                    qDebug().noquote() << global::nameOutput
                                       << "[Camera] Start with format:"
                                       << cameraFormatList.at(i).pixelFormat()
                                       << "and resolution:"
                                       << width + "x" + height;
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
        disconnect( videoSink );
        delete videoSink;

        camera->stop();
        delete camera;
        camera = Q_NULLPTR;

        delete captureSession;

        vkCameraWindow->hide();
        qDebug().noquote() << global::nameOutput << "[Camera] Stop";
    }
}


void QvkCameraSingle::slot_cameraError()
{
    vkCameraWindow->error = camera->errorString();
    vkCameraWindow->repaint();
}


void QvkCameraSingle::slot_radioButtonCurrentCameraClicked( bool value )
{
    Q_UNUSED(value)

    // Set text from current camera
    QList<QLabel *> listLabel = ui->centralWidget->findChildren<QLabel *>( "labelCurrentCamera" );
    if ( radioButtonCamera->isChecked() == true ) {
        listLabel.at(0)->setText( checkBoxCameraOnOff->text() );
    }

     // Hide widgetToolButton from other camera
    QList<QWidget *> listWidget = ui->centralWidget->findChildren<QWidget *>();
    if ( listWidget.empty() == false ) {
        for ( int i = 0; i < listWidget.count(); i++ ) {
            if ( listWidget.at(i)->objectName().contains( "widgetToolButton-" ) ) {
                listWidget.at(i)->hide();
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

    QList<QLabel *> listLabelCameraWindowSize = ui->centralWidget->findChildren<QLabel *>();
    if ( listLabelCameraWindowSize.empty() == false ) {
        for ( int i = 0; i < listLabelCameraWindowSize.count(); i++ ) {
            if ( listLabelCameraWindowSize.at(i)->objectName().contains( "labelCameraWindowSize" ) ) {
                listLabelCameraWindowSize.at(i)->hide();
            }
        }
    }

    // If the config emmpty the last camera is set
    radioButtonCamera->setChecked( true );

    // Show all Widget from current camera
    sliderCameraWindowZoom->show();
    sliderCameraWindowSize->show();
    labelCameraWindowSize->show();
    widgetToolButton->show();
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
    QImage image = videoFrame.toImage();
    image = image.convertedTo( QImage::Format_ARGB32 );

    if ( checkBoxCameraMirrorHorizontal->isChecked() == true ) {
        image = image.mirrored( true, false );
    }

    if ( checkBoxCameraMirrorVertical->isChecked() == false ) {
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


    // Zoom
    if ( sliderCameraWindowZoom->value() > 0 ) {
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
    if ( toolButton_camera_view_rectangle->isChecked() == true ) {
        if ( vkCameraWindow->isFullScreen() == false ) {
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            int w = comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            int h = comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;
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


    // Ellipse
    if ( toolButton_camera_view_ellipse->isChecked() == true ) {
        if ( vkCameraWindow->isFullScreen() == false ) {
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            int w = comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            int h = comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;
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
        if ( vkCameraWindow->isFullScreen() == false ) {
            vkCameraWindow->setFixedSize( image.width(), image.height() );
        }
    }


    // Circle
    if ( toolButton_camera_view_circle->isChecked() == true ) {
        if ( vkCameraWindow->isFullScreen() == true ) {
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
            qreal width = image.width();
            qreal height = image.height();
            qreal quotient = width / height;
            qreal w = comboBoxCameraResolution->currentText().section( "x", 0, 0 ).toInt() - sliderCameraWindowSize->value();
            qreal h = comboBoxCameraResolution->currentText().section( "x", 1, 1 ).toInt() - sliderCameraWindowSize->value() / quotient;

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
        }
    }
    // Circle end

    vkCameraWindow->setNewImage( image );
}
