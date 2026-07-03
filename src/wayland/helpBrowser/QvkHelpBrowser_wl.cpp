
#include "global.h"
#include "QvkHelpBrowser_wl.h"
#include "ui_QvkHelpBrowser_wl.h"

#include <QObject>
#include <QWidget>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QToolButton>
#include <QPushButton>
#include <QUrl>
#include <QDialogButtonBox>
#include <QAction>
#include <QList>
#include <QSettings>
#include <QTimer>
#include <QDateTime>
#include <QByteArray>
#include <QEvent>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEnginePage>


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

    connect(ui->webEngineView,
            &QWebEngineView::loadStarted,
            this,
            [=]()
    {
        QWebEnginePage *webEnginePage = ui->webEngineView->page();
        QString m_url = webEnginePage->url().toString();
        if (m_url.contains("Wayland") == true){
            qDebug().noquote() << global::nameOutput << "[Help] Load begin " << m_url;
        }
    });

    connect(ui->webEngineView,
            &QWebEngineView::loadFinished,
            this,
            [=](bool value)
    {
        QWebEnginePage *webEnginePage = ui->webEngineView->page();
        QString m_url = webEnginePage->url().toString();
        if (m_url.contains("Wayland") == true){
            if (value == true){
                qDebug().noquote() << global::nameOutput << "[Help] Load finish" << m_url;
                ui->webEngineView->page()->toPlainText([](const QString &text){
                    qDebug().noquote() << global::nameOutput << "[Help] Content:" << text;
                });
            }else{
                qDebug().noquote() << global::nameOutput << "[Help] Load error" << m_url;
            }
        }
    });

    ui->webEngineView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    // https://felgo.com/doc/qt/qtwebengine-webenginewidgets-simplebrowser-example/

    QString folderName_wl = global::name;
    QString fileName_wl = "InstallTime";
    QString groupName_wl = global::name;
    QDateTime time;
    time.setMSecsSinceEpoch(QDateTime::currentDateTime().currentMSecsSinceEpoch());
    QSettings installSetting(QSettings::IniFormat, QSettings::UserScope, folderName_wl, fileName_wl, Q_NULLPTR);
    installSetting.beginGroup(groupName_wl);
    QString timeStringMSecsSinceEpoch = installSetting.value("time", time.toString("yyyy.MM.dd-hh:mm:ss:zzz")).toString();
    QString version = installSetting.value("version", global::version).toString();
    QByteArray headerValue = timeStringMSecsSinceEpoch.append("_").append(version).toLatin1();
    installSetting.endGroup();

    // Die InstallTime.ini wird ausgelesen und im Log als UserAgent angezeigt
    ui->webEngineView->page()->profile()->setHttpUserAgent(headerValue);

    // Die getLinuxDirs.php und die getWaylandDirs.php werden im log nur angezeigt
    // wenn man diese zeitlich versetzt aufruft
    QTimer::singleShot(100, this, [=](){
        ui->webEngineView->setUrl(QUrl("https://vokoscreen.volkoh.de/3.0/help/getLinuxDirs.php"));
    });

    QTimer::singleShot(2000, this, [=](){
        ui->webEngineView->setUrl(QUrl("https://vokoscreen.volkoh.de/3.0/help/getWaylandDirs.php"));
    });
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
