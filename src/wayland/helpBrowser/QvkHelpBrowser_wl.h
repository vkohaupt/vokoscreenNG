#ifndef QVKHELPBROWSER_WL_H
#define QVKHELPBROWSER_WL_H

#include <QWidget>

namespace Ui {
class QvkHelpBrowser_wl;
}

class QvkHelpBrowser_wl : public QWidget
{
    Q_OBJECT

public:
    explicit QvkHelpBrowser_wl(QWidget *parent = nullptr);
    ~QvkHelpBrowser_wl();

private:
    Ui::QvkHelpBrowser_wl *ui;
};

#endif // QVKHELPBROWSER_WL_H
