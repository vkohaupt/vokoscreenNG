/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#include "QvkLoadExtension.h"
#include "global.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QLibrary>

QvkLoadExtension::QvkLoadExtension( QvkMainWindow *mainWindow, Ui_formMainWindow *GUI )
{
   ui = GUI;
   parent = mainWindow;
}

QvkLoadExtension::~QvkLoadExtension()
{}

void QvkLoadExtension::loadExtension()
{
  QString extensionsPath = QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) + QDir::separator() + "vokoscreen_extensions" + QDir::separator();
  qDebug().noquote() << global::nameOutput << "ExtensionsPath:" << extensionsPath;

  QDir libDir( extensionsPath );
  QStringList filter = QStringList()  << "*.so.*";
  QStringList libStringList = libDir.entryList( filter, QDir::Files | QDir::NoSymLinks );

  // Testen ob eine lib im Ordner ist
  if ( !libStringList.empty() )
  {

    QLibrary *library = new QLibrary( extensionsPath + libStringList[ 0 ] );

    if ( library->load() )
    {
      qDebug().noquote() << global::nameOutput << libStringList[ 0 ] << "Library has been loaded";

      typedef QWidget*(*CreateWidgetFunction)( Ui_formMainWindow *GUI );
      CreateWidgetFunction cwf = ( CreateWidgetFunction )library->resolve( "vokoscreen_extensions" );

      if ( cwf )
      {
        cwf( ui );
        qDebug().noquote() << global::nameOutput << "Extension was loaded";
        //myUi.extensionLoadpushButton->hide();
      }
      else
      {
        qDebug().noquote() << global::nameOutput << "Extension can not be displayed from the loaded library";
      }
    }
    else
    {
      qDebug().noquote() << global::nameOutput << "Library found but not loaded" << library->errorString();
    }
  }
  else
  {
    qDebug().noquote() << global::nameOutput << "No library for extension found";
  }

  qDebug( " " );
}
