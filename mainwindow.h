#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QvkRegionController.h"
#include "QvkWinInfo.h"

#include <QMainWindow>

#include <gst/gst.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QvkRegionController *regionController;
    const QString VK_Gstr_Separator = " ! ";
    QDesktopWidget *desk = QApplication::desktop();
    QString VK_GStreamer_Version();
    QString VK_getXimagesrc();
    QString VK_getMuxer();

    GstElement *pipeline;
    GError *error = NULL;

    QvkWinInfo *vkWinInfo;

private slots:
    void VK_preStart();
    void VK_Start();
    void VK_Stop();
    void VK_Pause();
    void VK_Continue();
    void myScreenCountChanged( int newCount );


signals:
    void signal_close();


protected:
    void closeEvent( QCloseEvent * event );


};

#endif // MAINWINDOW_H
