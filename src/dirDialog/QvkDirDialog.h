#ifndef QVKDIRDIALOG_H
#define QVKDIRDIALOG_H

// Qt include checked
#include <QFileDialog>
#include <QModelIndex>

class QWidget;
class QDialog;
class QFileSystemModel;


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

};
#endif


#ifdef Q_OS_UNIX
namespace Ui {
    class QvkDirDialog;
}

class QvkDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QvkDirDialog(QWidget *parent = nullptr);
    ~QvkDirDialog();

    QStringList selectedFiles();
    QFileSystemModel *fileSystemModel;

private:
    Ui::QvkDirDialog *ui;
    QStringList listCurrentDir;

private slots:
    void slot_clicked( QModelIndex );

protected:

};
#endif

#endif // QVKDIRDIALOG_H
