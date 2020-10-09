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

#include <QDir>

#include "QvkCiscoOpenh264Controller.h"
#include "QvkCiscoOpenh264Downloader.h"
#include "global.h"

QvkCiscoOpenh264Controller::QvkCiscoOpenh264Controller( QString pathWithSettingsFilename )
{
    vkBzlib = new QvkBzlib();

    QFileInfo fileInfo( pathWithSettingsFilename );
    QvkCiscoOpenh264Downloader *vkCiscoOpenh264Downloader = new QvkCiscoOpenh264Downloader( fileInfo.path() );
    connect( vkCiscoOpenh264Downloader, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_deCompress( QString ) ) );

//    connect( vkBzlib, SIGNAL( signal_deCompressed( QString ) ), this, SLOT( slot_rename( QString ) ) );

    QString downloadFile = "http://ciscobinary.openh264.org/openh264-2.1.1-win32.dll.bz2";
    vkCiscoOpenh264Downloader->doDownload( downloadFile );
}


QvkCiscoOpenh264Controller::~QvkCiscoOpenh264Controller()
{
}


void QvkCiscoOpenh264Controller::slot_deCompress( QString pathWithFile )
{
    vkBzlib->deCompress( pathWithFile );
}


void QvkCiscoOpenh264Controller::slot_rename( QString pathWithFile )
{
    QString openh264FileName = "libopenh264.dll";

    // Delete old libopenh264.dll
    QFileInfo fileInfoFileOld( pathWithFile );
    QFile fileOld( fileInfoFileOld.path() + QDir::separator() + openh264FileName );
    if ( fileOld.remove() )
    {
        qDebug().noquote() << global::nameOutput << "[h264] Delete" << fileInfoFileOld.path() + QDir::separator() + openh264FileName;
    }
    else
    {
        qDebug().noquote() << global::nameOutput << "[h264] Delete failed" << fileInfoFileOld.path() + QDir::separator() + openh264FileName;
    }

    // Rename openh264-x.x.x-win32.dll to libopenh264.dll
    QFileInfo fileInfo( pathWithFile );
    QFile file( pathWithFile );
    if ( file.rename( fileInfo.path() + QDir::separator() + openh264FileName ) )
    {
        qDebug().noquote() << global::nameOutput << "[h264] Rename" << pathWithFile << "to" << fileInfo.path() + QDir::separator() + openh264FileName;
    }
    else
    {
        qDebug().noquote() << global::nameOutput << "[h264] Rename failed" << pathWithFile << "to" << fileInfo.path() + QDir::separator() + openh264FileName;
    }

    // Delete downloaded *.bz2 file
    QString bzFile = pathWithFile + ".bz2";
    QFile fileDeleteBz( bzFile );
    if ( fileDeleteBz.remove() )
    {
        qDebug().noquote() << global::nameOutput << "[h264] Delete " << bzFile;
    }
    else
    {
        qDebug().noquote() << global::nameOutput << "[h264] Delete failed" << bzFile;
    }

}

