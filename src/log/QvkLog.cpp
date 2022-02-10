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

#include "QvkLog.h" 

#include <QDebug>
#include <QStandardPaths>
#include <QTime>
#include <QDir>

QvkLog::QvkLog()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString stringDateTime = dateTime.toString( "yyyy_MM_dd_hh_mm_ss" );
    path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

    QString logFolderName = path + "/" + "log";
    if( !QDir( logFolderName ).exists() )
    {
        QDir().mkpath( logFolderName );
    }

    logFile.setFileName( path + "/" + "log" + "/" + stringDateTime + ".log" );
}


QvkLog::~QvkLog()
{
}


void QvkLog::outputMessage( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    // Output terminal
    QByteArray localMsg = msg.toLocal8Bit();
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
#ifdef Q_OS_LINUX
    QString eol = "\n";
#endif
#ifdef Q_OS_WIN
    QString eol = "\r\n";
#endif
    QString sTime = QTime::currentTime().toString();
    QString sMsg( msg );
    if ( !sMsg.isEmpty() ) {
       sMsg.insert( 14, " " + sTime );
    }
    logFile.open( QIODevice::Append | QIODevice::Text | QIODevice::Unbuffered );
    logFile.write( sMsg.toUtf8() );
    logFile.write( eol.toUtf8() );
    logFile.close();

    // Output GUI
    emit signal_newLogText( sMsg );
}
