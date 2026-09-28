#include "global.h"
#include "QvkConvertDialog_wl.h"
#include "ui_QvkConvertDialog_wl.h"

#include <QDialogButtonBox>
#include <QList>
#include <QPushButton>

QvkConvertDialog_wl::QvkConvertDialog_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkConvertDialog_wl)
{
    ui->setupUi(this);
    setWindowTitle(global::name + " " + global::version);

    // Is needed only for the translated text
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->hide();
    QList<QPushButton *> list = buttonBox->findChildren<QPushButton *>();
    ui->pushButtonClose->setText(list.at(0)->text());

    connect(ui->pushButtonClose, &QPushButton::clicked, this, [=](){close();});
}

QvkConvertDialog_wl::~QvkConvertDialog_wl()
{
    delete ui;
}
