#ifndef QVKDIRDIALOG_H
#define QVKDIRDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QFileSystemModel>

#include <QFileDialog>

#ifdef Q_OS_WIN
namespace Ui {
class QvkDirDialog;
}

class QvkDirDialog : public QFileDialog
{
    Q_OBJECT

public:
    explicit QvkDirDialog(QWidget *parent = nullptr);
    ~QvkDirDialog();

private:
    Ui::QvkDirDialog *ui;

private slots:

protected:
#endif


#ifdef Q_OS_LINUX
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
#endif

};

#endif // QVKDIRDIALOG_H
