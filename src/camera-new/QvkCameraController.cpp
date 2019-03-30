#include "ui_QvkCameraController.h"

#include "QvkCameraController.h"

#include <QMouseEvent>
#include <QDebug>

QvkCameraController::QvkCameraController( Ui_formMainWindow *ui_surface ) : ui( new Ui::QvkCameraController )
{
    ui->setupUi(this);
    ui_formMainWindow = ui_surface;
    ui->widgetVideo->setStyleSheet( "background-color:black;" );

    ui->widgetTab->hide();
    pixmap = new QPixmap( "/home/vk/Programmieren/vokoscreenNG/src/pictures/test.png"  );
    ui->labelVideo->resize( sliderValue().width(), sliderValue().height() );
    resize( sliderValue().width(), sliderValue().height() );
    show();
}

QvkCameraController::~QvkCameraController()
{
    delete ui;
}

void QvkCameraController::mousePressEvent( QMouseEvent *event )
{
    if ( ( ui->labelVideo->underMouse() == true ) and ( event->button() == Qt::RightButton ) )
    {
        if ( ui->widgetTab->isVisible() == true )
        {
            qDebug() << "Tab wird entfernt";
            qDeleteAll( ui->widgetTab->children() );
            delete ui->widgetTab;
            ui->labelVideo->setMinimumSize( QSize ( 1, 1 ) );
            resize( ui->labelVideo->width(), ui->labelVideo->height() );
            qDebug() << "mousePressEvent" << ui->labelVideo->width() << ui->labelVideo->height();
        }
        else
        {
            qDebug() << "Tab wird gesetzt";
            ui->labelVideo->setMinimumSize( QSize( width(), height() ) );
            ui->labelVideo->setPixmap( pixmap->scaled( ui->labelVideo->width(), ui->labelVideo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
            ui->widgetTab->show();
        }
    }
}

void QvkCameraController::resizeEvent( QResizeEvent *event )
{
    if ( ui->tabWidget != nullptr )
    {
        qDebug() << "resizeEvent";
        ui->labelVideo->setMinimumSize( QSize( 1, 1 ) );
        ui->labelVideo->setPixmap( pixmap->scaled( ui->labelVideo->width(), ui->labelVideo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
        qDebug() << "resizeEvent" << ui->labelVideo->width() << ui->labelVideo->height();
    }
}

void QvkCameraController::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( ( event->button() == Qt::LeftButton ) and ( ui->labelVideo->underMouse() == true ) )
    {
        if ( isFullScreen() == true )
        {
            showNormal();
        }
        else
        {
            setWindowState( Qt::WindowFullScreen );
            ui->widgetTab->hide();
        }
    }
}

QSize QvkCameraController::sliderValue()
{
    QSize size;
    if ( ui->sliderCameraWindowSize->value() == 1 )
    {
        size = QSize( 160, 120 );
    }

    if ( ui->sliderCameraWindowSize->value() == 2 )
    {
        size = QSize( 320, 240 );
    }

    if ( ui->sliderCameraWindowSize->value() == 3 )
    {
        size = QSize( 640, 480 );
    }

    return size;
}

