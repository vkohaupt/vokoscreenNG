#include "QvkCiscoWaitDialog.h"
#include "ui_QvkCiscoWaitDialog.h"

QvkCiscoWaitDialog::QvkCiscoWaitDialog(QWidget *parent) : QWidget(parent), ui(new Ui::QvkCiscoWaitDialog)
{
    ui->setupUi(this);
}


QvkCiscoWaitDialog::~QvkCiscoWaitDialog()
{
    delete ui;
}
