#include "QvkHelpBrowser_wl.h"
#include "ui_QvkHelpBrowser_wl.h"

QvkHelpBrowser_wl::QvkHelpBrowser_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkHelpBrowser_wl)
{
    ui->setupUi(this);
}

QvkHelpBrowser_wl::~QvkHelpBrowser_wl()
{
    delete ui;
}
