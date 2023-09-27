#ifndef QVKDIRDIALOG_H
#define QVKDIRDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QFileSystemModel>

#include <QFileDialog>

namespace Ui {
class QvkDirDialog;
}

class QvkDirDialog : public QFileDialog
{
    Q_OBJECT

public:
    explicit QvkDirDialog(QWidget *parent = nullptr);
    ~QvkDirDialog();

#ifdef Q_OS_LINUX
    QStringList selectedDir();
    QFileSystemModel *fileSystemModel;
#endif

private:
    Ui::QvkDirDialog *ui;

#ifdef Q_OS_LINUX
    QStringList listCurrentDir;
#endif


private slots:
#ifdef Q_OS_LINUX
    void slot_clicked( QModelIndex );
#endif

protected:


};

#endif // QVKDIRDIALOG_H
