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

#include "glib.h"
#include <gst/gst.h>

#include "global.h"
#include "QvkPadsAndCaps.h"

#include <QDebug>

QvkPadsAndCaps::QvkPadsAndCaps()
{
}


QvkPadsAndCaps::~QvkPadsAndCaps()
{
}


// Functions below print the Capabilities in a human-friendly format
QStringList list;
gboolean QvkPadsAndCaps::print_field( GQuark field, const GValue *value, gpointer pfx )
{
    Q_UNUSED(pfx)
    gchar *str = gst_value_serialize( value );

    if ( (QString)g_quark_to_string( field ) == "profile" ) {
        QString my_str = str;
        my_str.remove( "{" );
        my_str.remove( "}" );
        my_str.remove( "(string)" );
        my_str.remove( "," );
        my_str = my_str.trimmed();

        list << my_str.split( " " );
    }

    g_free (str);
    return true;
}


void QvkPadsAndCaps::pad_profile()
{
    const GList *pads;
    GstStaticPadTemplate *padtemplate;

    GstElementFactory *source_factory = gst_element_factory_find ( "openh264enc" );
    if ( !source_factory ) {
        g_printerr( "Not all element factories could be created.\n" );
        return;
    }

    pads = gst_element_factory_get_static_pad_templates( source_factory );
    while( pads )
    {
        padtemplate = (GstStaticPadTemplate *) pads->data;
        pads = g_list_next( pads );
        if ( padtemplate->static_caps.string )
        {
            GstCaps *caps = gst_static_caps_get( &padtemplate->static_caps );

            for ( guint i = 0; i < gst_caps_get_size( caps ); i++ )
            {
                GstStructure *structure = gst_caps_get_structure( caps, i );
                gst_structure_foreach( structure, print_field, NULL );
            }

            gst_caps_unref (caps);
        }
    }

    emit signal_openh264encProfile( list );

    gst_object_unref( source_factory );
}
