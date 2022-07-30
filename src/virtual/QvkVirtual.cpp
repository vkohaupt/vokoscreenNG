#include "QvkVirtual.h"
#include "global.h"

#include <QProcess>
#include <QDebug>

QvkVirtual::QvkVirtual()
{
}


#ifdef Q_OS_LINUX
QString QvkVirtual::isVirtualMaschine()
{
    QString maschine = "systemd-detect-virt not found";

    QProcess process;
    process.setProcessChannelMode( QProcess::MergedChannels );
    process.start( "systemd-detect-virt", QStringList() );

    if ( process.waitForFinished( 30000 ) )
    {
        QString text( process.readAll() );
        maschine = text.trimmed();
    }

    return maschine;
}
#endif


#ifdef Q_OS_WIN
QString QvkVirtual::isVirtualMaschine()
{
    QString maschine = "Nativ";

    QProcess process;
    process.setProcessChannelMode( QProcess::MergedChannels );
    QStringList list;
    list.append( "computersystem" );
    list.append( "get" );
    list.append( "manufacturer,name,model" );
    process.start( "C:/WINDOWS/System32/Wbem/WMIC.exe", list );

    if ( process.waitForFinished( 30000 ) )
    {
        QString text( process.readAll() );
        text = text.section( "\n", 1 );
        text = text.replace( "\r", "" );
        maschine = text.replace( "\n", "" );
    }

    return maschine;
}
#endif

/*
QProcess wmic;

wmic.setProcessChannelMode(QProcess::MergedChannels);
wmic.start("wmic", QStringList() << "computersystem" << "get" << "manufacturer,name");
if(wmic.waitForFinished(10000))
{
    QString text(wmic.readAll());
    bool isVM = text.contains("VMware") || text.contains("VirtualBox") || text.contains("Virtual Machine");
    qDebug() << text;
}
*/
