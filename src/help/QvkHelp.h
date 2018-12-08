#ifndef QvkHelp_H 
#define QvkHelp_H

#include "ui_formMainWindow.h"
#include "ui_formhelp.h"
#include "QvkDownloader.h"

#include <QObject>
#include <QTemporaryDir>

class QvkHelp: public QWidget
{
    Q_OBJECT
    
public:
    QvkHelp( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow );
    virtual ~QvkHelp();

    
public slots:

  
private:
    Ui_formMainWindow *ui;
    Ui_formHelp *uiFormHelp;

    QStringList toDownloadFiles = { ".png", ".svg", ".css", ".jpg" };
    bool isFileInLine( QString line );

    QvkDownloader *vkDownloadHTML;
    QvkDownloader *vkDownloadFiles;

    void loadHTML(QString value );
    QString remotePath;
    QString remoteBasename;
    int getCountFileToDownload( QString tempPathFileName );
    QStringList localFiles;
    QString tempPath;
    QString vk_helpPath;

    QTemporaryDir temporaryDirLocal;

private slots:
    void slot_toolButtonHelpFullscreen();
    void slot_toolButtonHelpWindow();
    void slot_toolButtonHelpArea();
    void slot_toolButtonHelpNoMouseCursor();
    void slot_toolButtonHelpScale();
    void slot_toolButtonHelpMagnifier();
    void slot_toolButtonHelpShowclick();
    void slot_toolButtonHelpCountdown();
    void slot_toolButtonHelpExecute();

    void slot_audioHelp();

    void slot_parseHTML(QString tempPathFileName);
    void slot_showHelp(QString tempPathFileName);

    void slot_close();

protected:

signals:

    
};

#endif
