#include "mainwindow.h"
#include "ui_formMainWindow.h"
#include "QvkAudioController.h"

QvkMainWindow::QvkMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::formMainWindow)
{
    ui->setupUi(this);

    QvkAudioController *vkAudioController = new QvkAudioController( ui );
    vkAudioController->init();
}

QvkMainWindow::~QvkMainWindow()
{
    delete ui;
}

