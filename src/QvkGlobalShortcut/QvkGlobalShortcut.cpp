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

#include "QvkGlobalShortcut.h"

QvkGlobalShortcut::QvkGlobalShortcut(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;

    QGlobalShortcut *shortcutWebcam = new QGlobalShortcut( this );
    connect( shortcutWebcam, SIGNAL( activated() ), ui->checkBoxCameraOnOff, SLOT( click() ) );
    shortcutWebcam->setShortcut( QKeySequence( "Ctrl+Shift+F8" ) );

    QGlobalShortcut *shortcutMagnifier = new QGlobalShortcut( this );
    connect( shortcutMagnifier, SIGNAL( activated() ), ui->checkBoxMagnifier, SLOT( click() ) );
    shortcutMagnifier->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

    QGlobalShortcut *shortcutStart = new QGlobalShortcut( this );
    connect( shortcutStart, SIGNAL( activated() ), ui->pushButtonStart, SLOT( click() ) );
    shortcutStart->setShortcut( QKeySequence( "Ctrl+Shift+F10" ) );

    QGlobalShortcut *shortcutStop = new QGlobalShortcut( this );
    connect( shortcutStop, SIGNAL( activated() ), ui->pushButtonStop, SLOT( click() ) );
    shortcutStop->setShortcut( QKeySequence( "Ctrl+Shift+F11" ) );

    QGlobalShortcut *shortcutPauseContinue = new QGlobalShortcut( this );
    connect( shortcutPauseContinue, SIGNAL( activated() ), this, SLOT( slot_pauseContinue() ) );
    shortcutPauseContinue->setShortcut( QKeySequence( "Ctrl+Shift+F12" ) );
}


QvkGlobalShortcut::~QvkGlobalShortcut()
{
}


void QvkGlobalShortcut::slot_pauseContinue()
{
    if ( ui->pushButtonContinue->isHidden() == true )
    {
        ui->pushButtonPause->click();
    }
    else
    {
        ui->pushButtonContinue->click();
    }
}
