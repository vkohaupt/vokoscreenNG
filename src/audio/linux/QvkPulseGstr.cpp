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

#include "QvkPulseGstr.h"

#include <QDebug>

QvkPulseGstr::QvkPulseGstr()
{
}


QvkPulseGstr::~QvkPulseGstr()
{
}


QString QvkPulseGstr::get_AudioDeviceString( GstDevice *device )
{
  static const char *const ignored_propnames[] = { "name", "parent", "direction", "template", "caps", Q_NULLPTR };
  GString *launch_line = Q_NULLPTR;
  GstElement *element;
  GstElement *pureelement;
  GParamSpec **properties, *property;
  GValue value = G_VALUE_INIT;
  GValue pvalue = G_VALUE_INIT;
  guint i, number_of_properties;
  GstElementFactory *factory;

  element = gst_device_create_element( device, Q_NULLPTR );

  if ( !element )
    return Q_NULLPTR;

  factory = gst_element_get_factory( element );
  if ( !factory )
  {
    gst_object_unref( element );
    return Q_NULLPTR;
  }

  if ( !gst_plugin_feature_get_name( factory ) )
  {
    gst_object_unref( element );
    return Q_NULLPTR;
  }

  pureelement = gst_element_factory_create( factory, Q_NULLPTR );

  properties = g_object_class_list_properties( G_OBJECT_GET_CLASS( element ), &number_of_properties );
  if ( properties )
  {
    for ( i = 0; i < number_of_properties; i++ )
    {
      gint j;
      gboolean ignore = FALSE;
      property = properties[i];

      if ( ( property->flags & G_PARAM_READWRITE ) != G_PARAM_READWRITE )
        continue;

      for ( j = 0; ignored_propnames[j]; j++ )
        if ( !g_strcmp0( ignored_propnames[j], property->name ) )
          ignore = TRUE;

      if ( ignore )
        continue;

      g_value_init( &value, property->value_type );
      g_value_init( &pvalue, property->value_type );
      g_object_get_property( G_OBJECT( element ), property->name, &value );
      g_object_get_property( G_OBJECT( pureelement ), property->name, &pvalue );
      if (gst_value_compare( &value, &pvalue ) != GST_VALUE_EQUAL )
      {
        gchar *valuestr = gst_value_serialize( &value );
        if ( !valuestr )
        {
          GST_WARNING( "Could not serialize property %s:%s", GST_OBJECT_NAME( element ), property->name );
          g_free( valuestr );
          goto next;
        }

        launch_line = g_string_new( valuestr );

        g_free( valuestr );
      }

    next:
      g_value_unset( &value );
      g_value_unset( &pvalue );
    }
    g_free( properties );
  }

  gst_object_unref( GST_OBJECT( element ) );
  gst_object_unref( GST_OBJECT( pureelement ) );

  QString string = g_string_free( launch_line, FALSE );
  return string;
}


QStringList QvkPulseGstr::get_all_Audio_devices()
{
    GstDeviceMonitor *monitor;
    GstCaps *caps;
    GstDevice *device;
    gchar *name;
    GList *iterator = Q_NULLPTR;
    GList *list = Q_NULLPTR;
    QString stringDevice;
    QStringList stringList;

    monitor = gst_device_monitor_new();
    caps = gst_caps_new_empty_simple( "audio/x-raw" );
    gst_device_monitor_add_filter( monitor, "Audio/Source", caps );
    gboolean isMonitorStart;
    isMonitorStart =  gst_device_monitor_start( monitor );

    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next )
    {
        device = (GstDevice*)iterator->data;
        name = gst_device_get_display_name( device );
        stringDevice = get_AudioDeviceString( device );
        stringDevice.append( ":::" ).append( name );
        stringList.append( stringDevice );
    }

    if ( isMonitorStart )
    {
       // 31.08.2020 Deaktiviert da dies ab und zu beim starten von vokoscreenNG einen Absturz verursacht.
       // gst_device_monitor_stop( monitor );
    }

    return stringList;
}
