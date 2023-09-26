
#include "QvkDirDialog.h"

#include <QDebug>
#include <QStandardPaths>
#include <QTableView>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QStackedWidget>
#include <QListView>
#include <QToolButton>
#include <QPushButton>

QvkDirDialog::QvkDirDialog( QWidget *parent )
{
    Q_UNUSED(parent)
    setModal( true );

#ifdef Q_OS_WIN
    setFileMode( QFileDialog::Directory );
    setOption( QFileDialog::ShowDirsOnly, true );
    setDirectory( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
#endif

#ifdef Q_OS_LINUX
    setFileMode( QFileDialog::Directory );
    setOption( QFileDialog::ShowDirsOnly, true );
    setOption( QFileDialog::DontUseNativeDialog, true );
    setOption( QFileDialog::DontUseCustomDirectoryIcons, true );
    setDirectory( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
    setWindowFlag( Qt::WindowContextHelpButtonHint, false );
#endif

}
