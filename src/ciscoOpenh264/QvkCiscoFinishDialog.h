#ifndef QVKCISCOFINISHDIALOG_H
#define QVKCISCOFINISHDIALOG_H

#include <QWidget>

namespace Ui {
class QvkCiscoFinishDialog;
}

class QvkCiscoFinishDialog : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCiscoFinishDialog(QWidget *parent = nullptr);
    ~QvkCiscoFinishDialog();

private:
    Ui::QvkCiscoFinishDialog *ui;


private slots:
    void slot_closeAllWindows();


};

#endif // QVKCISCOFINISHDIALOG_H
