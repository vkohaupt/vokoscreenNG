#ifndef QVKDIRDIALOG_H
#define QVKDIRDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QFileSystemModel>

namespace Ui {
class QvkDirDialog;
}

class QvkDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QvkDirDialog(QWidget *parent = nullptr);
    ~QvkDirDialog();
    QStringList selectedDir();
    QFileSystemModel *fileSystemModel;

private:
    Ui::QvkDirDialog *ui;
    QStringList listCurrentDir;


private slots:
    void slot_clicked( QModelIndex );


protected:


};

#endif // QVKDIRDIALOG_H
