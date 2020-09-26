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

#include <QDebug>

#include "QvkDevices.h"
#include "QvkPulseAudioDevices.h"
#include "QvkPulseAudioServer.h"

QvkPulseAudioDevices::QvkPulseAudioDevices()
{    
}


QvkPulseAudioDevices::~QvkPulseAudioDevices()
{
}


QStringList QvkPulseAudioDevices::getAllDevices()
{
    QStringList list;
    if ( QvkPulseAudioServer::isAvailable() )
    {
        const char *ss = get_all_audio_devices();
        QString s1 = QString::fromUtf8( ss );
        QString s2 = s1.left( QString::fromUtf8( ss ).count() - 3 );
        list = s2.split( "---" );
    }
    return list;
}
