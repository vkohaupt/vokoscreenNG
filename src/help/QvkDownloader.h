#ifndef QvkDownloader_H
#define QvkDownloader_H

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

class QvkDownloader : public QObject
{
    Q_OBJECT
    QNetworkAccessManager networkAccessManager;
    QVector<QNetworkReply *> listDownloads;


public:
    explicit QvkDownloader( QObject *parent = nullptr );
    void doDownload( const QUrl &url );


private:
    QString tempPath;
    bool saveLocal(const QString &filename, QIODevice *data );
    void execute();


signals:
    void signal_fileDownloaded( QString );


public slots:
    void slot_downloadFinished( QNetworkReply *reply );


private slots:


};

#endif // QvkDownloader_H
