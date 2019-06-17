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
    print_plugin_info();
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
    bool isMonitorStart =  gst_device_monitor_start( monitor );

    list = gst_device_monitor_get_devices( monitor );
    for ( iterator = list; iterator; iterator = iterator->next )
    {
        device = (GstDevice*)iterator->data;
        name = gst_device_get_display_name( device );
        stringDevice = get_AudioDeviceString( device );
        stringDevice.append( ":::" ).append( name );
        stringList.append( stringDevice );
    }

    if ( isMonitorStart == true )
    {
       gst_device_monitor_stop( monitor );
    }

    return stringList;
}


// new ****************************************************************************************** compile only no function

void QvkPulseGstr::print_plugin_info ()
{
/*   GList *list, *g;
   GstRegistry *registry;
   registry = gst_registry_get ();
   list = gst_registry_get_plugin_list (registry);
   for (g = list; g; g = g->next)
   {
     GstPlugin *plugin = GST_PLUGIN (g->data);
     // one for the registry, one for the list
     GST_DEBUG ("Plugin refcount %d %s", GST_OBJECT_REFCOUNT_VALUE (plugin), gst_plugin_get_name (plugin));
//     ASSERT_OBJECT_REFCOUNT (plugin, "plugin in registry", 2);
     gst_object_unref (plugin);
   }
   g_list_free (list);

   list = gst_registry_feature_filter (registry, NULL, FALSE, NULL);
   for (g = list; g; g = g->next)
   {
      GstPluginFeature *feature = GST_PLUGIN_FEATURE (g->data);

       // one for the registry, one for the list
       GST_DEBUG ("Feature refcount %d %s", GST_OBJECT_REFCOUNT_VALUE (feature),
           GST_OBJECT_NAME (feature));
       gst_object_unref (feature);
   }
   g_list_free (list);
*/
/* Hier ist dies letzte bearbeitung
    GList *gList, *g;
    GList *iterator;
    GstRegistry *gstRegistry;
    gList = gst_registry_get_plugin_list( gst_registry_get() );
    for (int i = 0; i < g_list_length (gList); i++)
    {
        GstPlugin *plugin = GST_PLUGIN( gList->data );
        qDebug() << gst_plugin_get_name( plugin );
        gst_object_unref (plugin);
        gList->next;
    }
*/
/*
    const gchar *name;
    name = "vp8enc";
    GstPlugin *plugin = gst_registry_find_plugin( gst_registry_get(), name );

    GstElementFactory *factory = gst_element_factory_find( name );
    if ( !factory )
    {
        qDebug().noquote() << global::nameOutput << "Fail Muxer not available:" << name;
    }
    else
    {
        qDebug().noquote() << global::nameOutput << "Muxer available:" << name;
    }
*/

//**********************

//    const gchar *release_date = gst_plugin_get_release_date_string (plugin);
//    const gchar *filename = gst_plugin_get_filename (plugin);

//    n_print ("Plugin Details:\n");

//    push_indent ();

    /*  n_print ("%-25s%s\n", "Name", gst_plugin_get_name (plugin));
  n_print ("%-25s%s\n", "Description", gst_plugin_get_description (plugin));
  n_print ("%-25s%s\n", "Filename", (filename != NULL) ? filename : "(null)");
  n_print ("%-25s%s\n", "Version", gst_plugin_get_version (plugin));
  n_print ("%-25s%s\n", "License", gst_plugin_get_license (plugin));
  n_print ("%-25s%s\n", "Source module", gst_plugin_get_source (plugin));

    if (release_date != NULL) {
        const gchar *tz = "(UTC)";
        gchar *str, *sep;

        // may be: YYYY-MM-DD or YYYY-MM-DDTHH:MMZ
        // YYYY-MM-DDTHH:MMZ => YYYY-MM-DD HH:MM (UTC)
        str = g_strdup (release_date);
        sep = strstr (str, "T");
        if (sep != NULL) {
        *sep = ' ';
            sep = strstr (sep + 1, "Z");
            if (sep != NULL)
                *sep = ' ';
        } else {
            tz = "";
        }
//        n_print ("%-25s%s%s\n", "Source release date", str, tz);
        g_free (str);
    }
*/
//    n_print ("%-25s%s\n", "Binary package", gst_plugin_get_package (plugin));
//    n_print ("%-25s%s\n", "Origin URL", gst_plugin_get_origin (plugin));

//     pop_indent ();

//    n_print ("\n");
}
