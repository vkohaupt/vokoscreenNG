#ifndef QvkScreenshot_H
#define QvkScreenshot_H

#include "ui_formMainWindow.h"
#include "mainwindow.h"

class QvkScreenshot: public QObject
{ 
    
    Q_OBJECT

public:
    QvkScreenshot( QvkMainWindow *value, Ui_formMainWindow *ui_mainwindow);
    virtual ~QvkScreenshot();


public slots:


private slots: 
    void slot_formats_Screenshot();
    void slot_preshot_Screenshot();
    void slot_singleShot_Screenshot();
    void slot_shot_Screenshot();
    void slot_show_Screenshoot();
    void slot_Folder();
//    void slot_screen_count_changed();
    void slot_newPicturePath();
    void slot_pictureFileSystemWatcherSetNewPath();
    void slot_pictureFileSystemWatcherSetButtons();
    void slot_resizeEvent( QResizeEvent *event );


signals:
    void signal_finish_screenshot( bool );


protected:  


private:
    Ui_formMainWindow *ui;
    QvkWinInfo *vkWinInfo;
    QvkCountdown *vkCountdown;
    QvkRegionChoise *regionChoise;

    QvkMainWindow *parent;
    QPixmap pixmap;
    QFileSystemWatcher *pictureFileSystemWatcher;

    void makeScreenshotByStart();
};

#endif
