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
    help.append( "    Exsample URL:\n");
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
    QString pathString = QDir::currentPath();
    QByteArray pathByteArray;
    pathByteArray.append( pathString );
    qputenv( "GSTREAMER_1_0_ROOT_X86", pathByteArray );
    qputenv( "GST_PLUGIN_PATH", pathByteArray );
    
    //Environment variables for debugging
    //qputenv( "GST_DEBUG", "2");
    //qputenv( "GST_DEBUG_FILE", "C:\\Users\\vk\\Documents\\VK_Error.txt" );
#endif
    gst_init (&argc, &argv);

    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    QTranslator translator;
    translator.load( QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );

    QvkMainWindow w;
    w.show();

    return app.exec();
}
