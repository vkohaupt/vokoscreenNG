
#include "QvkFileDialog.h"

#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QList>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QToolButton>
#include <QIcon>
#include <QListView>
#include <QStackedWidget>
#include <QEvent>
#include <QObject>

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
        QDialogButtonBox *dialogButtonBox = list.at(0);
        QList<QAbstractButton *> listButtons = dialogButtonBox->buttons();
        if ( !listButtons.empty() ) {
            for ( int x = 0; x < listButtons.count(); x++ ) {
                QAbstractButton *abstractButton = listButtons.at(x);
                abstractButton->setIcon( QIcon() );
            }
        }
    }

    // Hide sidbar and widget for type
    QList<QWidget *> listWidget = findChildren<QWidget *>();
    if ( !listWidget.empty() ) {
        for ( int x = 0; x < listWidget.count(); x++ ) {
            QWidget *widget = listWidget.at(x);
            if ( widget->objectName() == "sidebar" ) {
                widget->hide();
            }
            if ( widget->objectName() == "fileTypeLabel" ) {
                widget->hide();
            }
            if ( widget->objectName() == "fileTypeCombo" ) {
                widget->hide();
            }
        }
    }

    // Set Buttons horizontal
    QList<QDialogButtonBox *> listPushButtons = findChildren<QDialogButtonBox *>();
    if ( !listPushButtons.empty() ) {
        for (int x = 0; x < listPushButtons.count(); x++) {
            QDialogButtonBox *dialogButtonBox = listPushButtons.at(x);
            dialogButtonBox->setOrientation( Qt::Horizontal );
        }
    }

    // Set custom icons
    QList<QToolButton *> listToolButton = findChildren<QToolButton *>();
    if ( !listToolButton.empty() ) {
        for ( int x = 0; x < listToolButton.count(); x++ ) {
            QToolButton *toolButton = listToolButton.at(x);
            if ( toolButton->objectName() == "backButton" ) {
                toolButton->setIcon( QIcon( ":/pictures/fileDirDialog/backButton.png" ) );
            }
            if ( toolButton->objectName() == "forwardButton" ) {
                toolButton->setIcon( QIcon( ":/pictures/fileDirDialog/forwardButton.png" ) );
            }
            if ( toolButton->objectName() == "toParentButton" ) {
                toolButton->setIcon( QIcon( ":/pictures/fileDirDialog/toParentButton.png" ) );
            }
            if ( toolButton->objectName() == "newFolderButton" ) {
                toolButton->setIcon( QIcon( ":/pictures/fileDirDialog/newFolderButton.png" ) );
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
