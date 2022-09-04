/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
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

#ifndef QVKCISCOOPEN264DOWNLOADER_H
#define QVKCISCOOPEN264DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QStringList>
#include <QUrl>
#include <QStandardPaths>
#include <QVector>

class QvkCiscoOpenh264Downloader : public QObject
{
    Q_OBJECT
    QNetworkAccessManager networkAccessManager;
    QVector<QNetworkReply *> listDownloads;


public:
    explicit QvkCiscoOpenh264Downloader( QString vk_pathLocal, QObject *parent = nullptr);
    void doDownload( const QUrl &url );


private:
    QString pathLocal;
    bool saveLocal(const QString &filename, QIODevice *data );
    void execute();


signals:
    void signal_fileDownloaded( QString );
    void signal_failedDownload();
    void signal_downloadProgress( qint64 downloadedSize, qint64 fileSize );


public slots:
    void slot_downloadFinished( QNetworkReply *reply );
    void slot_downloadProgress( qint64 downloadedSize, qint64 fileSize );


private slots:


};

#endif // QvkDownloader_H
