#include "QvkLog.h" 

#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QTime>

#include <stdio.h>

QvkLog::QvkLog( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    connect( this, SIGNAL( signal_newLogText( QString ) ), this, SLOT( slot_addLogVokoscreen( QString ) ) );

    QDateTime dateTime = QDateTime::currentDateTime();
    QString stringDateTime = dateTime.toString( "yyyy-MM-dd_hh:mm:ss" );

    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );
    logFile.setFileName( path + "/" + stringDateTime + ".log");
}


QvkLog::~QvkLog()
{
}


void QvkLog::slot_addLogVokoscreen( QString value )
{
    ui->textBrowserLog->append( value );
}


void QvkLog::writeToLog( QString string )
{
#ifdef Q_OS_LINUX
    QString endLine = "\n";
#endif
#ifdef Q_OS_WIN
    QString endLine = "\r\n";
#endif
    logFile.open( QIODevice::Append | QIODevice::Text | QIODevice::Unbuffered );
    logFile.write( string.toUtf8() );
    logFile.write( endLine.toUtf8() );
    logFile.close();
}


void QvkLog::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf( stderr, "%s\n", localMsg.constData() );
        txt += localMsg.constData();
        break;
    case QtInfoMsg:
        fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
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
