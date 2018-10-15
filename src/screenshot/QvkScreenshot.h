#ifndef QvkScreenshot_H
#define QvkScreenshot_H

#include "ui_mainwindow.h"
#include "mainwindow.h"

class QvkScreenshot: public QObject
{ 
    
    Q_OBJECT

public:
    QvkScreenshot(MainWindow *value, Ui_MainWindow *ui_mainwindow);
    virtual ~QvkScreenshot();


public slots:


private slots: 
    void slot_formats_Screenshot();
    void slot_preshot_Screenshot();
    void slot_singleShot_Screenshot();
    void slot_shot_Screenshot();
    void slot_show_Screenshoot();
    void slot_Screenshot_count_changed( int newCount );
    void slot_newPicturePath();
    void slot_pictureFileSystemWatcherSetNewPath();
    void slot_pictureFileSystemWatcherSetButtons();
    void slot_resizeEvent( QResizeEvent *event );


signals:
    void signal_finish_screenshot( bool );


protected:  


private:
    Ui_MainWindow *ui;
    QvkWinInfo *vkWinInfo;
    QvkCountdown *vkCountdown;
    QvkRegionChoise *regionChoise;

    MainWindow *parent;
    QPixmap pixmap;
    QFileSystemWatcher *pictureFileSystemWatcher;

};

#endif
