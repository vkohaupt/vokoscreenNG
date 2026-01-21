/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2025 Volker Kohaupt
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

#include "QvkLocale_wl.h"
#include "global.h"

#include <QUrl>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

QvkLocale_wl::QvkLocale_wl()
{
    vkDownload = new QvkDownloader_wl( temporaryDirLocal.path() );
    connect(vkDownload, &QvkDownloader_wl::signal_fileDownloaded, this, [=](QString tempPathFileName){slot_parse(tempPathFileName);});
    vkDownload->doDownload( QUrl( "https://vokoscreen.volkoh.de/3.0/help/getLinuxDirs.php" ) );
    vkDownload->doDownload( QUrl( "https://vokoscreen.volkoh.de/3.0/help/getWaylandDirs.php" ) );
}


QvkLocale_wl::~QvkLocale_wl()
{
}


void QvkLocale_wl::slot_parse( QString tempPathFileName )
{
    QFile file( tempPathFileName );
    if( !file.open( QIODevice::ReadOnly ) ) {
        return;
    }

    QTextStream textStream( &file );
    while( !textStream.atEnd() ) {
        localeList << textStream.readLine();
    }
    file.close();

    temporaryDirLocal.remove();

    emit signal_locale( localeList );
}


void QvkLocale_wl::slot_cleanUp()
{
    bool bo = temporaryDirLocal.remove();
    if ( bo == true ) {
        qDebug().noquote() << global::nameOutput << "QvkLocale::slot_cleanUp TempDir is removed" << temporaryDirLocal.path();
    } else {
        qDebug().noquote() << global::nameOutput << "QvkLocale::slot_cleanUp TempDir not removed" << temporaryDirLocal.path();
    }
}
