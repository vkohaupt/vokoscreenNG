
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

    // Start with empty history
    QStringList stringList;
    setHistory( stringList );

    // Remove image in QDialogButtonBox
    QList<QDialogButtonBox *> list = findChildren<QDialogButtonBox *>();
    if ( !list.empty() ) {
        QList<QAbstractButton *> listButtons = list.at(0)->buttons();
        if ( !listButtons.empty() ) {
            for ( int x = 0; x < listButtons.count(); x++ ) {
                listButtons.at(x)->setIcon( QIcon() );
            }
        }
    }

    // Hide sidbar and widget for type
    QList<QWidget *> listWidget = findChildren<QWidget *>();
    if ( !listWidget.empty() ) {
        for ( int x = 0; x < listWidget.count(); x++ ) {
            if ( listWidget.at(x)->objectName() == "sidebar" ) {
                listWidget.at(x)->hide();
            }
            if ( listWidget.at(x)->objectName() == "fileTypeLabel" ) {
                listWidget.at(x)->hide();
            }
            if ( listWidget.at(x)->objectName() == "fileTypeCombo" ) {
                listWidget.at(x)->hide();
            }
        }
    }

    // Set Buttons horizontal
    QList<QDialogButtonBox *> listPushButtons = findChildren<QDialogButtonBox *>();
    if ( !listPushButtons.empty() ) {
        for (int x = 0; x < listPushButtons.count(); x++) {
            listPushButtons.at(x)->setOrientation( Qt::Horizontal );
        }
    }

    // Set custom icons
    QList<QToolButton *> listToolButton = findChildren<QToolButton *>();
    if ( !listToolButton.empty() ) {
        for ( int x = 0; x < listToolButton.count(); x++ ) {
            if ( listToolButton.at(x)->objectName() == "backButton" ) {
                listToolButton.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/backButton.png" ) );
            }
            if ( listToolButton.at(x)->objectName() == "forwardButton" ) {
                listToolButton.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/forwardButton.png" ) );
            }
            if ( listToolButton.at(x)->objectName() == "toParentButton" ) {
                listToolButton.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/toParentButton.png" ) );
            }
            if ( listToolButton.at(x)->objectName() == "newFolderButton" ) {
                listToolButton.at(x)->setIcon( QIcon( ":/pictures/fileDirDialog/newFolderButton.png" ) );
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
*/
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
