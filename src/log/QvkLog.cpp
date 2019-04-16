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
    QString stringDateTime = dateTime.toString( "yyyy-MM-dd_hh:mm:ss" );
    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

    QString logFolderName = path + "/" + "log";
    if( !QDir( logFolderName ).exists() )
    {
        QDir().mkdir( logFolderName );
    }

    logFile.setFileName( path + "/" + "log" + "/" + stringDateTime + ".log" );
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


void QvkLog::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf( stderr, "%s\n", localMsg.constData() );
        //fprintf( stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
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
