#include "QvkFileDownloader.h"

// https://wiki.qt.io/Download_Data_from_URL

QvkFileDownloader::QvkFileDownloader(QUrl url, QObject *parent) : QObject(parent)
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
    DownloadedData = pReply->readAll();
    pReply->deleteLater();
    emit signal_downloaded();
}


QByteArray QvkFileDownloader::downloadedData() const
{
    return DownloadedData;
}

