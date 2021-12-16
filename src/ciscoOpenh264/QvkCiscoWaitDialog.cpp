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

void QvkCiscoWaitDialog::progress( qint64 downloadedSize, qint64 fileSize )
{
    ui->progressBar->setMinimum( 0 );
    ui->progressBar->setMaximum( fileSize );
    ui->progressBar->setValue( downloadedSize );

    update();
    repaint();
}
