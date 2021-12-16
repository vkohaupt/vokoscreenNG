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

    void progress(qint64 downloadedSize, qint64 fileSize );

private:
    Ui::QvkCiscoWaitDialog *ui;


private slots:


};

#endif
