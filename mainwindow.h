#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    QString VK_gstr_Separator = " ! ";
    void mygstr();
    QString VK_GStreamer_Version();
    QString VK_getXimagesrc();
    QString VK_getMuxer();

    GstElement *pipeline;
    GError *error = NULL;

    GstElement *source, *videoconvert, *matroskamux, *filesink, *videorate;

    GstElement *VK_VideoEncoder;
    GstElement *VK_VideoQueue;

private slots:
    void VK_Start();
    void VK_Stop();

signals:
    void close();

protected:
    void closeEvent( QCloseEvent * event );


};

#endif // MAINWINDOW_H
