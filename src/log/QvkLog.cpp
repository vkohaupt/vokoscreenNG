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

#include "QvkLog.h" 

#include <QDebug>
#include <QStandardPaths>
#include <QTime>

#include <QDir>

QvkLog::QvkLog( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    connect( this, SIGNAL( signal_newLogText( QString ) ), this, SLOT( slot_addLog( QString ) ) );

    QDateTime dateTime = QDateTime::currentDateTime();
    QString stringDateTime = dateTime.toString( "yyyy_MM_dd_hh_mm_ss" );
    path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

    QString logFolderName = path + "/" + "log";
    if( !QDir( logFolderName ).exists() )
    {
        QDir().mkpath( logFolderName );
    }

    logFile.setFileName( path + "/" + "log" + "/" + stringDateTime + ".log" );

    // If more 30 files, remove the last.
    QDir dir( logFolderName );
    dir.setFilter( QDir::Files );
    dir.setSorting( QDir::Name );
    QList<QFileInfo> list = dir.entryInfoList();
    if ( list.count() > 30 )
    {
        QFile file( list.at(0).absoluteFilePath() );
        file.remove();
    }
}


QvkLog::~QvkLog()
{
}


void QvkLog::slot_addLog( QString value )
{
    ui->textBrowserLog->append( value );
}


void QvkLog::writeToLog( QString string )
{
#ifdef Q_OS_LINUX
    QString eol = "\n";
#endif
#ifdef Q_OS_WIN
    QString eol = "\r\n";
#endif
    logFile.open( QIODevice::Append | QIODevice::Text | QIODevice::Unbuffered );
    logFile.write( string.toUtf8() );
    logFile.write( eol.toUtf8() );
    logFile.close();
}


void QvkLog::outputMessage( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    QByteArray time = QDateTime::currentDateTime().toString( "hh:mm:ss" ).toLocal8Bit();

    QString txt;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf( stderr, "%s %s\n", time.constData(), localMsg.constData() );
        //fprintf( stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += time + " " + localMsg.constData();
        break;
    case QtInfoMsg:
        fprintf( stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        break;
    case QtWarningMsg:
        fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        break;
    case QtCriticalMsg:
        fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        break;
    case QtFatalMsg:
        fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        abort();
    }
    writeToLog( txt );
    emit signal_newLogText( txt );
}
