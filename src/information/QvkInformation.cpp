#include "QvkInformation.h"

#include <QDebug>
#include <QTimer>
#include <QStorageInfo>

QvkInformation::QvkInformation(Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    ui->labelVideoSize->setText("");
    ui->labelFreeSize->setText("");
    ui->labelInfoRecordTime->setText("00:00:00");

    QTimer *timer = new QTimer(this);
    timer->setInterval( 500 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_systemInfo() ) );
    timer->start();

    timerRecord = new QTimer(this);
    timerRecord->setInterval( 1000 );
    connect( ui->pushButtonStart,    SIGNAL( clicked( bool ) ), this,        SLOT( slot_recordTimeInit() ) );
    connect( ui->pushButtonStart,    SIGNAL( clicked( bool ) ), timerRecord, SLOT( start() ) );
    connect( timerRecord,            SIGNAL( timeout() ),       this,        SLOT( slot_recordTimeStart() ) );
    connect( ui->pushButtonStop,     SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );
    connect( ui->pushButtonPause,    SIGNAL( clicked( bool ) ), timerRecord, SLOT( stop() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), timerRecord, SLOT( start() ) );
}

QvkInformation::~QvkInformation()
{
}


void QvkInformation::slot_newVideoFilename( QString filename )
{
    newVideoFilename = filename;
}


void QvkInformation::slot_systemInfo()
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


void QvkInformation::slot_recordTimeInit()
{
    recordCounter = 0;
}


void QvkInformation::slot_recordTimeStart()
{
   recordCounter++;
   QTime time( 0, 0, 0, 0 );
   ui->labelInfoRecordTime->setText( time.addSecs( recordCounter ).toString( "hh:mm:ss" ) );
}
