/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2021 Volker Kohaupt
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

#include "QvkCiscoOpenh264Downloader.h"
#include "global.h"

QvkCiscoOpenh264Downloader::QvkCiscoOpenh264Downloader( QString vk_pathLocal , QObject *parent ) : QObject(parent)
{
    connect( &networkAccessManager, SIGNAL( finished( QNetworkReply* ) ), SLOT( slot_downloadFinished( QNetworkReply* ) ) );
    pathLocal = vk_pathLocal;
}


void QvkCiscoOpenh264Downloader::doDownload( const QUrl &url )
{
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setUrl( url );
    QNetworkReply *reply = networkAccessManager.get( request );


    connect( reply, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( slot_downloadProgress( qint64, qint64 ) ) );


    listDownloads.append( reply );
}



void QvkCiscoOpenh264Downloader::slot_downloadProgress( qint64 downloadedSize, qint64 fileSize )
{
    emit signal_downloadProgress( downloadedSize, fileSize );
}



bool QvkCiscoOpenh264Downloader::saveLocal( const QString &filename, QIODevice *data )
{
    QString fileInTempPath = pathLocal + "/" + filename;
    QFile file( fileInTempPath );
    if ( !file.open( QIODevice::WriteOnly ) )
    {
        return false;
    }
    file.write( data->readAll() );
    file.close();
    return true;
}


void QvkCiscoOpenh264Downloader::slot_downloadFinished( QNetworkReply *reply )
{
    qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toUInt() << "---------------------------------------";
    QString filename = QFileInfo( reply->url().path() ).fileName();
    bool downloadOK = false;
    if ( reply->error() )
    {
        qDebug().noquote() << global::nameOutput << "[h264] Download of" << reply->url().toString() << "failed:" << reply->errorString();
        downloadOK = false;
    }
    else
    {
        if ( saveLocal( filename, reply ) == true )
        {
            qDebug().noquote() << global::nameOutput << "[h264] Download of" << reply->url().toString() << "succeeded (saved to" << pathLocal + "/" + filename + ")";
            downloadOK = true;
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "[h264] Download of" << reply->url().toString() << "can not save" << pathLocal + "/" + filename + ")";
            downloadOK = false;
        }
    }

    listDownloads.removeAll( reply );
    reply->deleteLater();

    if ( downloadOK == true )
    {
        emit signal_fileDownloaded( pathLocal + "/" + filename );
    }

    if ( downloadOK == false )
    {
        emit signal_failedDownload();
    }
}
