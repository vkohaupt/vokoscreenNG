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

#include "QvkWatcherPlug.h"
#include "global.h"

#include <QDebug>

/*
 * QvkWatcherPlug monitoring only new or removed Audiodevices from the PulseAudio server.
 * QvkWatcherPlug does not return any devices, if the PulseAudio server start or stop.
 */

QvkWatcherPlug::QvkWatcherPlug()
{
    global::lineEditAudioPlug = new QLineEdit;
}

QvkWatcherPlug::~QvkWatcherPlug()
{}

static gchar *get_launch_line (GstDevice * device)
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

  return g_string_free (launch_line, FALSE);
}


gboolean QvkWatcherPlug::func( GstBus *bus, GstMessage *message, gpointer user_data )
{
   Q_UNUSED(bus);
   Q_UNUSED(user_data);

   GstDevice *gstDevice;
   gchar *name;
   gchar *device;

   QString audioDevicePlug = "";

   switch ( GST_MESSAGE_TYPE( message ) )
   {
     case GST_MESSAGE_DEVICE_ADDED:
       gst_message_parse_device_added( message, &gstDevice );
       name = gst_device_get_display_name( gstDevice );
       qDebug().noquote() << global::nameOutput << "[Audio] device added:" << name;
       device = get_launch_line( gstDevice );
       qDebug().noquote() << global::nameOutput << "[Audio] device added:" << device;
       audioDevicePlug.append( "[Audio-device-added]" );
       audioDevicePlug.append( ":");
       audioDevicePlug.append( name );
       audioDevicePlug.append( ":");
       audioDevicePlug.append( device );
       global::lineEditAudioPlug->setText( audioDevicePlug );
       g_free( name );
       g_free( device );
       gst_object_unref( gstDevice );
       break;
     case GST_MESSAGE_DEVICE_REMOVED:
       gst_message_parse_device_removed( message, &gstDevice );
       name = gst_device_get_display_name( gstDevice );
       qDebug().noquote() << global::nameOutput << "[Audio] device removed:" << name;
       device = get_launch_line( gstDevice );
       qDebug().noquote() << global::nameOutput << "[Audio] device removed:" << device;
       audioDevicePlug.append( "[Audio-device-removed]" );
       audioDevicePlug.append( ":");
       audioDevicePlug.append( name );
       audioDevicePlug.append( ":");
       audioDevicePlug.append( device );
       global::lineEditAudioPlug->setText( audioDevicePlug );
       g_free( name );
       g_free( device );
       gst_object_unref( gstDevice );
       break;
     default:
       break;
   }

   return G_SOURCE_CONTINUE;
}


GstDeviceMonitor *QvkWatcherPlug::start_monitor()
{
   GstDeviceMonitor *gstDeviceMonitor;
   GstBus *gstBus;
   GstCaps *gstCaps;

   gstDeviceMonitor = gst_device_monitor_new();
   gstBus = gst_device_monitor_get_bus( gstDeviceMonitor );
   gst_bus_add_watch( gstBus, QvkWatcherPlug::func, Q_NULLPTR );
   gst_object_unref( gstBus );

   gstCaps = gst_caps_new_empty_simple( "audio/x-raw" );
   gst_device_monitor_add_filter( gstDeviceMonitor, "Audio/Source", gstCaps );
   gst_caps_unref( gstCaps );

   gst_device_monitor_start( gstDeviceMonitor );

   return gstDeviceMonitor;
}

