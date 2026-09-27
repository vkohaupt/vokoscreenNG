#ifndef QVKCONVERTDIALOG_WL_H
#define QVKCONVERTDIALOG_WL_H

#include <QWidget>

namespace Ui {
class QvkConvertDialog_wl;
}

class QvkConvertDialog_wl : public QWidget
{
    Q_OBJECT

public:
    explicit QvkConvertDialog_wl(QWidget *parent = nullptr);
    ~QvkConvertDialog_wl();

private:
    Ui::QvkConvertDialog_wl *ui;
};

#endif // QVKCONVERTDIALOG_WL_H
