#include <QPointer>

#include "QvkLogController.h" 
#include "QvkLog.h"

QPointer<QvkLog> vklog;
void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    vklog->outputMessage( type, context, msg );
}


QvkLogController::QvkLogController( Ui_MainWindow *ui_mainwindow )
{
    ui = ui_mainwindow;
    vklog = new QvkLog( ui );
    qInstallMessageHandler( myMessageOutput );
}


QvkLogController::~QvkLogController()
{
}
