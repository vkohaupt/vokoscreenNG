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
#include <QTimer>
#include <QPainter>

QvkTheme::QvkTheme( Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;

    timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_searchNewTheme() ) );
    timer->start(3000);

    connect( this, SIGNAL( signal_newTheme() ), this, SLOT( slot_setNewTheme() ) );

    slot_setNewTheme();
}


QvkTheme::~QvkTheme()
{  
}


void QvkTheme::slot_searchNewTheme()
{
    if ( QIcon::themeName() != oldThemeName )
    {
        oldThemeName = QIcon::themeName();
        emit signal_newTheme();
    }
}


void QvkTheme::slot_setNewTheme()
{
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarScreencast ), VK_getIcon( "video-display",     ":/pictures/screencast/monitor.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarPlayer ),     VK_getIcon( "multimedia-player", ":/pictures/player/player.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarCamera ),     VK_getIcon( "camera-web",        ":/pictures/camera/camera.png" ) );
    makeAndSetValidIconForSideBar( ui->tabWidgetSideBar->indexOf( ui->tabSidebarLog ),        VK_getIcon( "help-about",        ":/pictures/log/log.png" ) );

    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabScreen ),    VK_getIcon( "video-display",          ":/pictures/screencast/monitor.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabAudio ),     VK_getIcon( "audio-input-microphone", ":/pictures/screencast/microphone.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabTimer ),     VK_getIcon( "appointment-new",        ":/pictures/screencast/timer.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabMisc ),      VK_getIcon( "preferences-system",     ":/pictures/screencast/preferences-system.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabAvailable ), VK_getIcon( "help-contents",          ":/pictures/screencast/supported-formats.png" ) );
    makeAndSetValidIcon( ui->tabWidgetScreencast, ui->tabWidgetScreencast->indexOf( ui->tabAbout ),     VK_getIcon( "help-about",             ":/pictures/screencast/help-about.png" ) );

    ui->toolButtonScreencastAreaReset->setIcon( VK_getIcon( "edit-undo", ":/pictures/screencast/undo.png" ) );
    ui->toolButtonScreencastx264Reset->setIcon( VK_getIcon( "edit-undo", ":/pictures/screencast/undo.png" ) );

    makeAndSetValidIcon( ui->tabWidgetCamera, ui->tabWidgetCamera->indexOf( ui->tabCamera ), VK_getIcon( "camera-web", ":/pictures/camera/camera.png" ) );

    makeAndSetValidIcon( ui->tabWidgetLog, ui->tabWidgetLog->indexOf( ui->tabLog ), VK_getIcon( "help-about", ":/pictures/log/log.png" ) );
}


QIcon QvkTheme::VK_getIcon( QString iconName, QString iconNameFallback )
{
    QIcon icon;

    if ( global::VK_showOnlyFallbackIcons == true )
    {
        QIcon tmpIcon( iconNameFallback );
        icon = tmpIcon;
        return icon;
    }

    if ( QIcon::hasThemeIcon( iconName ) == true )
    {
        icon = QIcon::fromTheme( iconName );
    }
    else
    {
        QIcon tmpIcon( iconNameFallback );
        icon = tmpIcon;
    }

    return icon;
}


void QvkTheme::makeAndSetValidIcon( QTabWidget *tabWidget, int index , QIcon icon )
{
    int a = 128;
    QPixmap iconPixmap = icon.pixmap( a, a );
    tabWidget->setTabIcon( index, QIcon( iconPixmap ) );
}


void QvkTheme::makeAndSetValidIconForSideBar( int index, QIcon icon )
{
    int a = 256;
    QSize size = QSize( a, a );

    QPixmap iconPixmap( icon.pixmap( size ) );

    QFont font;
    font.setPointSize( 40 );
    QFontMetrics fontMetrics( font );
    int textWidth = fontMetrics.width( ui->tabWidgetSideBar->tabToolTip( index ) );
    int textHight = fontMetrics.height();

    QPixmap workPixmap( size.width() + textHight, size.height() + textHight );
    workPixmap.fill( Qt::transparent );

    QPainter painter;
    QPen pen;
    painter.begin( &workPixmap );
      painter.setRenderHints( QPainter::Antialiasing, true );
      painter.setFont( font );
      painter.drawPixmap( QPoint( textHight/2, 0 ), iconPixmap );
      pen.setColor( Qt::black );
      painter.setPen( pen );
      int x = ( workPixmap.width() - textWidth ) / 2;
      painter.drawText( x, workPixmap.height() - fontMetrics.descent(), ui->tabWidgetSideBar->tabToolTip( index ) );
    painter.end();

    QTransform transform;
    transform.rotate( 90 );
    workPixmap = workPixmap.transformed( transform, Qt::SmoothTransformation );
    ui->tabWidgetSideBar->setTabIcon( index, QIcon( workPixmap ) );
}
