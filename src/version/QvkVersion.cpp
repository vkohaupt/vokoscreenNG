#include "QvkVersion.h"
#include "global.h"

QvkVersion::QvkVersion()
{
    connect( &manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( slot_downloadFinished( QNetworkReply* ) ) );
}


void QvkVersion::slot_doDownload( bool checked )
{
    if ( checked == false )
    {
        emit signal_newVersionAvailable( "" );
        return;
    }

#ifdef Q_OS_LINUX
    QNetworkRequest request( QUrl( "https://vokoscreen.volkoh.de/3.0/version/linux/VERSION" ) );
#endif
#ifdef Q_OS_WIN
    QNetworkRequest request( QUrl( "https://vokoscreen.volkoh.de/3.0/version/windows/VERSION" ) );
#endif
    manager.get( request );
}


void QvkVersion::slot_downloadFinished( QNetworkReply *reply )
{
    QUrl url = reply->url();
    if ( reply->error() )
    {
        qDebug().noquote() << global::nameOutput << "Download of" << url.toEncoded().constData() << "failed." << reply->errorString();
    }
    else
    {
       QString string = QString( reply->readAll() );
       QStringList stringList = string.split( "\n" );
       if ( !stringList.empty() )
       {
           QString update;
           int i = stringList.indexOf( QRegExp( "Version*", Qt::CaseInsensitive, QRegExp::Wildcard ) );
           if ( i > -1 )
           {
              update = stringList.at(i).section( "=", 1, 1 );
           }
           emit signal_newVersionAvailable( update );
       }
    }
}
