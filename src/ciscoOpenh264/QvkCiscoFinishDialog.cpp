#include "QvkCiscoFinishDialog.h"
#include "ui_QvkCiscoFinishDialog.h"

QvkCiscoFinishDialog::QvkCiscoFinishDialog(QWidget *parent) : QWidget(parent), ui(new Ui::QvkCiscoFinishDialog)
{
    ui->setupUi(this);

    connect( ui->buttonBox, SIGNAL( accepted() ), this, SLOT( slot_closeAllWindows() ) );
}


QvkCiscoFinishDialog::~QvkCiscoFinishDialog()
{
    delete ui;
}


void QvkCiscoFinishDialog::slot_closeAllWindows()
{
    qApp->closeAllWindows();
}
