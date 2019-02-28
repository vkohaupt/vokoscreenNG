#include "QvkGlobalShortcut.h"

QvkGlobalShortcut::QvkGlobalShortcut(QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow )
{
    Q_UNUSED(mainWindow);
    ui = ui_mainwindow;

    QGlobalShortcut *shortcutWebcam = new QGlobalShortcut( this );
    connect( shortcutWebcam, SIGNAL( activated() ), ui->checkBoxCameraOnOff, SLOT( click() ) );
    shortcutWebcam->setShortcut( QKeySequence( "Ctrl+Shift+F8" ) );

    QGlobalShortcut *shortcutMagnifier = new QGlobalShortcut( this );
    connect( shortcutMagnifier, SIGNAL( activated() ), ui->checkBoxMagnifier, SLOT( click() ) );
    shortcutMagnifier->setShortcut( QKeySequence( "Ctrl+Shift+F9" ) );

    QGlobalShortcut *shortcutStart = new QGlobalShortcut( this );
    connect( shortcutStart, SIGNAL( activated() ), ui->pushButtonStart, SLOT( click() ) );
    shortcutStart->setShortcut( QKeySequence( "Ctrl+Shift+F10" ) );

    QGlobalShortcut *shortcutStop = new QGlobalShortcut( this );
    connect( shortcutStop, SIGNAL( activated() ), ui->pushButtonStop, SLOT( click() ) );
    shortcutStop->setShortcut( QKeySequence( "Ctrl+Shift+F11" ) );

    QGlobalShortcut *shortcutPauseContinue = new QGlobalShortcut( this );
    connect( shortcutPauseContinue, SIGNAL( activated() ), this, SLOT( slot_pauseContinue() ) );
    shortcutPauseContinue->setShortcut( QKeySequence( "Ctrl+Shift+F12" ) );
}


QvkGlobalShortcut::~QvkGlobalShortcut()
{
}


void QvkGlobalShortcut::slot_pauseContinue()
{
    if ( ui->pushButtonContinue->isHidden() == true )
    {
        ui->pushButtonPause->click();
    }
    else
    {
        ui->pushButtonContinue->click();
    }
}
