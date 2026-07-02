#include "QvkHelpBrowser_wl.h"
#include "ui_QvkHelpBrowser_wl.h"

#include <QWidget>
#include <QToolButton>
#include <QUrl>
#include <QDialogButtonBox>
#include <QAction>
#include <QList>

/*
 * The remote HTML-file and the toolbutton have the same name.
 *
 * Example:
 * QToolbutton -> help_screencast_fullscreen
 * Remote file -> help_screencast_fullscreen.html
 *
 * We write as snake_case, so the underline character is the separator.
 * The first section is help, this we need for set for icon and installEventFilter, see constructor.
 * The second section is the tab in the sidebar and the subdir on remote.
 * The third section is the name from the function that we want help.
 * See also QvkHelpBrowser_wl::eventFilter(QObject *object, QEvent *event)
 * and void QvkHelpBrowser_wl::init()
 */

QvkHelpBrowser_wl::QvkHelpBrowser_wl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QvkHelpBrowser_wl)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    resize(800, 800);
    path = "https://vokoscreen.volkoh.de/3.0/helpwayland/de_DE/screencast/";

    QIcon icon;
    icon.addFile(QString::fromUtf8(":/pictures/logo/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    // Is needed only for the translated text
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->hide();
    QList<QPushButton *> list = buttonBox->findChildren<QPushButton *>();
    ui->pushButtonClose->setText(list.at(0)->text());

    connect(ui->pushButtonClose, &QPushButton::clicked, this, [=](){close();});

    ui->webEngineView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    // https://felgo.com/doc/qt/qtwebengine-webenginewidgets-simplebrowser-example/
}


QvkHelpBrowser_wl::~QvkHelpBrowser_wl()
{
    delete ui;
}


bool QvkHelpBrowser_wl::eventFilter(QObject *object, QEvent *event)
{
    QToolButton *toolButton = qobject_cast<QToolButton *>(object);
    if ((event->type() == QEvent::MouseButtonRelease) and
        (toolButton->isDown() == false)){
        return false;
    }

    if ((event->type() == QEvent::MouseButtonRelease) and
        (toolButton->isEnabled() == true)){
        fileName = toolButton->objectName() + ".html";
        url = path + fileName;
        ui->webEngineView->setUrl(url);
        ui->labelURL->setText(url.toString());
        show();
    }
    return false;
}


void QvkHelpBrowser_wl::set_GuiUi(Ui_formMainWindow_wl *ui)
{
    GuiUi = ui;
}


void QvkHelpBrowser_wl::set_init()
{
    QList<QToolButton *> listToolButton = GuiUi->centralwidget->findChildren<QToolButton *>();
    for (int i = 0; i < listToolButton.count(); i++){
        QToolButton *toolButton = listToolButton.at(i);
        if (toolButton->objectName().startsWith( "help_")){
            toolButton->setIcon(QIcon(":/pictures/help/information.png"));
            toolButton->setIconSize(QSize(20,20));
            toolButton->installEventFilter(this);
        }
    }
}


void QvkHelpBrowser_wl::set_close()
{
    close();
    ui->webEngineView->setPage(nullptr);
}
