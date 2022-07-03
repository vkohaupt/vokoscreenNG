
#include <QGuiApplication>
#include <QImageWriter>
#include <QComboBox>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QThread>

#include "global.h"
#include "QvkSnapshot.h"
#include "QvkSpezialSlider.h"


QvkSnapshot::QvkSnapshot( QvkMainWindow *_vkMainWindow, Ui_formMainWindow *ui_formMainWindow )
{
    ui = ui_formMainWindow;
    vkMainWindow = _vkMainWindow;
    connect( ui->pushButtonSnapshot, SIGNAL( clicked() ), this, SLOT( slot_newImage() ) );

    supportedImageFormats();
    is_imageFolderExists_and_haveWritePermission();
}

QvkSnapshot::~QvkSnapshot()
{
}


void QvkSnapshot::supportedImageFormats()
{
    QList<QByteArray> listFormats = QImageWriter::supportedImageFormats();
    if ( listFormats.empty() == false )
    {
        for ( int x = 0; x < listFormats.count(); x++ )
        {
            ui->comboBoxSnapshotImageFormats->addItem( QString( listFormats.at(x) ) );
        }
    }

    ui->comboBoxSnapshotImageFormats->setCurrentIndex( ui->comboBoxSnapshotImageFormats->findText( "png" ) );
}


void QvkSnapshot::slot_newImage()
{
    bool bo = false;
    screen = QGuiApplication::screens();

    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        QImage image = screen.at( ui->comboBoxScreencastScreen->currentIndex() )->grabWindow(0).toImage();
        bo = image.save( ui->lineEditSnapshotImagePath->text() + "/" + screen.at( ui->comboBoxScreencastScreen->currentIndex() )->name() + "." +
                         ui->comboBoxSnapshotImageFormats->currentText().toUtf8(), ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {

        if (  vkMainWindow->vkRegionChoise->recordemode == false )
        {
            vkMainWindow->vkRegionChoise->recordMode( true );
            QvkSpezialSlider *spezialSlider = ui->centralWidget->findChild<QvkSpezialSlider *>( "sliderSecondWaitBeforeRecording" );
            QThread::msleep( static_cast<unsigned long>( spezialSlider->value()) * 1000 );
        }

        int startx = vkMainWindow->vkRegionChoise->x() + vkMainWindow->vkRegionChoise->getXRecordArea();
        int starty = vkMainWindow->vkRegionChoise->y() + vkMainWindow->vkRegionChoise->getYRecordArea();
        int endx = vkMainWindow->vkRegionChoise->getWidth();
        int endy = vkMainWindow->vkRegionChoise->getHeight();

        QImage image = screen.at( ui->comboBoxScreencastScreenArea->currentIndex() )->grabWindow(0).toImage();

        QImage copyImage = image.copy( startx, starty, endx, endy );
        bo = copyImage.save( ui->lineEditSnapshotImagePath->text() + "/" + screen.at( ui->comboBoxScreencastScreen->currentIndex() )->name() + "." +
                             ui->comboBoxSnapshotImageFormats->currentText().toUtf8(), ui->comboBoxSnapshotImageFormats->currentText().toUtf8() );

        if ( ui->pushButtonStart->isEnabled() == true )
        {
            vkMainWindow->vkRegionChoise->recordMode( false );
        }
    }

    if ( bo == false ) {
        qDebug().noquote() << global::nameOutput << "Failed to save image";
    }
}


bool QvkSnapshot::is_imageFolderExists_and_haveWritePermission()
{
    // Create Folder if not exists
    QDir dir( ui->lineEditSnapshotImagePath->text() );
    if ( !dir.exists() )
    {
        // check of QStandardPaths::MoviesLocation
        QDir dir( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
        if ( !dir.exists() )
        {
            bool myBool = dir.mkpath( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
            Q_UNUSED(myBool);
        }
        ui->lineEditSnapshotImagePath->setText( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
    } else {
        ui->lineEditSnapshotImagePath->setText( QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ) );
    }

    // Check write permission
    QString filename;
    filename = ui->lineEditSnapshotImagePath->text() + + "/vokoscreenNG-test-write.txt";
    bool value;
    QFileInfo fileInfo( filename );
    QFile file( filename );
    if ( file.open( QIODevice::ReadWrite ) )
    {
        QTextStream stream( &file );
        stream << "Test Test Test Test Test Test" << Qt::endl;
        file.close();
        qDebug().noquote();
        qDebug().noquote() << global::nameOutput << "Permission: Can write in" << fileInfo.absolutePath();
        qDebug().noquote();
        file.remove();
        value = true;
    } else {
        qDebug().noquote();
        qDebug().noquote() << "Permission: ERROR can not write in" << fileInfo.absolutePath();
        qDebug().noquote();
        QMessageBox *messageBox = new QMessageBox();
        QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowIcon( icon );
        messageBox->setWindowTitle( global::name + " " + global::version );
        messageBox->setIcon( QMessageBox::Critical );
        messageBox->setTextFormat( Qt::RichText );
        messageBox->setText( ( "<b>No write access on video folder</b>" ) );
        messageBox->setInformativeText( "vokoscreenNG can not create a video on<br>" + \
                                        fileInfo.absolutePath() + "<br><br>" + \
                                        "<b>Possible reasons:</b><br>" +
                                        "1. The folder is read-only<br>" +
                                        "2. Operating system security settings<br>" +
                                        "3. Antivirus program prevents writing<br><br>" +
                                        "<b>Please fix the problem and restart vokoscreenNG<b>"
                                       );
        messageBox->exec();
        value = false;
    }
    return value;
}
