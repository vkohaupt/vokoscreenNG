#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <QCoreApplication>
#include <QNetworkReply>
#include <QTimer>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QSettings>

class QvkComposite: public QObject
{
    Q_OBJECT
public:
    QvkComposite();

public slots:
    void slot_doDownload();


private:
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloadsQList;
    QString remoteVersion;


private slots:
    void slot_downloadFinished( QNetworkReply *reply );

    
signals:

};

#endif // COMPOSITE_H
