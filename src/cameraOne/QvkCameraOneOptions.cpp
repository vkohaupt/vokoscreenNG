#include "QvkCameraOneOptions.h"
#include "ui_QvkCameraOneOptions.h"

QvkCameraOneOptions::QvkCameraOneOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraOneOptions)
{
    ui->setupUi(this);
}

QvkCameraOneOptions::~QvkCameraOneOptions()
{
    delete ui;
}
