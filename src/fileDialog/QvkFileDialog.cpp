
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
