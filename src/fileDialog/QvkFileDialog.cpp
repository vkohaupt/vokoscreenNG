
#include "QvkFileDialog.h"

#include <QDebug>
#include <QStandardPaths>
#include <QTableView>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QStackedWidget>
#include <QListView>

QvkFileDialog::QvkFileDialog( QWidget *parent )
{
    Q_UNUSED(parent)
    setModal( true );
    setFileMode( QFileDialog::ExistingFile );
    setOption( QFileDialog::DontUseNativeDialog, true );
    setOption( QFileDialog::DontUseCustomDirectoryIcons, true );

    // Remove image in QDialogButtonBox
    QList<QDialogButtonBox *> list = findChildren<QDialogButtonBox *>();
    if ( !list.empty() )
    {
        QList<QAbstractButton *> listButtons = list.at(0)->buttons();
        if ( !listButtons.empty() )
        {
            for ( int x = 0; x < listButtons.count(); x++ )
            {
                listButtons.at(x)->setIcon( QIcon( "" ) );
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
