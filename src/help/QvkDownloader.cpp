/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

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
