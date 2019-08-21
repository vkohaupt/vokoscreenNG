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


private:
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloadsQList;
    QString remoteVersion;


private slots:
    void slot_doDownload();
    void slot_downloadFinished( QNetworkReply *reply );

    
signals:
    void signal_newVersionAvailable( QString version );

};

#endif
