/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
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

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/pulseaudio.h>

#include "QvkPulseAudioServer.h"

QvkPulseAudioServer::QvkPulseAudioServer()
{
}


QvkPulseAudioServer::~QvkPulseAudioServer()
{
}


bool QvkPulseAudioServer::isAvailable()
{
    bool value = false;

    pa_mainloop *pa_ml;
    pa_mainloop_api *pa_mlapi;
    pa_context *context = NULL;

    // Create a mainloop API and connection to the default server
    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api( pa_ml );
    context = pa_context_new( pa_mlapi, NULL );

    // This function connects to the pulse server
    int status = pa_context_connect( context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL );
    if ( status < 0 ) {
        value = false;
    }
    else {
        value = true;
    }

    pa_context_unref( context );
    pa_mainloop_free( pa_ml );

    return value;
}
