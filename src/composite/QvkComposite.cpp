#include "QvkComposite.h"
#include "global.h"

#include <QX11Info>

QvkComposite::QvkComposite()
{
    connect( &manager, SIGNAL( finished( QNetworkReply* ) ), SLOT( slot_downloadFinished( QNetworkReply* ) ) );
}


void QvkComposite::slot_doDownload()
{
    if ( QX11Info::isCompositingManagerRunning() == true )
    {
        QNetworkRequest request( QUrl( "https://vokoscreen.volkoh.de/3.0/version/linux/COMPOSITE-ON" ) );
        manager.get( request );
    } else
    {
        QNetworkRequest request( QUrl( "https://vokoscreen.volkoh.de/3.0/version/linux/COMPOSITE-OFF" ) );
        manager.get( request );
    }
}


void QvkComposite::slot_downloadFinished( QNetworkReply *reply )
{
    QUrl url = reply->url();
    if ( reply->error() )
    {
        //qDebug().noquote() << global::nameOutput << "Download of" << url.toEncoded().constData() << "failed" << reply->errorString();
    }
    else
    {
        // Read data and make nothing
        //qDebug().noquote() << global::nameOutput << "Download of" << url.toEncoded().constData() << "successful";
        QString string = QString( reply->readAll() );
        Q_UNUSED(string)
    }
}
