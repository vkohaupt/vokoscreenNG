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

#include "QvkCiscoOpenh264Controller.h"
#include "QvkCiscoOpenh264Downloader.h"
#include "QvkBzlib.h"


QvkCiscoOpenh264Controller::QvkCiscoOpenh264Controller()
{
    QvkCiscoOpenh264Downloader *vkCiscoOpenh264Downloader = new QvkCiscoOpenh264Downloader( "/tmp" );
    connect( vkCiscoOpenh264Downloader, SIGNAL( signal_fileDownloaded( QString ) ), this, SLOT( slot_deCompress( QString ) ) );

    QString downloadFile = "http://ciscobinary.openh264.org/openh264-2.1.1-win64.dll.bz2";
    vkCiscoOpenh264Downloader->doDownload( downloadFile );
}


QvkCiscoOpenh264Controller::~QvkCiscoOpenh264Controller()
{
}


void QvkCiscoOpenh264Controller::slot_deCompress( QString pathWithFile )
{
    QvkBzlib *vkBzlib = new QvkBzlib();
    vkBzlib->deCompress( pathWithFile );
}
