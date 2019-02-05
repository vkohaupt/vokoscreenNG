#ifndef QvkHelp_H 
#define QvkHelp_H

#include "ui_formMainWindow.h"
#include "ui_help.h"
#include "ui_player.h"
#include "QvkDownloader.h"

#include <QObject>
#include <QTemporaryDir>

#include <QMouseEvent>
#include <QEvent>


class QvkHelp: public QWidget
{
    Q_OBJECT
    
public:
    QvkHelp(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, Ui_player *ui_player);
    virtual ~QvkHelp();
    

public slots:

  
private:
    Ui_formMainWindow *ui;
    Ui_help *uiHelp;
    Ui_player *uiPlayer;

    QStringList toDownloadFiles = { ".png", ".svg", ".css", ".jpg" };
    bool isFileInLine( QString line );

    QvkDownloader *vkDownloadHTML;
    QvkDownloader *vkDownloadFiles;

    void loadHTML( QString value );
    QString remotePath;
    QString remoteBasename;
    int getCountFileToDownload( QString tempPathFileName );
    QStringList localFiles;
    QString tempPath;
    QString vk_helpPath;

    QTemporaryDir temporaryDirLocal;


private slots:
    void slot_parseHTML( QString tempPathFileName );
    void slot_showHelp( QString tempPathFileName );

    void slot_cleanUp();


protected:
    bool eventFilter(QObject *object, QEvent *ev) override;


signals:

    
};

#endif
