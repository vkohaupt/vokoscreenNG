/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2025 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
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

#include "QvkDownloader_wl.h"
#include "global.h"

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDateTime>
#include <QSettings>
#include <QByteArray>
#include <QFileInfo>
#include <QIODevice>
#include <QFile>
#include <QFileInfo>


QvkDownloader_wl::QvkDownloader_wl( QString pathLocal , QObject *parent ) : QObject(parent)
{
    connect(&networkAccessManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *value){slot_downloadFinished(value);});
    tempPath = pathLocal;
}


void QvkDownloader_wl::doDownload( const QUrl &url )
{
    QNetworkRequest request;
    request.setSslConfiguration( QSslConfiguration::defaultConfiguration() );
    request.setUrl( url );

    QDateTime time;
    time.setMSecsSinceEpoch( QDateTime::currentDateTime().currentMSecsSinceEpoch() );
    QSettings installSetting( QSettings::IniFormat, QSettings::UserScope, global::name + "_wl", QString( "InstallTime" ), Q_NULLPTR );
    installSetting.beginGroup( global::name + "_wl" );
    QString timeStringMSecsSinceEpoch = installSetting.value( "time", time.toString( "yyyy.MM.dd-hh:mm:ss:zzz" ) ).toString();
    QString version = installSetting.value( "version", global::version ).toString();
    QByteArray headerValue = timeStringMSecsSinceEpoch.append( "_" ).append( version ).toLatin1();
    installSetting.endGroup();
    request.setRawHeader( "User-Agent", headerValue );
    QNetworkReply *reply = networkAccessManager.get( request );
    listDownloads.append( reply );
}


bool QvkDownloader_wl::saveLocal( const QString &filename, QIODevice *data )
{
    QString fileInTempPath = tempPath + "/" + filename;
    QFile file( fileInTempPath );
    if ( !file.open( QIODevice::WriteOnly ) ) {
        return false;
    }
    file.write( data->readAll() );
    file.close();
    return true;
}


void QvkDownloader_wl::slot_downloadFinished( QNetworkReply *reply )
{
    QString filename = QFileInfo( reply->url().path() ).fileName();
    if ( reply->error() ) {
        //qDebug().noquote() << global::nameOutput << "Download of" << reply->url().toString() << "failed:" << reply->errorString();
    } else {
        if ( saveLocal( filename, reply ) ) {
            //qDebug().noquote() << global::nameOutput << "Download of" << reply->url().toString() << "succeeded (saved to" << tempPath + "/" + filename + ")";
        }
    }

    listDownloads.removeAll( reply );
    reply->deleteLater();

    if ( listDownloads.isEmpty() ) {
        // all downloads finished
        emit signal_fileDownloaded( tempPath + "/" + filename );
    }
}
