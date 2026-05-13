#include "QvkCameraSingle_wl.h"
#include "ui_QvkCameraSingle_wl.h"

QvkCameraSingle_wl::QvkCameraSingle_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkCameraSingle_wl)
{
    ui->setupUi(this);
}

QvkCameraSingle_wl::~QvkCameraSingle_wl()
{
    delete ui;
}
