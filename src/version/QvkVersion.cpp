#include "QvkVersion.h"
#include "global.h"

QvkVersion::QvkVersion()
{
    connect( &manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( downloadFinished( QNetworkReply* ) ) );
}


void QvkVersion::doDownload()
{
    // Man sollte evtl. unterscheiden ob ein update z.b 3.0.1 oder eine neue Version 4.0.0 verfügbar ist.
#ifdef Q_OS_LINUX
    QNetworkRequest request( QUrl( "http://vokoscreen.volkoh.de/3.0/version/linux/VERSION" ) );
#endif
#ifdef Q_OS_WIN
    QNetworkRequest request( QUrl( "http://vokoscreen.volkoh.de/3.0/version/windows/VERSION" ) );
#endif
    manager.get( request );
}


void QvkVersion::downloadFinished( QNetworkReply *reply )
{
    QUrl url = reply->url();
    if ( reply->error() )
    {
        fprintf( stderr, "[vokoscreenNG] Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable( reply->errorString() ) );
    }
    else
    {
       QString string = QString( reply->readAll() );
       QStringList stringList = string.split( "\n" );
       if ( !stringList.empty() )
       {
           int i = stringList.indexOf( QRegExp( "Version*", Qt::CaseInsensitive, QRegExp::Wildcard ) );
           QString newVersion = stringList.at(i).section( "=", 1, 1 );
           QString currentVersion = global::version.section( " ", 0, 0 );

           if ( newVersion > currentVersion )
           {
               emit newVersionAvailable( newVersion );
           }
       }
    }
}
