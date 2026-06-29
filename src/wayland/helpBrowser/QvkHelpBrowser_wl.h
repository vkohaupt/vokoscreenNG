#ifndef QVKHELPBROWSER_WL_H
#define QVKHELPBROWSER_WL_H

#include "ui_formMainWindow_wl.h"

#include <QWidget>
#include <QString>
#include <QUrl>

namespace Ui {
class QvkHelpBrowser_wl;
}

class QvkHelpBrowser_wl : public QWidget
{
    Q_OBJECT

public:
    explicit QvkHelpBrowser_wl(QWidget *parent = nullptr);
    ~QvkHelpBrowser_wl();
    void set_GuiUi(Ui_formMainWindow_wl *ui);
    void set_init();
    void set_close();


private:
    Ui::QvkHelpBrowser_wl *ui;
    Ui_formMainWindow_wl *GuiUi;
    QString path;
    QString fileName;
    QUrl url;


protected:
    bool eventFilter(QObject *object, QEvent *ev) override;

};

#endif // QVKHELPBROWSER_WL_H
