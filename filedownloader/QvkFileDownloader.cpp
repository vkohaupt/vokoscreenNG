#include "QvkFileDownloader.h"

// https://wiki.qt.io/Download_Data_from_URL

//QvkFileDownloader::QvkFileDownloader(QUrl url, QObject *parent) : QObject(parent)
QvkFileDownloader::QvkFileDownloader( QObject *parent) : QObject(parent)
{
}


void QvkFileDownloader::setMyUrl( QUrl url )
{
    connect( &networkAccessManager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( slot_fileDownloaded( QNetworkReply*) ) );
    QNetworkRequest request( url );
    networkAccessManager.get( request );
}


QvkFileDownloader::~QvkFileDownloader()
{
}


void QvkFileDownloader::slot_fileDownloaded( QNetworkReply* pReply )
{
    qDebug() << "1111111111111111111111111111111111111111111111111" << pReply->url();
    DownloadedData = pReply->readAll();
    //pReply->deleteLater();
    emit signal_downloaded();
}


QByteArray QvkFileDownloader::downloadedData() const
{
    return DownloadedData;
}

