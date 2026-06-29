#include "QvkHelpBrowser_wl.h"
#include "ui_QvkHelpBrowser_wl.h"

#include <QWidget>
#include <QToolButton>
#include <QUrl>
#include <QDialogButtonBox>

QvkHelpBrowser_wl::QvkHelpBrowser_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkHelpBrowser_wl)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    resize(800, 600);
    path = "https://vokoscreen.volkoh.de/3.0/help/de_DE/screencast/";

    // Is needed only for the translated text
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->hide();
    QList<QPushButton *> list = buttonBox->findChildren<QPushButton *>();
    ui->pushButtonClose->setText( list.at(0)->text() );

}


QvkHelpBrowser_wl::~QvkHelpBrowser_wl()
{
    delete ui;
}


void QvkHelpBrowser_wl::set_GuiUi(Ui_formMainWindow_wl *ui)
{
    GuiUi = ui;
}


void QvkHelpBrowser_wl::init()
{
    connect(GuiUi->help_screencast_countdown,
            &QToolButton::clicked,
            this,
            [=](){
        fileName = GuiUi->help_screencast_countdown->objectName() + ".html";
        url = path + fileName;
        ui->webEngineView->setUrl(url);
        ui->labelURL->setText(url.toString());
        show();}
    );
}
