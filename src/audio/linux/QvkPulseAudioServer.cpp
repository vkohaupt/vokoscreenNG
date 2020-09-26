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

#include <pulse/simple.h>
#include <pulse/error.h>

#include "QvkPulseAudioServer.h"

QvkPulseAudioServer::QvkPulseAudioServer()
{
}


QvkPulseAudioServer::~QvkPulseAudioServer()
{
}


bool QvkPulseAudioServer::isAvailable()
{
    // pulseaudio connection
    pa_simple *paConnection = NULL;

    // format specifier
    static pa_sample_spec sspec;
    sspec.channels = 2;
    sspec.format = PA_SAMPLE_S16LE;
    sspec.rate = 44100;

    int error = 0;

    paConnection = pa_simple_new(
                NULL, // default PA server
                "Test libpulse", // app name
                PA_STREAM_RECORD,  // stream direction
                NULL, // default device
                "record", // stream name
                &sspec,  // format spec
                NULL, // default channel map
                NULL, // may be NULL for defaults, but we want tweak!
                &error );

    bool value;
    if( ! paConnection )
    {
        //fprintf( stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror( error ) );
        value = false;
    }
    else
    {
        pa_simple_free( paConnection );
        //printf( "Connected to PulseAudio server ok.\n" );
        value = true;
    }

    return value;
}
