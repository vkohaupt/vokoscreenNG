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

#include "QvkSystray_wl.h"
#include "global.h"
#include "QvkSpezialCheckbox.h"

#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QIcon>

QvkSystray_wl::QvkSystray_wl(Ui_formMainWindow_wl *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkSystray_wl::~QvkSystray_wl()
{
}


void QvkSystray_wl::init()
{
    QAction *titleAction = new QAction( this );
    titleAction->setIcon( QIcon( ":pictures/systray/systray.png" ) );
    titleAction->setText( global::name + " " + global::version );
    titleAction->setEnabled( false );

    startAction = new QAction( this );
    startAction->setIcon( QIcon( ":pictures/player/start.png" ) );
    startAction->setData( "Start" );

    stopAction = new QAction( this );
    stopAction->setIcon( QIcon( ":pictures/player/stop.png" ) );
    stopAction->setData( "Stop" );
    stopAction->setEnabled( false );

    pauseAction = new QAction( this );
    pauseAction->setIcon( QIcon( ":pictures/player/pause.png" ) );
    pauseAction->setData( "Pause" );
    pauseAction->setEnabled( false );

    continueAction = new QAction( this );
    continueAction->setIcon( QIcon( ":pictures/player/start.png" ) );
    continueAction->setData( "Continue" );
    continueAction->setEnabled( false );
/*
    cameraAction = new QAction( this );
    cameraAction->setIcon( QIcon( ":pictures/systray/camera.png" ) );
    cameraAction->setData( "Camera" );
    cameraAction->setCheckable( true );
    cameraAction->setEnabled( true );
*/
    snapshotAction = new QAction( this );
    snapshotAction->setIcon( QIcon( ":pictures/systray/snapshot.png" ) );
    snapshotAction->setText( "Snapshot" );
    snapshotAction->setData( "Snapshot" );

    exitAction = new QAction( this );
    exitAction->setIcon( QIcon( ":pictures/systray/exit.png" ) );
    exitAction->setData( "Exit" );

    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){startAction->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){stopAction->setEnabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){pauseAction->setEnabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){continueAction->setDisabled(true);});
    connect(ui->pushButtonStart, &QPushButton::clicked, this, [=](){slot_setRecordIcon();});

    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){startAction->setEnabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){stopAction->setDisabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){pauseAction->setDisabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){continueAction->setDisabled(true);});
    connect(ui->pushButtonStop, &QPushButton::clicked, this, [=](){slot_setSystrayIcon();});

    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){startAction->setDisabled(true);});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){stopAction->setDisabled(true);});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){pauseAction->setDisabled(true);});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){continueAction->setEnabled(true);});
    connect(ui->pushButtonPause, &QPushButton::clicked, this, [=](){slot_setPauseIcon();});

    connect( ui->pushButtonContinue, SIGNAL( clicked(bool) ), startAction,    SLOT( setEnabled(bool) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked(bool) ), stopAction,     SLOT( setDisabled(bool) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked(bool) ), pauseAction,    SLOT( setDisabled(bool) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked(bool) ), continueAction, SLOT( setEnabled(bool) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked(bool) ), this,           SLOT( slot_setRecordIcon() ) );

    connect( startAction,    SIGNAL( triggered(bool) ), ui->pushButtonStart,    SLOT( click() ) );
    connect( stopAction,     SIGNAL( triggered(bool) ), ui->pushButtonStop,     SLOT( click() ) );
    connect( pauseAction,    SIGNAL( triggered(bool) ), ui->pushButtonPause,    SLOT( click() ) );
    connect( continueAction, SIGNAL( triggered(bool) ), ui->pushButtonContinue, SLOT( click() ) );
/*
    connect( ui->checkBoxCameraOnOff, SIGNAL( toggled(bool) ),   cameraAction,            SLOT( setChecked(bool) ) );
    connect( cameraAction,            SIGNAL( triggered(bool) ), ui->checkBoxCameraOnOff, SLOT( setChecked(bool) ) );
    connect( ui->comboBoxCamera,      SIGNAL( currentIndexChanged(int) ), this,           SLOT( slot_currentIndexChanged(int) ) );
*/
    connect( snapshotAction, SIGNAL( triggered(bool) ), ui->pushButtonSnapshot, SLOT( click() ) );

    menu = new QMenu();
    menu->addAction( titleAction );
    menu->addSeparator();
    menu->addAction( startAction );
    menu->addAction( stopAction );
    menu->addAction( pauseAction );
    menu->addAction( continueAction );
    menu->addSeparator();
//    menu->addAction( cameraAction );
    menu->addAction( snapshotAction );
    menu->addSeparator();
    menu->addAction( exitAction );

    setIcon( QIcon( ":/pictures/systray/systray.png" ) );
    setContextMenu ( menu );
    setToolTip( global::name );

    connect( exitAction, SIGNAL( triggered(bool) ), this, SLOT( slot_hide() ) );
/*
    if ( ui->checkBoxCameraOnOff->isEnabled() == false ){
        cameraAction->setEnabled( false );
    }
*/
    setMenuText();
}


void QvkSystray_wl::setMenuText()
{
    startAction->setText( tr( "Start" ) );
    stopAction->setText( tr( "Stop" ) );
    pauseAction->setText( tr( "Pause" ) );
    continueAction->setText( tr( "Continue" ) );
//    cameraAction->setText( tr( "Camera" ) );
    exitAction->setText( tr( "Exit" ) );
}


// This slot need in this class
void QvkSystray_wl::slot_hide()
{
    hide();
    emit signal_SystemtrayIsClose();
}


void QvkSystray_wl::slot_closeSystray()
{
    hide();
}


void QvkSystray_wl::slot_setRecordIcon()
{
    setIcon( QIcon( ":/pictures/systray/record.png" ) );
}


void QvkSystray_wl::slot_setSystrayIcon()
{
    setIcon( QIcon( ":/pictures/systray/systray.png" ) );
}


void QvkSystray_wl::slot_setPauseIcon()
{
    setIcon( QIcon( ":/pictures/systray/pause.png" ) );
}

/*
void QvkSystray_wl::slot_currentIndexChanged( int index )
{
    if ( index > -1 ) {
        cameraAction->setEnabled( true );
    } else {
        cameraAction->setEnabled( false );
    }
}
*/
