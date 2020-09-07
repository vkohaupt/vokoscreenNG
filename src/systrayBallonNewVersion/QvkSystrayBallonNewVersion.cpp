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

#include "QvkSystrayBallonNewVersion.h"
#include "global.h"

#include <QDebug>

QvkSystrayBallonNewVersion::QvkSystrayBallonNewVersion(Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
}


QvkSystrayBallonNewVersion::~QvkSystrayBallonNewVersion()
{
}


void QvkSystrayBallonNewVersion::init()
{
    ui->label_Upate->clear();
    QPalette palette = QPalette( ui->label_Upate->palette() );
    palette.setColor( palette.WindowText, QColor( Qt::magenta ) );
    ui->label_Upate->setPalette( palette );
    connect( &version, SIGNAL( signal_newVersionAvailable( QString ) ), this, SLOT( slot_newVersionAvailable( QString ) ) );
    connect( ui->checkBoxLookForUpdates, SIGNAL( toggled( bool ) ), &version, SLOT( slot_doDownload( bool ) ) );
}


void QvkSystrayBallonNewVersion::slot_newVersionAvailable( QString update )
{
    if ( ui->checkBoxLookForUpdates->isChecked() == true )
    {
        if ( global::version < update )
        {
            QString string = "New Version yyyavailable: " + update;
            ui->label_Upate->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + string + "</a>" );
        }
        else
        {
            ui->label_Upate->setText( "No update available" );
        }
    }
    else
    {
        ui->label_Upate->clear();
    }
}
