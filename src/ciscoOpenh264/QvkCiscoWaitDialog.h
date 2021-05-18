#ifndef QVKCISCOWAITDIALOG_H
#define QVKCISCOWAITDIALOG_H

#include <QWidget>

namespace Ui {
class QvkCiscoWaitDialog;
}

class QvkCiscoWaitDialog : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCiscoWaitDialog(QWidget *parent = nullptr);
    ~QvkCiscoWaitDialog();

private:
    Ui::QvkCiscoWaitDialog *ui;


private slots:


};

#endif
