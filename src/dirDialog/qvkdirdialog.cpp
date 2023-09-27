#include "qvkdirdialog.h"
#include "ui_qvkdirdialog.h"

#include <QDebug>
#include <QFileSystemModel>
#include <QTreeView>
#include <QStandardItem>
#include <QFileInfo>
#include <QStandardPaths>
#include <QListView>
#include <QList>
#include <QAbstractButton>

QvkDirDialog::QvkDirDialog(QWidget *parent) : QDialog(parent), ui(new Ui::QvkDirDialog)
{
    ui->setupUi(this);

    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setRootPath( "/" );
    fileSystemModel->setFilter( QDir::AllDirs | QDir::NoDotAndDotDot );
    fileSystemModel->sort( 0, Qt::AscendingOrder );

    ui->treeView->setModel( fileSystemModel );
    ui->treeView->setRootIsDecorated( true );
    ui->treeView->setExpandsOnDoubleClick( true );
    ui->treeView->hideColumn( 1 );
    ui->treeView->setColumnWidth( 0, 500 );
    QList<QAbstractButton *> list = ui->buttonBox->buttons();
    for ( int i = 0; i < list.count(); i++ ) {
        list.at(i)->setIcon( QIcon() );
    }

    QModelIndex index = fileSystemModel->index( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
    ui->treeView->setExpanded( index, true );
    ui->treeView->expandAll();

    connect( ui->treeView, SIGNAL( clicked( QModelIndex ) ), this, SLOT( slot_clicked( QModelIndex ) ) );
}

QvkDirDialog::~QvkDirDialog()
{
    delete ui;
}


void QvkDirDialog::slot_clicked( QModelIndex modelIndex )
{
    listCurrentDir.clear();
    listCurrentDir << fileSystemModel->fileInfo( modelIndex ).absoluteFilePath();
}


QStringList QvkDirDialog::selectedDir()
{
    return listCurrentDir;
}
