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

#include "mainwindow.h"
#include "global.h"

#ifdef Q_OS_LINUX
#include "QvkWaylandRoutines.h"
#include "wl_mainwindow.h"
#endif

#ifdef Q_OS_WIN
#include "QvkSettings.h"
#endif

#include <QTranslator>
#include <QLibraryInfo>

#include <gst/gst.h>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    QString help;
    help.append( "\n" );
    help.append( "Usage: " + global::name + " [Option] [File or URL]" + "\n" );
    help.append( "\n" );
    help.append( "Options: \n" );
    help.append( "  -h or --help        Print this message\n" );
    help.append( "  -v or --version     Print version \n" );
    help.append( "\n" );
    help.append( "File or Url:\n" );
    help.append( "  Play a video\n" );
    help.append( "    Example file:\n" );
    help.append( "      vokoscreenNG /path/video\n" );
    help.append( "\n" );
    help.append( "    Example URL:\n");
    help.append( "      vokoscreenNG http://www.rapconverter.com/SampleDownload/Sample320.mp4\n");
    help.append( "      vokoscreenNG http://distribution.bbb3d.renderfarming.net/video/mp4/bbb_sunflower_1080p_60fps_normal.mp4" );
    help.append( "\n" );

    QStringList arguments = QApplication::instance()->arguments();
    if ( !arguments.empty() and ( arguments.count() == 2 ) )
    {
        QStringList arguments = QApplication::instance()->arguments();
        if ( ( arguments.at(1) == "--help" ) or
             ( arguments.at(1) == "-h"     ) )
        {
            qDebug().resetFormat().noquote() << help;
            return 0;
        }

        if ( ( arguments.at(1) == "--version" ) or ( arguments.at(1) == "-v" ) )
        {
            qDebug().noquote() << global::name << global::version;
            return 0;
        }

        // If call from terminal and local file or remote file not exists
        QFile file( arguments.at(1) );
        if ( ( file.exists() == false ) and
             ( arguments.at(1).contains( "http://" ) == false ) and
             ( arguments.at(1).contains( "https://" ) == false ) )
        {
            qDebug().noquote() << global::nameOutput << arguments.at(1) << "file not exists";
            qDebug().resetFormat().noquote() << help;
            qDebug( " " );
            return 1;
        }
    }

    // Initialize GStreamer
    // https://developer.gnome.org/gstreamer/stable/gst-running.html
#ifdef Q_OS_WIN
    QvkSettings vkSettings;
    QFileInfo dirPathProfile( vkSettings.getFileName() );
    QString pathProfile = dirPathProfile.absolutePath();
    QString programPath = QDir::currentPath();

    QByteArray programPathByteArray;
    programPathByteArray.append( programPath );
    programPathByteArray.append( ";" );
    programPathByteArray.append( pathProfile );
    qputenv( "GSTREAMER_1_0_ROOT_X86", programPathByteArray );

    QByteArray pluginPathByteArray;
    pluginPathByteArray.append( programPath );
    pluginPathByteArray.append( ";" );
    pluginPathByteArray.append( pathProfile );
    qputenv( "GST_PLUGIN_PATH_1_0", pluginPathByteArray );

    QByteArray pathPathByteArray;
    pathPathByteArray.append( programPath );
    pathPathByteArray.append( ";" );
    pathPathByteArray.append( pathProfile );
    qputenv( "PATH", pathPathByteArray );

/*
    //Environment variables for debugging
    qputenv( "GST_DEBUG", "4");
    QByteArray envPathProfile;
    envPathProfile.append( pathProfile.path() + "/GST_Error.txt" );
    qputenv( "GST_DEBUG_FILE", envPathProfile );
*/
#endif

    gst_init (&argc, &argv);

    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    QTranslator translator;
    translator.load( QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );

#ifdef Q_OS_LINUX
    if ( QvkWaylandRoutines::is_Wayland_Display_Available() == false )
    {
        QvkMainWindow *w = new QvkMainWindow;
        w->show();
    }
    else
    {
        Qvk_wl_MainWindow *wl = new Qvk_wl_MainWindow;
        wl->show();
    }
#endif

/*
#ifdef Q_OS_LINUX
    if ( qgetenv( "XDG_SESSION_TYPE" ).toLower() == "x11" )
    {
        QvkMainWindow *w = new QvkMainWindow;
        w->show();
    }
    
    if ( qgetenv( "XDG_SESSION_TYPE" ).toLower() == "wayland" )
    {
        Qvk_wl_MainWindow *wl = new Qvk_wl_MainWindow;
        wl->show();
    }
#endif
*/

#ifdef Q_OS_WIN
    QvkMainWindow *w = new QvkMainWindow;
    w->show();
#endif

    return app.exec();
}
