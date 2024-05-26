#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui_formMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QvkMainWindow; }
QT_END_NAMESPACE

class QvkMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QvkMainWindow(QWidget *parent = nullptr);
    ~QvkMainWindow();

private:
//    Ui::QvkMainWindow *ui;
    Ui::formMainWindow *ui;
};
#endif // MAINWINDOW_H
