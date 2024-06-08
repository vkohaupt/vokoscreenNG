#ifndef VERSION_H 
#define VERSION_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>


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
    void slot_doDownload( bool checked );
    void slot_downloadFinished( QNetworkReply *reply );

    
signals:
    void signal_newVersionAvailable( QString update );

};

#endif // VERSION_H
