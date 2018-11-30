#include <QPointer>
#include <QMessageLogContext>
#include <QString>

#include "QvkLogController.h" 
#include "QvkLog.h"

QPointer<QvkLog> vklog;
void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    vklog->outputMessage( type, context, msg );
}


QvkLogController::QvkLogController( Ui_formMainWindow *ui_mainwindow )
{
    vklog = new QvkLog( ui_mainwindow );
    qInstallMessageHandler( myMessageOutput );
}


QvkLogController::~QvkLogController()
{
}
