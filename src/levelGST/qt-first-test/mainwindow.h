#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QvkMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QvkMainWindow(QWidget *parent = nullptr);
    ~QvkMainWindow();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
