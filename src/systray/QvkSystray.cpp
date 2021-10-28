/* vokoscreenNG - A desktop recorder
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

#include "QvkSystray.h"
#include "global.h"
#include "QvkSpezialCheckbox.h"

#include <QDebug>

QvkSystray::QvkSystray(Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkSystray::~QvkSystray()
{
}


void QvkSystray::init()
{
    QAction *titleAction = new QAction( this );
    titleAction->setIcon( QIcon( ":pictures/systray/systray.png" ) );
    titleAction->setText( global::name + " " + global::version );
    titleAction->setEnabled( false );

    startAction = new QAction( this );
    startAction->setIcon( QIcon( ":pictures/player/start.png" ) );
    startAction->setText( tr( "Start" ) );
    startAction->setData( "Start" );

    stopAction = new QAction( this );
    stopAction->setIcon( QIcon( ":pictures/player/stop.png" ) );
    stopAction->setText( tr( "Stop" ) );
    stopAction->setData( "Stop" );
    stopAction->setEnabled( false );

    pauseAction = new QAction( this );
    pauseAction->setIcon( QIcon( ":pictures/player/pause.png" ) );
    pauseAction->setText( tr( "Pause" ) );
    pauseAction->setData( "Pause" );
    pauseAction->setEnabled( false );

    continueAction = new QAction( this );
    continueAction->setIcon( QIcon( ":pictures/player/start.png" ) );
    continueAction->setText( tr( "Continue" ) );
    continueAction->setData( "Continue" );
    continueAction->setEnabled( false );

    cameraAction = new QAction( this );
    cameraAction->setIcon( QIcon( ":pictures/camera/camera-systray.png" ) );
    cameraAction->setText( tr( "Camera" ) );
    cameraAction->setData( "Camera" );
    cameraAction->setCheckable( true );

    magnifierAction = new QAction( this );
    magnifierAction->setIcon( QIcon( ":pictures/systray/magnification.png" ) );
    magnifierAction->setText( tr( "Magnification" ) );
    magnifierAction->setData( "Magnification" );
    magnifierAction->setCheckable( true );

    showclickAction = new QAction( this );
    showclickAction->setIcon( QIcon( ":pictures/systray/showclick.png" ) );
    showclickAction->setText( "ShowClick" );
    showclickAction->setData( "ShowClick" );
    showclickAction->setCheckable( true );

    haloAction = new QAction( this );
    haloAction->setIcon( QIcon( ":pictures/systray/showclick.png" ) );
    haloAction->setText( "Halo" );
    haloAction->setData( "Halo" );
    haloAction->setCheckable( true );

    exitAction = new QAction( this );
    exitAction->setIcon( QIcon( ":pictures/systray/exit.png" ) );
    exitAction->setText( tr( "Exit" ) );
    exitAction->setData( "Exit" );

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setRecordIcon( bool ) ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), startAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setSystrayIcon( bool ) ) );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), continueAction, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setPauseIcon( bool ) ) );

    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setRecordIcon( bool ) ) );

    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ),  startAction, SLOT( setDisabled( bool ) ) );

    connect( startAction,    SIGNAL( triggered( bool ) ), ui->pushButtonStart,    SLOT( click() ) );
    connect( stopAction,     SIGNAL( triggered( bool ) ), ui->pushButtonStop,     SLOT( click() ) );
    connect( pauseAction,    SIGNAL( triggered( bool ) ), ui->pushButtonPause,    SLOT( click() ) );
    connect( continueAction, SIGNAL( triggered( bool ) ), ui->pushButtonContinue, SLOT( click() ) );

    connect( ui->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ),   cameraAction, SLOT( setChecked( bool ) ) );
    connect( cameraAction,            SIGNAL( triggered( bool ) ), ui->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );

    connect( ui->checkBoxMagnifier, SIGNAL( toggled( bool ) ),   magnifierAction, SLOT( setChecked( bool ) ) );
    connect( magnifierAction,       SIGNAL( triggered( bool ) ), ui->checkBoxMagnifier, SLOT( click() ) );

    QList<QvkSpezialCheckbox *> listSpezialCheckbox = ui->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckbox.count(); i++ )
    {
        if ( listSpezialCheckbox.at(i)->objectName() == "spezialCheckboxShowclick" )
        {
            connect( listSpezialCheckbox.at(i), SIGNAL( signal_clicked( bool ) ), showclickAction, SLOT( setChecked( bool ) ) );
            connect( showclickAction,           SIGNAL( triggered( bool ) ),      listSpezialCheckbox.at(i), SLOT( slot_click() ) );
        }

        if ( listSpezialCheckbox.at(i)->objectName() == "spezialCheckboxHalo" )
        {
            connect( listSpezialCheckbox.at(i), SIGNAL( signal_clicked( bool ) ), haloAction, SLOT( setChecked( bool ) ) );
            connect( haloAction,                SIGNAL( triggered( bool ) ),      listSpezialCheckbox.at(i), SLOT( slot_click() ) );
        }
    }

    menu = new QMenu();
    menu->addAction( titleAction );
    menu->addSeparator();
    menu->addAction( startAction );
    menu->addAction( stopAction );
    menu->addAction( pauseAction );
    menu->addAction( continueAction );
    menu->addSeparator();
    menu->addAction( cameraAction );
    menu->addAction( magnifierAction );
    menu->addAction( showclickAction );
    menu->addAction( haloAction );
    menu->addSeparator();
    menu->addAction( exitAction );

    setIcon( QIcon( ":/pictures/systray/systray.png" ) );
    setContextMenu ( menu );
    setToolTip( global::name );
    show();

    connect( exitAction, SIGNAL( triggered( bool ) ), this, SLOT( slot_hide() ) );

    connect( &version, SIGNAL( signal_newVersionAvailable( QString ) ), this, SLOT( slot_newVersionAvailable( QString ) ) );
    connect( ui->checkBoxLookForUpdates, SIGNAL( toggled( bool ) ), &version, SLOT( slot_doDownload( bool ) ) );
}


void QvkSystray::slot_newVersionAvailable( QString update )
{
    if ( ui->checkBoxLookForUpdates->isChecked() == true )
    {
        if ( global::version < update )
        {
            if ( QSystemTrayIcon::supportsMessages() == true )
            {
                connect( this, SIGNAL( messageClicked() ), this, SLOT( slot_showHomepage() ) );
                showMessage( global::name, "New Version available: " + update, QSystemTrayIcon::Information, 5000 );
            }
        }
    }
}


void QvkSystray::slot_showHomepage()
{
   QDesktopServices::openUrl(QUrl("https://linuxecke.volkoh.de/vokoscreen/vokoscreen.html", QUrl::TolerantMode));
}


// This slot need in this class
void QvkSystray::slot_hide()
{
    hide();
    emit signal_SystemtrayIsClose();
}

// This slot need in Main GUI
void QvkSystray::slot_closeSystray()
{
    hide();
}


void QvkSystray::slot_setRecordIcon( bool )
{
    setIcon( QIcon( ":/pictures/systray/record.png" ) );
}


void QvkSystray::slot_setSystrayIcon( bool )
{
    setIcon( QIcon( ":/pictures/systray/systray.png" ) );
}


void QvkSystray::slot_setPauseIcon( bool )
{
    setIcon( QIcon( ":/pictures/systray/pause.png" ) );
}
