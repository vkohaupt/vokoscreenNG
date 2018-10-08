#include "QvkInformation.h"

#include <QDebug>
#include <QTimer>
#include <QStorageInfo>

QvkInformation::QvkInformation(Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;

    ui->labelVideoSize->setText("");
    ui->labelFreeSize->setText("");

    QTimer *timer = new QTimer(this);
    connect( timer, SIGNAL( timeout() ), this, SLOT( slot_systemInfo() ) );
    timer->start( 500 );
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
