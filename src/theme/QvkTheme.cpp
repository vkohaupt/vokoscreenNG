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

#include "QvkTheme.h"
#include "global.h"

#include <QDebug>

QvkTheme::QvkTheme( Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;

    setTheme();
}


QvkTheme::~QvkTheme()
{  
}


void QvkTheme::setTheme()
{
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarScreencast ), QIcon( ":/pictures/screencast/monitor.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarPlayer ),     QIcon( ":/pictures/player/player.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarCamera ),     QIcon( ":/pictures/camera/camera.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarShowClick ),  QIcon( ":/pictures/showClick/vokoShowClick.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarLog ),        QIcon( ":/pictures/log/log.png" ) );

    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabScreen ),    QIcon( ":/pictures/screencast/monitor.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabAudio ),     QIcon( ":/pictures/screencast/microphone.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabVideo ),     QIcon( ":/pictures/screencast/format-codec.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabMisc ),      QIcon( ":/pictures/screencast/preferences-system.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabTimer ),     QIcon( ":/pictures/screencast/timer.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabAvailable ), QIcon( ":/pictures/screencast/supported-formats.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabAbout ),     QIcon( ":/pictures/screencast/help-about.png" ) );

    ui->toolButtonShowInSystrayAlternativeReset->setIcon( QIcon( ":/pictures/screencast/undo.png" ) );

    makeAndSetValidIcon( ui->tabWidgetCamera, ui->tabWidgetCamera->indexOf( ui->tabCamera ), QIcon( ":/pictures/camera/camera.png" ) );

    makeAndSetValidIcon( ui->tabWidgetLog, ui->tabWidgetLog->indexOf( ui->tabLog ), QIcon( ":/pictures/log/log.png" ) );
}



void QvkTheme::makeAndSetValidIcon( QTabWidget *tabWidget, int index , QIcon icon )
{
    int a = 128;
    QPixmap iconPixmap = icon.pixmap( a, a );
    tabWidget->setTabIcon( index, QIcon( iconPixmap ) );
}


void QvkTheme::makeAndSetValidIconForSideBar( int index, QIcon icon )
{
    int a = 128;
    QPixmap workPixmap(icon.pixmap( a, a ) );

    QTransform transform;
    transform.rotate( 90 );
    workPixmap = workPixmap.transformed( transform, Qt::SmoothTransformation );
    ui->tabWidgetSideBar->setTabIcon( index, workPixmap );
}
