
#include "QvkDirDialog.h"

#include <QDebug>
#include <QStandardPaths>
#include <QTableView>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QStackedWidget>
#include <QListView>
#include <QToolButton>

QvkDirDialog::QvkDirDialog( QWidget *parent )
{
    Q_UNUSED(parent)
    setModal( true );
    setFileMode( QFileDialog::Directory );
    setOption( QFileDialog::ShowDirsOnly, true );
    setOption( QFileDialog::DontUseNativeDialog, true );
    setOption( QFileDialog::DontUseCustomDirectoryIcons, true );
    setDirectory( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
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


// Wird benötigt für weitere tests
/*
    QList<QStackedWidget *> listWidgets = findChildren<QStackedWidget *>();
    if ( !listWidgets.empty() ) {
        listWidgets.at(0)->installEventFilter( this );
    }


    QList<QListView *> listWidgets = findChildren<QListView *>( "listView" );
    if ( !listWidgets.empty() ) {
        for (int x = 0; x < listWidgets.count(); x++) {
            listWidgets.at(x)->installEventFilter( this );
            qDebug() << listWidgets.at(x);
        }
    }

    QList<QToolButton *> listWidgets = findChildren<QToolButton *>();
    if ( !listWidgets.empty() ) {
        for (int x = 0; x < listWidgets.count(); x++) {
            if ( listWidgets.at(x)->objectName() == "backButton" )
               qDebug() << listWidgets.at(x)->icon();
        }
    }
*/
}


bool QvkDirDialog::eventFilter(QObject *object, QEvent *event)
{
    qDebug() << event->type();
    return QObject::eventFilter( object, event );
}
