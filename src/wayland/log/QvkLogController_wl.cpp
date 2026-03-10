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

#include <QMessageLogContext>
#include <QtLogging>
#include <QtMessageHandler>
#include <QFile>
#include <QString>
#include <QTime>
#include <QByteArray>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

#include <global.h>
#include "QvkLogController_wl.h"

QFile logFile_wl;

void myMessageHandler_wl( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    // Insert timestamp
    QString sTime = QTime::currentTime().toString();
    QString sMsg( msg );
    if ( !sMsg.isEmpty() ) {
        sMsg.prepend( sTime + " " ) ;
    }

    // Output terminal
    QByteArray localMsg = sMsg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf( stderr, "%s\n", localMsg.constData() );
        break;
    case QtInfoMsg:
        fprintf( stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
    case QtWarningMsg:
        fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
    case QtCriticalMsg:
        fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        break;
    case QtFatalMsg:
        fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        abort();
    }

    // Output Logfile
#ifdef Q_OS_UNIX
    QString eol = "\n";
#endif
#ifdef Q_OS_WIN
    QString eol = "\r\n";
#endif
    bool bo = logFile_wl.open( QIODevice::WriteOnly | QIODevice::Append );
    if ( bo == false ) {
        qDebug().noquote() << sTime << global::nameOutput << "[myMessageHandler_wl] Can not open" << logFile_wl.fileName();
        return;
    }
    QTextStream stream( &logFile_wl );
    stream << sMsg << eol;
    logFile_wl.close();
}


QvkLogController_wl::QvkLogController_wl()
{
    QString stringDateTime = QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" );
    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

    QString logFolderName = path + "/" + "log";
    if( !QDir( logFolderName ).exists() ) {
        QDir().mkpath( logFolderName );
    }

    logFile_wl.setFileName( path + "/" + "log" + "/" + stringDateTime + ".log" );
    qInstallMessageHandler( myMessageHandler_wl );
}


QvkLogController_wl::~QvkLogController_wl()
{
}


QString QvkLogController_wl::get_log_filePath()
{
    return logFile_wl.fileName();
}
