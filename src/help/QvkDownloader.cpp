#include "QvkDownloader.h"

QvkDownloader::QvkDownloader(QObject *parent) : QObject(parent)
{
    connect( &networkAccessManager, SIGNAL( finished( QNetworkReply* ) ), SLOT( slot_downloadFinished( QNetworkReply* ) ) );
    tempPath = QStandardPaths::writableLocation( QStandardPaths::TempLocation );
}


void QvkDownloader::doDownload( const QUrl &url )
{
    QNetworkRequest request( url );
    QNetworkReply *reply = networkAccessManager.get( request );
    listDownloads.append( reply );
}


bool QvkDownloader::saveLocal( const QString &filename, QIODevice *data )
{
    QString fileInTempPath = tempPath + "/" + filename;
    QFile file( fileInTempPath );
    if ( !file.open( QIODevice::WriteOnly ) )
    {
        qDebug( "[vokoscreen] Could not open %s for writing: %s\n", qPrintable( filename ), qPrintable( file.errorString() ) );
        return false;
    }
    file.write( data->readAll() );
    file.close();
    return true;
}


void QvkDownloader::slot_downloadFinished( QNetworkReply *reply )
{
    QString filename = QFileInfo( reply->url().path() ).fileName();
    if ( reply->error() )
    {
        qDebug( "[vokoscreen] Download of %s failed: %s\n", reply->url().toEncoded().constData(), qPrintable(reply->errorString() ) );
    }
    else
    {
        if ( saveLocal( filename, reply ) )
        {
            qDebug( "[vokoscreen] Download of %s succeeded (saved to %s)", reply->url().toEncoded().constData(), qPrintable( tempPath + "/" + filename) );
        }
    }

    listDownloads.removeAll( reply );
    reply->deleteLater();

    if ( listDownloads.isEmpty() )
    {
        // all downloads finished
        emit signal_fileDownloaded( tempPath + "/" + filename );
    }
}
