#include "QvkInformation.h"

#include <QDebug>
#include <QTimer>
#include <QStorageInfo>

QvkInformation::QvkInformation( Ui_formMainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    ui->labelVideoSize->setText("");
    ui->labelFreeSize->setText("");
    ui->labelInfoRecordTime->setText("00:00:00");

    // VideoSize and FreeDiskSpace
    timerStorageInfo = new QTimer(this);
    timerStorageInfo->setInterval( 1000 );
    connect( timerStorageInfo, SIGNAL( timeout() ), this, SLOT( slot_StorageInfo() ) );
    timerStorageInfo->start();

    // Recorded time
    elapsedTime = new QTime;
    connect( ui->pushButtonStart,    SIGNAL( clicked( bool ) ), this, SLOT( slot_timeFirstStart() ) );
    connect( ui->pushButtonPause,    SIGNAL( clicked( bool ) ), this, SLOT( slot_summedTimeAfterPause() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this, SLOT( slot_timeContinueStart() ) );

    timerRecord = new QTimer(this);
    timerRecord->setTimerType( Qt::PreciseTimer );
    timerRecord->setInterval( 1000 );
    connect( ui->pushButtonStart,    SIGNAL( clicked( bool ) ), timerRecord, SLOT( start() ) );
    connect( timerRecord,            SIGNAL( timeout() ),       this,        SLOT( slot_displayRecordTime() ) );
    connect( ui->pushButtonStop,     SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );
    connect( ui->pushButtonPause,    SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), timerRecord, SLOT( start() ) );
}


QvkInformation::~QvkInformation()
{
}


void QvkInformation::slot_timeFirstStart()
{
    elapsedTime->restart();
    ui->labelInfoRecordTime->setText("00:00:00");
}


void QvkInformation::slot_summedTimeAfterPause()
{
    int_summed += elapsedTime->elapsed();
}


void QvkInformation::slot_timeContinueStart()
{
    elapsedTime->restart();
}


void QvkInformation::slot_newVideoFilename( QString filename )
{
    newVideoFilename = filename;
}


void QvkInformation::slot_StorageInfo()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop or not start a record if disk space smaller 100MB(Default) 1GB(Max)
    if ( storage.bytesAvailable() <= ui->spinBoxDiskFreeSpace->text().toInt() * 1024 * 1024 )
    {
        ui->pushButtonStop->click();
    }

    ui->labelFreeSize->setText( QString::number( storage.bytesAvailable()/1024/1024 ) );

    QDir dir( ui->lineEditVideoPath->text() );
    QStringList filters;
    filters << newVideoFilename;
    QStringList videoFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    if ( !videoFileList.empty() )
    {
        QString string;
        string.append( ui->lineEditVideoPath->text() );
        string.append( "/" );
        string.append( videoFileList.at( 0 ) );
        QFileInfo file( string );
        file.refresh();
        ui->labelVideoSize->setText( QString::number( file.size()/1024 ) );
    }
}


void QvkInformation::slot_displayRecordTime()
{
   QTime time( 0, 0, 0, 0 );
   ui->labelInfoRecordTime->setText( time.addMSecs( elapsedTime->elapsed() + int_summed ).toString( "hh:mm:ss" ) );
}

