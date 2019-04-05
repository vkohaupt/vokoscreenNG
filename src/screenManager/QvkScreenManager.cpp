#include "QvkScreenManager.h"
#include "global.h"

#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

QvkScreenManager::QvkScreenManager()
{
    connect( qApp, SIGNAL( screenAdded( QScreen* ) ),   this, SLOT( slot_screen_count_changed() ) );
    connect( qApp, SIGNAL( screenRemoved( QScreen* ) ), this, SLOT( slot_screen_count_changed() ) );
}


QvkScreenManager::~QvkScreenManager()
{}

void QvkScreenManager::slot_screen_count_changed()
{
    QList<QScreen *> screen = QGuiApplication::screens();
    if ( screen.empty() == false )
    {
        emit signal_clear_widget();
        for ( int i = 0; i < screen.size(); i++ )
        {
            qDebug().noquote() << global::nameOutput << "Name from screen: " << screen.at(i)->name();
            qDebug().noquote() << global::nameOutput << "Screen available desktop width :" << QString::number( screen.at(i)->geometry().width() * screen.at(i)->devicePixelRatio() );
            qDebug().noquote() << global::nameOutput << "Screen available desktop height:" << QString::number( screen.at(i)->geometry().height() * screen.at(i)->devicePixelRatio() );
            qDebug().noquote() << global::nameOutput << "DevicePixelRatio:" << screen.at(i)->devicePixelRatio() << " (Normal displays is 1, Retina display is 2)";
            qDebug().noquote() << global::nameOutput << "Vertical refresh rate of the screen in Hz:" << screen.at(i)->refreshRate();
            qDebug().noquote() << global::nameOutput << "Screen orientation" << screen.at(i)->orientation();
            qDebug().noquote() << global::nameOutput << "Color depth of the screen: " << screen.at(i)->depth();
            qDebug().noquote() << global::nameOutput << "Model from screen: " << screen.at(i)->model();
            qDebug().noquote() << global::nameOutput << "Manufactur from screen: " << screen.at(i)->manufacturer();
            qDebug().noquote() << global::nameOutput << "SerialNumber from screen: " << screen.at(i)->serialNumber();

            QString X = QString::number( screen.at(i)->geometry().left() * screen.at(i)->devicePixelRatio() );
            QString Y = QString::number( screen.at(i)->geometry().top() * screen.at(i)->devicePixelRatio() );
            QString Width = QString::number( screen.at(i)->geometry().width() * screen.at(i)->devicePixelRatio() );
            QString Height = QString::number( screen.at(i)->geometry().height() * screen.at(i)->devicePixelRatio() );
            QString stringText = screen.at(i)->name() + " " + ":  " + Width + " x " + Height;
            QString stringData = "x=" + X + " " +
                                 "y=" + Y + " " +
                                 "with=" + Width + " " +
                                 "height=" + Height;
            emit signal_screen_count_changed( stringText, stringData );
        }
    }
}
