
#include <QGuiApplication>
#include <QImageWriter>
#include <QComboBox>

#include "global.h"
#include "QvkSnapshot.h"


QvkSnapshot::QvkSnapshot( Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;
    connect( ui->pushButtonSnapshot, SIGNAL( clicked() ), this, SLOT( slot_newImage() ) );

    screens();
    supportedImageFormats();
}

QvkSnapshot::~QvkSnapshot()
{
}


void QvkSnapshot::screens()
{
    screen = QGuiApplication::screens();
    if ( !screen.empty() )
    {
        for ( int i = 0; i < screen.count(); i++ )
        {
            QString X = QString::number( static_cast<int>( screen.at(i)->geometry().left() * screen.at(i)->devicePixelRatio() ) );
            QString Y = QString::number( static_cast<int>( screen.at(i)->geometry().top() * screen.at(i)->devicePixelRatio() ) );
            QString Width = QString::number( static_cast<int>( screen.at(i)->geometry().width() * screen.at(i)->devicePixelRatio() ) );
            QString Height = QString::number( static_cast<int>( screen.at(i)->geometry().height() * screen.at(i)->devicePixelRatio() ) );
            QString stringText = screen.at(i)->name() + " " + ":  " + Width + " x " + Height;
            QString stringData = "x=" + X + " " +
                                 "y=" + Y + " " +
                                 "with=" + Width + " " +
                                 "height=" + Height;

            ui->comboBoxSnapshotScreens->addItem( stringText );
        }
    }
}


void QvkSnapshot::supportedImageFormats()
{
    QList<QByteArray> listFormats = QImageWriter::supportedImageFormats();
    if ( listFormats.empty() == false )
    {
        for ( int x = 0; x < listFormats.count(); x++ )
        {
            ui->comboBoxImageFormats->addItem( QString( listFormats.at(x) ) );
        }
    }

    ui->comboBoxImageFormats->setCurrentIndex( ui->comboBoxImageFormats->findText( "png" ) );
}


void QvkSnapshot::slot_newImage()
{
    QImage image = screen.at( ui->comboBoxSnapshotScreens->currentIndex() )->grabWindow(0).toImage();
    bool bo = image.save( "/home/vk/Bilder/" + screen.at( ui->comboBoxSnapshotScreens->currentIndex() )->name() + "." +
              ui->comboBoxImageFormats->currentText().toUtf8(), ui->comboBoxImageFormats->currentText().toUtf8() );
    if ( bo == false ) {
        qDebug().noquote() << global::nameOutput << "Failed to save image";
    }
}
