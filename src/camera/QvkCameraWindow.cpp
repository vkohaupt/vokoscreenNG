/* vokoscreen - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "QvkCameraWindow.h"

#include <QDebug>
#include <QMouseEvent>

QvkCameraWindow::QvkCameraWindow( Ui_formMainWindow *ui_surface )
{
    ui_formMainWindow = ui_surface;

    setWindowFlags( Qt::WindowStaysOnTopHint );
    setStyleSheet( "background-color:black;" );
    setMinimumSize( QSize( 160, 120 ) );
    setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    setMouseTracking( true ); // No function, why?

    vkCameraSettingsDialog = new cameraSettingsDialog;

    connect( this, SIGNAL( signal_cameraWindow_close( bool ) ), vkCameraSettingsDialog, SLOT( close() ) );

    connect( ui_formMainWindow->checkBoxCameraMirrorHorizontal, SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMirrorHorizontal, SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraMirrorVertical,   SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMirrorVertical,   SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraInvert,           SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraInvert,           SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraGray,             SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraGray,             SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraMono,             SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraMono,             SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->checkBoxCameraWindowFrame,      SIGNAL( toggled( bool ) ), vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame,      SLOT( setChecked( bool ) ) );
    connect( ui_formMainWindow->sliderCameraWindowSize,         SIGNAL( valueChanged( int ) ), vkCameraSettingsDialog->ui->sliderCameraWindowSize,     SLOT( setValue( int ) ) );

    connect( vkCameraSettingsDialog->ui->checkBoxCameraMirrorHorizontal, SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraMirrorHorizontal, SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraMirrorVertical,   SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraMirrorVertical,   SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraInvert,           SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraInvert,           SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraGray,             SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraGray,             SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraMono,             SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraMono,             SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame,      SIGNAL( toggled( bool ) ), ui_formMainWindow->checkBoxCameraWindowFrame,      SLOT( setChecked( bool ) ) );
    connect( vkCameraSettingsDialog->ui->sliderCameraWindowSize,         SIGNAL( valueChanged( int ) ), ui_formMainWindow->sliderCameraWindowSize,     SLOT( setValue( int ) ) );

    connect( vkCameraSettingsDialog->ui->buttonBox, SIGNAL( accepted() ), vkCameraSettingsDialog, SLOT( close() ) );
    connect( vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen, SIGNAL( clicked( bool ) ), this, SLOT( slot_switchToFullscreen() ) );
}


QvkCameraWindow::~QvkCameraWindow()
{
}


void QvkCameraWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_cameraWindow_close( false );
}


void QvkCameraWindow::slot_switchToFullscreen()
{
    if ( isFullScreen() == true )
    {
        showNormal();
        vkCameraSettingsDialog->close();
    }
    else
    {
        setWindowState( Qt::WindowFullScreen );
        vkCameraSettingsDialog->close();
    }
}


void QvkCameraWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if ( isFullScreen() == true )
    {
        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Window" ) );
    }
    else
    {
        vkCameraSettingsDialog->ui->pushButtonSwitchToFullscreen->setText( tr( "Switch to Fullscreen" ) );
    }
}


void QvkCameraWindow::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
            vkCameraSettingsDialog->close();
        }
    }
}

void QvkCameraWindow::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape )
    {
        showNormal();
    }

    if ( event->key() == Qt::Key_F11 )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
        }
    }
}

void QvkCameraWindow::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::RightButton )
    {
       if ( vkCameraSettingsDialog->isVisible() )
       {
           vkCameraSettingsDialog->close();
       }
       else
       {
           vkCameraSettingsDialog->move( event->globalX() + 30, event->globalY() + 30 );
           vkCameraSettingsDialog->show();
           if ( isFullScreen() == true )
           {
               vkCameraSettingsDialog->ui->widgetCameraWindowSize->hide();
               vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->hide();
               vkCameraSettingsDialog->ui->line_2->hide();
               vkCameraSettingsDialog->ui->line_3->hide();
           }
           else
           {
               vkCameraSettingsDialog->ui->widgetCameraWindowSize->show();
               vkCameraSettingsDialog->ui->checkBoxCameraWindowFrame->show();
               vkCameraSettingsDialog->ui->line_2->show();
               vkCameraSettingsDialog->ui->line_3->show();
           }
       }
       return;
    }

    if ( isFullScreen() == true )
    {
        vkCameraSettingsDialog->close();
    }
}
