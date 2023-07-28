#include "QvkSystrayAlternative.h"
#include "global.h"
#include "QvkSpezialCheckbox.h"

#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>

QvkSystrayAlternative::QvkSystrayAlternative( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *gui_sliderShowInSystrayAlternative )
{
    ui = ui_mainwindow;
    sliderShowInSystrayAlternative = gui_sliderShowInSystrayAlternative;

    connect( sliderShowInSystrayAlternative, SIGNAL( valueChanged( int ) ), this, SLOT( slot_resizeWindow( int ) ) );
    connect( ui->toolButtonShowInSystrayAlternativeReset, SIGNAL( clicked( bool ) ), this, SLOT( slot_ShowInSystrayAlternativeReset( bool ) ) );

    size = QSize( sliderShowInSystrayAlternative->value(), sliderShowInSystrayAlternative->value() );

    vkSystrayAlternativeWindow = new QvkSystrayAlternativeWindow( this );
    vkSystrayAlternativeWindow->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip );

    slot_setSystrayIcon( true );

    QAction *titleAction = new QAction( this );
    titleAction->setIcon( QIcon( ":pictures/systray/systray.png" ) );
    titleAction->setText( global::name + " " + global::version );
    titleAction->setEnabled( false );

    startAction = new QAction( this );
    startAction->setIcon( QIcon( ":/pictures/player/start.png" ) );
//    startAction->setText( tr( "Start" ) );
    startAction->setData( "Start" );

    stopAction = new QAction( this );
    stopAction->setIcon( QIcon( ":/pictures/player/stop.png" ) );
//    stopAction->setText( tr( "Stop" ) );
    stopAction->setData( "Stop" );
    stopAction->setEnabled( false );

    pauseAction = new QAction( this );
    pauseAction->setIcon( QIcon( ":/pictures/player/pause.png" ) );
//    pauseAction->setText( tr( "Pause" ) );
    pauseAction->setData( "Pause" );
    pauseAction->setEnabled( false );

    continueAction = new QAction( this );
    continueAction->setIcon( QIcon( ":/pictures/player/start.png" ));
//    continueAction->setText( tr( "Continue" ) );
    continueAction->setData( "Continue" );
    continueAction->setEnabled( false );

    cameraAction = new QAction( this );
    cameraAction->setIcon( QIcon( ":pictures/systray/camera.png" ) );
//    cameraAction->setText( tr( "Camera" ) );
    cameraAction->setData( "Camera" );
    cameraAction->setCheckable( true );
    cameraAction->setEnabled( false );

    magnifierAction = new QAction( this );
    magnifierAction->setIcon( QIcon( ":pictures/systray/magnification.png" ) );
//    magnifierAction->setText( tr( "Magnification" ) );
    magnifierAction->setData( "Magnification" );
    magnifierAction->setCheckable( true );

    showclickAction = new QAction( this );
    showclickAction->setIcon( QIcon( ":pictures/systray/showclick.png" ) );
    showclickAction->setText( "ShowClick" );
    showclickAction->setData( "ShowClick" );
    showclickAction->setCheckable( true );

    haloAction = new QAction( this );
    haloAction->setIcon( QIcon( ":pictures/systray/showclick.png" ) );
    haloAction->setText( "Halo" );
    haloAction->setData( "Halo" );
    haloAction->setCheckable( true );

    snapshotAction = new QAction( this );
    snapshotAction->setIcon( QIcon( ":pictures/systray/snapshot.png" ) );
    snapshotAction->setText( "Snapshot" );
    snapshotAction->setData( "Snapshot" );
    snapshotAction->setCheckable( false );

    exitAction = new QAction( this );
    exitAction->setIcon( QIcon( ":/pictures/systray/exit.png" ) );
    exitAction->setText( tr( "Exit" ) );
    exitAction->setData( "Exit" );

    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setRecordIcon( bool ) ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), startAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setSystrayIcon( bool ) ) );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), continueAction, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setPauseIcon( bool ) ) );

    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), startAction,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), stopAction,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), pauseAction,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), continueAction, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this,           SLOT( slot_setRecordIcon( bool ) ) );

    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ),  startAction, SLOT( setDisabled( bool ) ) );

    connect( startAction,    SIGNAL( triggered( bool ) ), ui->pushButtonStart,    SLOT( click() ) );
    connect( stopAction,     SIGNAL( triggered( bool ) ), ui->pushButtonStop,     SLOT( click() ) );
    connect( pauseAction,    SIGNAL( triggered( bool ) ), ui->pushButtonPause,    SLOT( click() ) );
    connect( continueAction, SIGNAL( triggered( bool ) ), ui->pushButtonContinue, SLOT( click() ) );
    connect( snapshotAction, SIGNAL( triggered( bool ) ), ui->pushButtonSnapshotSnapshot, SLOT( click() ) );
    connect( exitAction,     SIGNAL( triggered( bool ) ), this,                   SLOT( close() ) );
    connect( exitAction,     SIGNAL( triggered( bool ) ), mainWindow,             SLOT( close() ) );

    connect( ui->checkBoxCameraOnOff, SIGNAL( toggled( bool ) ),   cameraAction,            SLOT( setChecked( bool ) ) );
    connect( cameraAction,            SIGNAL( triggered( bool ) ), ui->checkBoxCameraOnOff, SLOT( setChecked( bool ) ) );
    connect( ui->comboBoxCamera,      SIGNAL( currentIndexChanged( int ) ), this,           SLOT( slot_currentIndexChanged( int ) ) );

    connect( ui->checkBoxMagnifier, SIGNAL( toggled( bool ) ),   magnifierAction, SLOT( setChecked( bool ) ) );
    connect( magnifierAction,       SIGNAL( triggered( bool ) ), ui->checkBoxMagnifier, SLOT( click() ) );

    QList<QvkSpezialCheckbox *> listSpezialCheckbox = ui->centralWidget->findChildren<QvkSpezialCheckbox *>();
    for ( int i = 0; i < listSpezialCheckbox.count(); i++ )
    {
        if ( listSpezialCheckbox.at(i)->objectName() == "spezialCheckboxShowclick" )
        {
            connect( listSpezialCheckbox.at(i), SIGNAL( signal_clicked( bool ) ), showclickAction, SLOT( setChecked( bool ) ) );
            connect( showclickAction,           SIGNAL( triggered( bool ) ),      listSpezialCheckbox.at(i), SLOT( slot_click() ) );
        }

        if ( listSpezialCheckbox.at(i)->objectName() == "spezialCheckboxHalo" )
        {
            connect( listSpezialCheckbox.at(i), SIGNAL( signal_clicked( bool ) ), haloAction, SLOT( setChecked( bool ) ) );
            connect( haloAction,                SIGNAL( triggered( bool ) ),      listSpezialCheckbox.at(i), SLOT( slot_click() ) );
        }
    }

    menu = new QMenu();
    menu->addAction( titleAction );
    menu->addSeparator();
    menu->addAction( startAction );
    menu->addAction( stopAction );
    menu->addAction( pauseAction );
    menu->addAction( continueAction );
    menu->addSeparator();
    menu->addAction( cameraAction );
    menu->addAction( magnifierAction );
    menu->addAction( showclickAction );
    menu->addAction( haloAction );
    menu->addAction( snapshotAction );
    menu->addSeparator();
    menu->addAction( exitAction );

    vkSystrayAlternativeWindow->set_Menu( menu );

    setMenueText();
}


QvkSystrayAlternative::~QvkSystrayAlternative()
{
}


void QvkSystrayAlternative::setMenueText()
{
    startAction->setText( tr( "Start" ) );
    stopAction->setText( tr( "Stop" ) );
    pauseAction->setText( tr( "Pause" ) );
    continueAction->setText( tr( "Continue" ) );
    cameraAction->setText( tr( "Camera" ) );
    magnifierAction->setText( tr( "Magnification" ) );
    exitAction->setText( tr( "Exit" ) );
}


void QvkSystrayAlternative::slot_currentIndexChanged( int index )
{
    if ( index > -1 )
        cameraAction->setEnabled( true );
    else
        cameraAction->setEnabled( false );
}


void QvkSystrayAlternative::setVisible( bool value )
{
    vkSystrayAlternativeWindow->setVisible( value );
}


void QvkSystrayAlternative::slot_resizeWindow( int value )
{
    size = QSize( value, value );

    if ( ui->pushButtonStart->isEnabled() == true )
    {
        slot_setSystrayIcon( true );
    }

    if ( ui->pushButtonStop->isEnabled() == true )
    {
        slot_setRecordIcon( true );
    }

    if ( ui->pushButtonContinue->isEnabled() == true )
    {
        slot_setPauseIcon( true );
    }
}


void QvkSystrayAlternative::slot_setRecordIcon( bool )
{
    QPixmap pixmap( ":/pictures/systray/record.png" );
    pixmap = pixmap.scaled( size , Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    vkSystrayAlternativeWindow->set_Size( size, pixmap );
}


void QvkSystrayAlternative::slot_setSystrayIcon( bool )
{
    QPixmap pixmap( ":/pictures/systray/systray.png"  );
    pixmap = pixmap.scaled( size , Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    vkSystrayAlternativeWindow->set_Size( size, pixmap );
}


void QvkSystrayAlternative::slot_setPauseIcon( bool )
{
    QPixmap pixmap( ":/pictures/systray/pause.png"  );
    pixmap = pixmap.scaled( size , Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    vkSystrayAlternativeWindow->set_Size( size, pixmap );
}


void QvkSystrayAlternative::slot_ShowInSystrayAlternativeReset( bool )
{
    vkSystrayAlternativeWindow->move( 0, 0 );
    sliderShowInSystrayAlternative->setValue( 48 );
}


void QvkSystrayAlternative::slot_shortcutSystray( QString device, QString shortcut )
{
    if ( device == "Start" ){
        startAction->setShortcutVisibleInContextMenu( true );
        startAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            startAction->setShortcutVisibleInContextMenu( false );
        }
        return;
    }

    if ( device == "Stop" ){
        stopAction->setShortcutVisibleInContextMenu( true );
        stopAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            stopAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "Pause" ){
        pauseAction->setShortcutVisibleInContextMenu( true );
        pauseAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            pauseAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "Continue" ){
        continueAction->setShortcutVisibleInContextMenu( true );
        continueAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            continueAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "camera" ){
        cameraAction->setShortcutVisibleInContextMenu( true );
        cameraAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            cameraAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "magnification" ){
        magnifierAction->setShortcutVisibleInContextMenu( true );
        magnifierAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            magnifierAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "showclick" ){
        showclickAction->setShortcutVisibleInContextMenu( true );
        showclickAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            showclickAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "halo" ){
        haloAction->setShortcutVisibleInContextMenu( true );
        haloAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            haloAction->setShortcutVisibleInContextMenu( false );
        }
    }

    if ( device == "snapshot" ){
        snapshotAction->setShortcutVisibleInContextMenu( true );
        snapshotAction->setShortcut( QKeySequence::fromString( shortcut ) );
        if ( shortcut == "None" ){
            snapshotAction->setShortcutVisibleInContextMenu( false );
        }
    }

}
