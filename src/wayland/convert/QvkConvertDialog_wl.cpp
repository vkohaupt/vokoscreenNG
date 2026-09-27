#include "QvkConvertDialog_wl.h"
#include "ui_QvkConvertDialog_wl.h"

QvkConvertDialog_wl::QvkConvertDialog_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkConvertDialog_wl)
{
    ui->setupUi(this);
}

QvkConvertDialog_wl::~QvkConvertDialog_wl()
{
    delete ui;
}
