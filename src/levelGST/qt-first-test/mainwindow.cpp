#include "mainwindow.h"
#include "ui_formMainWindow.h"

QvkMainWindow::QvkMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::formMainWindow)
{
    ui->setupUi(this);
}

QvkMainWindow::~QvkMainWindow()
{
    delete ui;
}

