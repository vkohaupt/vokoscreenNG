#ifndef QvkHelp_H 
#define QvkHelp_H

#include "ui_mainwindow.h"
#include "ui_formhelp.h"
#include "QvkDownloader.h"

#include <QObject>
#include <QTemporaryDir>

class QvkHelp: public QWidget
{
    Q_OBJECT
    
public:
    QvkHelp( QMainWindow *mainWindow, Ui_MainWindow *ui_mainwindow );
    virtual ~QvkHelp();

    
public slots:

  
private:
    Ui_MainWindow *ui;
    Ui_Form *uiForm;

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
    void slot_screenFullscreen();
    void slot_screenWindow();
    void slot_screenArea();
    void slot_screenMagnifier();
    void slot_screenShowclick();
    void slot_screenCountdown();

    void slot_audioHelp();

    void slot_parseHTML(QString tempPathFileName);
    void slot_showHelp(QString tempPathFileName);

    void slot_close();

protected:
  
signals:

    
};

#endif
