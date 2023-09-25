
#include "QvkFileDialog.h"

#include <QDebug>
#include <QStandardPaths>
#include <QTableView>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QStackedWidget>
#include <QListView>
#include <QToolButton>

QvkFileDialog::QvkFileDialog( QWidget *parent )
{
    Q_UNUSED(parent)
    setModal( true );
    setFileMode( QFileDialog::ExistingFile );
    setOption( QFileDialog::DontUseNativeDialog, true );
    setOption( QFileDialog::DontUseCustomDirectoryIcons, true );
    setWindowFlag( Qt::WindowContextHelpButtonHint, false );

    // Remove image in QDialogButtonBox
    QList<QDialogButtonBox *> list = findChildren<QDialogButtonBox *>();
    if ( !list.empty() ) {
        QList<QAbstractButton *> listButtons = list.at(0)->buttons();
        if ( !listButtons.empty() ) {
            for ( int x = 0; x < listButtons.count(); x++ ) {
                listButtons.at(x)->setIcon( QIcon( "" ) );
            }
        }
    }

    QList<QToolButton *> listWidgets = findChildren<QToolButton *>();
    if ( !listWidgets.empty() ) {
        for (int x = 0; x < listWidgets.count(); x++) {
            if ( listWidgets.at(x)->objectName() == "backButton" ) {
                listWidgets.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/backButton.png" ) );
            }
            if ( listWidgets.at(x)->objectName() == "forwardButton" ) {
                listWidgets.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/forwardButton.png" ) );
            }
            if ( listWidgets.at(x)->objectName() == "toParentButton" ) {
                listWidgets.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/toParentButton.png" ) );
            }
            if ( listWidgets.at(x)->objectName() == "newFolderButton" ) {
                listWidgets.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/newFolderButton.png" ) );
            }
        }
    }
}


void QvkFileDialog::setVideoPath( QString videoPath )
{
    setDirectory( videoPath );
}


bool QvkFileDialog::eventFilter(QObject *object, QEvent *event)
{
    qDebug() << event->type();
    return QObject::eventFilter( object, event );
}
