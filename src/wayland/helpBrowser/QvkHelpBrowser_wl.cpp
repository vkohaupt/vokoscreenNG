#include "QvkHelpBrowser_wl.h"
#include "ui_QvkHelpBrowser_wl.h"

QvkHelpBrowser_wl::QvkHelpBrowser_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkHelpBrowser_wl)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
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
        QUrl url("https://vokoscreen.volkoh.de/3.0/help/de_DE/screencast/help_screencast_countdown.html");
        ui->webEngineView->setUrl(url);
        this->show();}
    );
}
