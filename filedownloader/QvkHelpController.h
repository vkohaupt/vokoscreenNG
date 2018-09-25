#ifndef QvkHelpController_H 
#define QvkHelpController_H

#include "ui_mainwindow.h"
#include "QvkFileDownloader.h"

#include <QObject>

class QvkHelpController: public QObject
{
    Q_OBJECT
    
public:
    QvkHelpController( Ui_MainWindow *ui_mainwindow );
    virtual ~QvkHelpController();
    void init();

    
public slots:

  
private:
    Ui_MainWindow *ui;
    QvkFileDownloader *vkFileDownloader;
    QString vk_helpPath;


private slots:
    void slot_loadImage();
    void slot_screenFullscreen();


protected:

  
signals:

    
};

#endif
