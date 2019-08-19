#ifndef version_H 
#define version_H

#include <QCoreApplication>
#include <QNetworkReply>
#include <QTimer>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QSettings>

class QvkVersion: public QObject
{
    Q_OBJECT
public:
    QvkVersion();

public slots:
    QString getRemoteVersion();
    bool isNewVersionAvailable( QString localVersion, QString remoteVersion );
  
private:
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloadsQList;
    bool saveToDisk( const QString &filename, QIODevice *data );
    QString remoteVersion;

private slots:
    void doDownload();
    void downloadFinished( QNetworkReply *reply );
    void readVersionTempFile( QString localVersionFilename );
    void setRemoteVersion( QString version );
    
signals:
    void versionDownloadFinish();
};

#endif