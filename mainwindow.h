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
    void mygstr();
    QString VK_GStreamer_Version();

    GstElement *pipeline, *source, *videoconvert, *x264enc, *matroskamux, *filesink;


private slots:
    void VK_Start();
    void VK_Stop();


};

#endif // MAINWINDOW_H
