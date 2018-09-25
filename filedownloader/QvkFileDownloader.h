#ifndef QvkFileDownloader_H
#define QvkFileDownloader_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class QvkFileDownloader : public QObject
{
    Q_OBJECT
public:
    QvkFileDownloader(QUrl url, QObject *parent = 0);
    virtual ~QvkFileDownloader();
    QByteArray downloadedData() const;


signals:
    void signal_downloaded();


private slots:
    void slot_fileDownloaded(QNetworkReply* pReply);


private:
    QNetworkAccessManager networkAccessManager;
    QByteArray DownloadedData;
};

#endif
