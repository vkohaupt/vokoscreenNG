#include "QvkDownloader.h"
#include "global.h"

QvkDownloader::QvkDownloader( QString pathLocal , QObject *parent ) : QObject(parent)
{
    connect( &networkAccessManager, SIGNAL( finished( QNetworkReply* ) ), SLOT( slot_downloadFinished( QNetworkReply* ) ) );
    tempPath = pathLocal;
}



void QvkDownloader::doDownload( const QUrl &url )
{
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setUrl( url );
    QNetworkReply *reply = networkAccessManager.get( request );
    listDownloads.append( reply );
}


bool QvkDownloader::saveLocal( const QString &filename, QIODevice *data )
{
    QString fileInTempPath = tempPath + "/" + filename;
    QFile file( fileInTempPath );
    if ( !file.open( QIODevice::WriteOnly ) )
    {
        // qDebug().noquote() << global::nameOutput << "Could not open" << filename << "for writing:" << file.errorString();
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
        //qDebug().noquote() << global::nameOutput << "Download of" << reply->url().toString() << "failed:" << reply->errorString();
    }
    else
    {
        if ( saveLocal( filename, reply ) )
        {
            //qDebug().noquote() << global::nameOutput << "Download of" << reply->url().toString() << "succeeded (saved to" << tempPath + "/" + filename + ")";
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
