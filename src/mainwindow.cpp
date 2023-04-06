/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2022 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "mainwindow.h"

#include "ui_formMainWindow.h"
#include "QvkInformation.h"
#include "global.h"
#include "QvkScreenManager.h"
#include "QvkLicenses.h"
#include "QvkImageFromTabs.h"
#include "QvkDirDialog.h"
#include "QvkVirtual.h"
#include "QvkSnapshot.h"
#include "QvkPadsAndCaps.h"

#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <QStringList>
#include <QLibraryInfo>
#include <QThread>
#include <QMessageBox>
#include <QStyleFactory>
#include <QAbstractItemView>

#ifdef Q_OS_LINUX
  #include <pulse/pulseaudio.h>
  #include "QvkComposite.h"
#endif

QvkMainWindow::QvkMainWindow(QWidget *parent) : QMainWindow(parent),
                                                ui(new Ui::formMainWindow),
                                                vkWinInfo(new QvkWinInfo)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    // Only for Windows WASAPI
    soundEffect = new QSoundEffect();
#endif

#ifdef Q_OS_LINUX
    // Composite
    QvkComposite *vkComposite = new QvkComposite( this );
    Q_UNUSED(vkComposite)
#endif

    QFile fileCSS( ":/pictures/css/css.qss" );
    fileCSS.open( QFile::ReadOnly | QFile::Text );
    QTextStream streamCSS( &fileCSS );
    qApp->setStyleSheet( streamCSS.readAll() );
    fileCSS.close();

    sliderScreencastCountDown = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_60->insertWidget( 1, sliderScreencastCountDown );
    sliderScreencastCountDown->setObjectName( "sliderScreencastCountDown" );
    sliderScreencastCountDown->setTracking( true );
    sliderScreencastCountDown->setMinimum( 0 );
    sliderScreencastCountDown->setMaximum( 30 );
    sliderScreencastCountDown->setValue( 0 );
    sliderScreencastCountDown->setPageStep( 1 );
    sliderScreencastCountDown->show();

    sliderFrames = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_33->insertWidget( 0, sliderFrames );
    sliderFrames->setObjectName( "sliderFrames" );
    sliderFrames->setTracking( true );
    sliderFrames->setMinimum( 10 );
    sliderFrames->setMaximum( 144 );
    sliderFrames->setValue( 25 );
    sliderFrames->show();

    sliderX264 = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_26->insertWidget( 2, sliderX264 );
    sliderX264->setObjectName( "sliderX264" );
    sliderX264->setTracking( true );
    sliderX264->setMinimum( 0 );
    sliderX264->setMaximum( 50 );
    sliderX264->setValue( 17 );
    sliderX264->show();

    sliderOpenh264 = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_openh264->insertWidget( 1, sliderOpenh264 );
    sliderOpenh264->setObjectName( "sliderOpenh264" );
    sliderOpenh264->setTracking( true );
    sliderOpenh264->setMinimum( 1 ); // we need minimum 1, with 0 we get wrong colors.
    sliderOpenh264->setMaximum( 51 );
    sliderOpenh264->setValue( 23 );
    sliderOpenh264->show();

    sliderVp8 = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_vp8->insertWidget( 1, sliderVp8 );
    sliderVp8->setObjectName( "sliderVp8" );
    sliderVp8->setTracking( true );
    sliderVp8->setMinimum( 0 );
    sliderVp8->setMaximum( 63 );
    sliderVp8->setValue( 20 );
    sliderVp8->show();

    sliderLimitOfFreeDiskSpace = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_27->insertWidget( 3, sliderLimitOfFreeDiskSpace );
    sliderLimitOfFreeDiskSpace->setObjectName( "sliderLimitOfFreeDiskSpace" );
    sliderLimitOfFreeDiskSpace->setMinimum( 100 );
    sliderLimitOfFreeDiskSpace->setMaximum( 999 );
    sliderLimitOfFreeDiskSpace->setValue( 250 );
    sliderLimitOfFreeDiskSpace->show();

    sliderShowInSystrayAlternative = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_18->insertWidget( 1, sliderShowInSystrayAlternative );
    sliderShowInSystrayAlternative->setObjectName( "sliderShowInSystrayAlternative" );
    sliderShowInSystrayAlternative->setMinimum( 24 );
    sliderShowInSystrayAlternative->setMaximum( 64 );
    sliderShowInSystrayAlternative->setValue( 48 );
    sliderShowInSystrayAlternative->show();

    sliderHour = new QvkSpezialSlider( Qt::Horizontal );
    ui->verticalLayout_14->addWidget( sliderHour );
    sliderHour->setObjectName( "sliderHour" );
    sliderHour->setMinimum( 0 );
    sliderHour->setMaximum( 23 );
    sliderHour->setValue( 0 );
    sliderHour->show();

    sliderMinute = new QvkSpezialSlider( Qt::Horizontal );
    ui->verticalLayout_14->addWidget( sliderMinute );
    sliderMinute->setObjectName( "sliderMinute" );
    sliderMinute->setMinimum( 0 );
    sliderMinute->setMaximum( 59 );
    sliderMinute->setValue( 0 );
    sliderMinute->show();

    sliderSecondWaitBeforeRecording = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_7->insertWidget( 1, sliderSecondWaitBeforeRecording );
    sliderSecondWaitBeforeRecording->setObjectName( "sliderSecondWaitBeforeRecording" );
    sliderSecondWaitBeforeRecording->setMinimum( 0 );
    sliderSecondWaitBeforeRecording->setMaximum( 3 );
    sliderSecondWaitBeforeRecording->setValue( 1 );
    sliderSecondWaitBeforeRecording->setPageStep( 1 );
    sliderSecondWaitBeforeRecording->show();

    sliderStopRecordingAfterHouers = new QvkSpezialSlider( Qt::Horizontal );
    ui->verticalLayout_29->addWidget( sliderStopRecordingAfterHouers );
    sliderStopRecordingAfterHouers->setObjectName( "sliderStopRecordingAfterHouers" );
    sliderStopRecordingAfterHouers->setMinimum( 0 );
    sliderStopRecordingAfterHouers->setMaximum( 99 );
    sliderStopRecordingAfterHouers->setValue( 0 );
    sliderStopRecordingAfterHouers->show();

    sliderStopRecordingAfterMinutes = new QvkSpezialSlider( Qt::Horizontal );
    ui->verticalLayout_29->addWidget( sliderStopRecordingAfterMinutes );
    sliderStopRecordingAfterMinutes->setObjectName( "sliderStopRecordingAfterMinutes" );
    sliderStopRecordingAfterMinutes->setMinimum( 0 );
    sliderStopRecordingAfterMinutes->setMaximum( 99 );
    sliderStopRecordingAfterMinutes->setValue( 0 );
    sliderStopRecordingAfterMinutes->show();

    sliderStopRecordingAfterSeconds = new QvkSpezialSlider( Qt::Horizontal );
    ui->verticalLayout_29->addWidget( sliderStopRecordingAfterSeconds );
    sliderStopRecordingAfterSeconds->setObjectName( "sliderStopRecordingAfterSeconds" );
    sliderStopRecordingAfterSeconds->setMinimum( 0 );
    sliderStopRecordingAfterSeconds->setMaximum( 99 );
    sliderStopRecordingAfterSeconds->setValue( 15 );
    sliderStopRecordingAfterSeconds->show();

    sliderWaitBeforeSnapshot = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_59->insertWidget( 1, sliderWaitBeforeSnapshot );
    sliderWaitBeforeSnapshot->setObjectName( "sliderWaitBeforeSnapshot" );
    sliderWaitBeforeSnapshot->setMinimum( 0 );
    sliderWaitBeforeSnapshot->setMaximum( 30 );
    sliderWaitBeforeSnapshot->setValue( 4 );
    sliderWaitBeforeSnapshot->setDecimalPoint( true );
    sliderWaitBeforeSnapshot->show();

    sliderGstDebugLevel = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_36->insertWidget( 1, sliderGstDebugLevel );
    sliderGstDebugLevel->setObjectName( "sliderGstDebugLevel" );
    sliderGstDebugLevel->setMinimum( 0 );
    sliderGstDebugLevel->setMaximum( 9 );
    sliderGstDebugLevel->setValue( 0 );
    sliderGstDebugLevel->setDecimalPoint( false );
    sliderGstDebugLevel->setEnabled( false );
    sliderGstDebugLevel->setPageStep( 1 );
    sliderGstDebugLevel->show();

    ui->comboBox_shortcut_start->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->comboBox_shortcut_pause->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->comboBox_shortcut_magnification->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    vklogController = new QvkLogController();
    connect( vklogController, SIGNAL( signal_newLogText( QString ) ), this, SLOT( slot_textToGuiLog( QString ) ) );

    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );

    vkMagnifierController = new QvkMagnifierController(ui);
    Q_UNUSED(vkMagnifierController);
    ui->label_magnifier_on_screen->setText( "" );

    vkPlayer = new QvkPlayer( this, ui );

    QvkHelp *vkHelp = new QvkHelp( ui );

    QvkLicenses *vkLicenses = new QvkLicenses( ui );

    vkRegionChoise = new QvkRegionChoise( ui );

    vkCountdown = new QvkCountdown();
    vkCountdown->init();

    QvkVirtual *vkVirtual = new QvkVirtual();

    QvkInformation *vkInformation = new QvkInformation( this, ui, sliderScreencastCountDown, sliderSecondWaitBeforeRecording );
    connect( this, SIGNAL( signal_newVideoFilename( QString ) ), vkInformation, SLOT( slot_newVideoFilename( QString ) ) );

    vkLimitDiskFreeSpace = new QvkLimitDiskFreeSpace( ui, sliderLimitOfFreeDiskSpace );

    qDebug().noquote() << global::nameOutput << "Version:" << global::version;
    qDebug().noquote() << global::nameOutput << "Locale:" << QLocale::system().name();
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug().noquote() << global::nameOutput << "Log from:" << dateTime.toString( "yyyy-MM-dd hh:mm:ss" );
    QLocale locale;
    qDebug().noquote() << global::nameOutput << "Country:" << QLocale::countryToString( locale.country() );
    qDebug().noquote() << global::nameOutput << "Qt:" << qVersion();
    qDebug().noquote() << global::nameOutput << gst_version_string();
#ifdef Q_OS_LINUX
    qDebug().noquote() << global::nameOutput << "PulseAudio library version:" << pa_get_library_version();
#endif
    qDebug().noquote() << global::nameOutput << "Virtual Maschine:" << vkVirtual->isVirtualMaschine();
    qDebug().noquote() << global::nameOutput << "Operating system:" << QSysInfo::prettyProductName();
    if ( QSysInfo::prettyProductName().contains( "Flatpak" ) ) {
        isFlatpak = true;
    }

    qDebug().noquote() << global::nameOutput << "KernelVersion:"  << QSysInfo::kernelVersion();
    qDebug().noquote() << global::nameOutput << "CPU Architecture:" << QSysInfo::currentCpuArchitecture();
    qDebug().noquote() << global::nameOutput << "Count CPU:" << QThread::idealThreadCount();
    qDebug().noquote() << global::nameOutput << global::name << "running as:" << QGuiApplication::platformName() << "client";
#ifdef Q_OS_LINUX
    qDebug().noquote() << global::nameOutput << global::name << "running on:" << qgetenv( "XDG_SESSION_TYPE" ).toLower();
#endif
    qDebug().noquote() << global::nameOutput << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    qDebug().noquote() << global::nameOutput << "Icon-Theme:" << QIcon::themeName();
    qDebug().noquote() << global::nameOutput << "Styles:" << QApplication::style()->metaObject()->className();
    qDebug().noquote() << global::nameOutput << "ExecutablePath:     " << QCoreApplication::applicationDirPath();
    qDebug().noquote() << global::nameOutput << "Qt-PluginsPath:     " << QLibraryInfo::location( QLibraryInfo::PluginsPath );
    qDebug().noquote() << global::nameOutput << "Qt-TranslationsPath:" << QLibraryInfo::location( QLibraryInfo::TranslationsPath );
    qDebug().noquote() << global::nameOutput << "Qt-LibraryPath:     " << QLibraryInfo::location( QLibraryInfo::LibrariesPath );
    qDebug().noquote() << global::nameOutput << "Settings:" << vkSettings.getFileName();
    qDebug().noquote() << global::nameOutput << "Log:" << vklogController->get_logPath();
    qDebug().noquote() << global::nameOutput << "Default Videopath:" << QStandardPaths::writableLocation( QStandardPaths::MoviesLocation );
    qDebug().noquote() << global::nameOutput << "User Videopath:" << vkSettings.getVideoPath();
    qDebug();
#ifdef Q_OS_WIN
    QByteArray GSTREAMER_1_0_ROOT_X86 = qgetenv( "GSTREAMER_1_0_ROOT_X86" );
    qDebug().noquote() << global::nameOutput << "GSTREAMER_1_0_ROOT_X86:"<< GSTREAMER_1_0_ROOT_X86;

    QByteArray GST_PLUGIN_PATH_1_0 = qgetenv( "GST_PLUGIN_PATH_1_0" );
    qDebug().noquote() << global::nameOutput << "GST_PLUGIN_PATH_1_0:"<< GST_PLUGIN_PATH_1_0;

    QByteArray PATH = qgetenv( "PATH" );
    qDebug().noquote() << global::nameOutput << "PATH:"<< PATH;

    QByteArray GST_REGISTRY_1_0 = qgetenv( "GST_REGISTRY_1_0" );
    qDebug().noquote() << global::nameOutput << "GST_REGISTRY_1_0:"<< GST_REGISTRY_1_0;
    qDebug();
#endif

    vk_setCornerWidget( ui->tabWidgetScreencast );

// Linux player deactivated to manny errrors
/*
#ifdef Q_OS_LINUX
    vkPlayer = new QvkPlayer( ui );
#endif
#ifdef Q_OS_WIN
    vkPlayer = new QvkPlayer( this, ui );
#endif
*/


    // Sidebar
    connect( ui->toolButtonScreencast, &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(0); } );
    connect( ui->toolButtonCamera,     &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(1); } );
    connect( ui->toolButtonShowclick,  &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(2); } );
    connect( ui->toolButtonSnapshot,   &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(3); } );
    connect( ui->toolButtonShortcut,   &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(4); } );
    connect( ui->toolButtonPlayer,     &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(5); } );
    connect( ui->toolButtonLog,        &QToolButton::clicked, this, [=]() { ui->tabWidgetSideBar->setCurrentIndex(6); } );
    ui->tabWidgetSideBar->tabBar()->hide();
    ui->toolButtonScreencast->click();

    // Bar for start, stop etc.
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonContinue,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastFullscreen, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( slot_comboBoxScreencastScreenCountdown( bool ) ) );
#ifdef Q_OS_LINUX
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastWindow,     SLOT( setEnabled( bool ) ) );
#endif
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastArea,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxScreencastScreen,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelScreencastCountdownText,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), sliderScreencastCountDown, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameAudio,            SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameFrames,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( slot_IfStartAudioCodecWidgetsSetEnabled() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoCodecx264,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoCodecOpenh264,   SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoCodecVp8,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoPath,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameLimitOfFreeDiskSpace, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameStartTime,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxMinimizedWhenRecordingStarts, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxShowInSystray, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStart() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxScreencastScreenArea, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setEnabled( bool ) ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastFullscreen, SLOT( setDisabled( bool ) ) );
#ifdef Q_OS_LINUX
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastWindow,     SLOT( setDisabled( bool ) ) );
#endif
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxScreencastScreen,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelScreencastCountdownText,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), sliderScreencastCountDown, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameAudio,            SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameFrames,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this, SLOT( slot_IfStopAudioCodecWidgetsSetDisabled() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoCodecx264,      SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoCodecOpenh264,  SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoCodecVp8,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoPath,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameLimitOfFreeDiskSpace, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameStartTime,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxStopRecordingAfter, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxMinimizedWhenRecordingStarts, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxShowInSystray, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStop() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_Stop() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxScreencastScreenArea, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, &QPushButton::clicked, this, [=]() { lastButtonPressed = "start"; } );

    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), this,                   SLOT( slot_Pause() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonPause,    SLOT( hide() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( show() ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonStop,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonPause, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( setDisabled( bool ) ) );

    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), this,                   SLOT( slot_Continue() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonContinue, SLOT( hide() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonPause,    SLOT( show() ) );
    connect( ui->pushButtonContinue, SIGNAL( clicked( bool ) ), ui->pushButtonStop,     SLOT( setDisabled( bool ) ) );
    ui->pushButtonContinue->hide();

    connect( ui->pushButtonPlay, SIGNAL( clicked( bool ) ), this, SLOT( slot_Play() ) );

    connect( ui->pushButtonScreencastOpenfolder, SIGNAL( clicked( bool ) ), this, SLOT( slot_Folder() ) );

    connect( ui->pushButtonScreencastSnapshot, &QPushButton::clicked, this, [=]() { lastButtonPressed = "snapshot"; } );

    connect( ui->pushButton_log_openfolder, SIGNAL( clicked( bool ) ), this, SLOT( slot_logFolder() ) );

    // Tab 1 Screen
#ifdef Q_OS_WIN
    ui->radioButtonScreencastWindow->setEnabled( false );
    ui->radioButtonScreencastWindow->setVisible( false );
    ui->help_screencast_window->setVisible( false );
#endif

    ui->radioButtonScreencastFullscreen->setText( tr("Fullscreen") ); // QT Creator sets an ampersand, translation now here
    ui->radioButtonScreencastWindow->setText( tr("Window") ); // QT Creator sets an ampersand, translation now here

    connect( ui->radioButtonScreencastFullscreen, SIGNAL( toggled( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastFullscreen, SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreenArea, SLOT( setDisabled( bool ) ) );

    connect( ui->radioButtonScreencastWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreenArea, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastWindow, SIGNAL( toggled( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );

    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), vkRegionChoise, SLOT( slot_init() ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), vkRegionChoise, SLOT( setVisible( bool ) ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setEnabled( bool ) ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreenArea, SLOT( setEnabled( bool ) ) );

    connect( ui->toolButtonScreencastAreaReset, SIGNAL( clicked( bool ) ), vkRegionChoise, SLOT( slot_areaReset() ) );

    connect( ui->comboBoxScreencastScreen, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slot_setMaxFPS( int ) ) );

    // Tab 2 Audio and Videocodec
#ifdef Q_OS_WIN
    vkAudioController = new QvkAudioController( ui );
    connect( vkAudioController->vkDirectSoundController, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), this, SLOT( slot_haveAudioDeviceSelected( bool ) ) );
    connect( vkAudioController->vkWASAPIController,      SIGNAL( signal_haveAudioDeviceSelected( bool ) ), this, SLOT( slot_haveAudioDeviceSelected( bool ) ) );
#endif

#ifdef Q_OS_LINUX
    vkAudioController = new QvkAudioController( ui );
    connect( vkAudioController, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), this, SLOT( slot_haveAudioDeviceSelected( bool ) ) );
    vkAudioController->init();
#endif

    connect( ui->toolButtonFramesReset, SIGNAL( clicked( bool ) ), this, SLOT( slot_framesReset() ) );

    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_VideoCodecs_in_Combox( QString ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_AudioCodecs_in_Combox( QString ) ) );

    connect( ui->comboBoxVideoCodec, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( slot_videoCodecChanged( QString ) ) );
    connect( ui->toolButtonx264Reset, SIGNAL( clicked( bool ) ), this, SLOT( slot_x264Reset() ) );
    connect( ui->toolButtonOpenh264Reset, SIGNAL( clicked( bool ) ), this, SLOT( slot_openh264Reset() ) );
    connect( ui->toolButtonVP8Reset, SIGNAL( clicked( bool ) ), this, SLOT( slot_vp8Reset() ) );

    // Tab 3 Time
    connect( ui->checkBoxStartTime, SIGNAL( toggled( bool ) ), this, SLOT( slot_StartTimer( bool ) ) );
    timerStartTimer = new QTimer();
    timerStartTimer->setTimerType( Qt::PreciseTimer );
    connect( timerStartTimer,       SIGNAL( timeout() ),           this,                SLOT( slot_startTime() ) );
    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ),     ui->pushButtonStart, SLOT( setHidden( bool ) ) );
    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ),     ui->labelScreencastCountdownText,  SLOT( setDisabled( bool ) ) );
    connect( ui->checkBoxStartTime, SIGNAL( clicked( bool ) ),     sliderScreencastCountDown,         SLOT( setDisabled( bool ) ) );

    connect( ui->checkBoxStopRecordingAfter, SIGNAL( toggled( bool ) ), ui->frameStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( timerStopRecordingAfter,        SIGNAL( timeout() ),       ui->pushButtonStop, SLOT( click() ) );

    // Tab 4 Misc
    videoFileSystemWatcher = new QFileSystemWatcher();
    connect( ui->toolButtonVideoPath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newVideoPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    connect( videoFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    connect( ui->checkBoxGstreamerDebugLevel, SIGNAL( toggled( bool) ), this, SLOT( slot_GstreamerDebugLevel( bool ) ) );
    connect( ui->pushButtonGstreamerDebugLevel, SIGNAL( clicked( bool ) ), this, SLOT( slot_GstreamerOpenFolder( bool ) ) );
    connect( ui->checkBoxGstreamerDebugLevel, SIGNAL( clicked( bool ) ), this, SLOT( slot_GstreamerDebugLevelDialog( bool ) ) );

    // ***************** showClick *****************************
    vkShowClick = new QvkShowClick();
    vkShowClick->init( ui );
    vk_setCornerWidget( ui->tabWidgetShowClick );

    // ***************** Halo **********************************
    vkHalo = new QvkHalo();
    vkHalo->init( ui );

    // ***************** snapshot ******************************
    QvkSnapshot *vkSnapshot = new QvkSnapshot( this, ui );
    vkSnapshot->init();
    vk_setCornerWidget( ui->tabWidgetSnapshot );

    // ***************** shortcut ******************************
    vkGlobalShortcut = new QvkGlobalShortcut( this, ui );
    Q_UNUSED(vkGlobalShortcut);
    vk_setCornerWidget( ui->tabWidgetShortcut );

    // *************** systrayAlternative **********************
    vkSystrayAlternative = new QvkSystrayAlternative( this, ui, sliderShowInSystrayAlternative );
    vkSystray = new QvkSystray( ui );
    if ( QSystemTrayIcon::isSystemTrayAvailable() == true )
    {
        vkSystray->init();
        connect( vkSystray,                 SIGNAL( signal_SystemtrayIsClose() ),                 this,      SLOT( close() ) );
        connect( ui->checkBoxShowInSystray, SIGNAL( clicked( bool ) ),                            vkSystray, SLOT( setVisible( bool ) ) );
        connect( vkGlobalShortcut,          SIGNAL( signal_shortcutSystray( QString, QString ) ), vkSystray, SLOT( slot_shortcutSystray( QString, QString ) ) );
        ui->frameShowInSystrayAlternative->hide();
        ui->toolButtonShowInSystrayAlternativeReset->hide();
    } else
    {
        connect( ui->checkBoxShowInSystrayAlternative, SIGNAL( clicked( bool ) ), vkSystrayAlternative, SLOT( setVisible( bool ) ) );
        connect( vkGlobalShortcut,          SIGNAL( signal_shortcutSystray( QString, QString ) ), vkSystrayAlternative, SLOT( slot_shortcutSystray( QString, QString ) ) );
        ui->frameShowInSystray->hide();
    }

    // Tab 5 Available muxer, encoder etc.
    QIcon iconAvailable( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
    QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    ui->labelAvalible->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

    QIcon iconNotAvailable( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
    ui->labelNotAvailable->setPixmap( iconNotAvailable.pixmap( size, QIcon::Normal, QIcon::On ));
    // End Tabs

    // Tab 6 About
    ui->labelSourcecodeUrl->setText( "<a href='https://github.com/vkohaupt/vokoscreenNG'>" + tr( "Sourcecode" ) + "</a>" );
    ui->labelWebSiteUrl->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + tr( "Homepage" ) + "</a>" );
    ui->labelLanguageUrl->setText( "<a href='https://app.transifex.com/vkohaupt/vokoscreen/'>" + tr( "Translations" ) + "</a>" );
    ui->labelDonateUrl->setText( "<a href='https://linuxecke.volkoh.de/vokoscreen/vokoscreen-donate.html'>" + tr( "Donate" ) + "</a>" );

    // Close GUI
    connect( this,      SIGNAL( signal_close() ),       ui->pushButtonContinue, SLOT( click() ) );
    connect( this,      SIGNAL( signal_close() ),       ui->pushButtonStop,     SLOT( click() ) );
    connect( this,      SIGNAL( signal_close( bool ) ), ui->checkBoxCameraOnOff,SLOT( setChecked( bool ) ) );
    connect( this,      SIGNAL( signal_close() ),       vkHelp,                 SLOT( slot_cleanUp() ) );
    connect( this,      SIGNAL( signal_close() ),       vkHelp,                 SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkLicenses,             SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkSystrayAlternative,   SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkPlayer,               SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkRegionChoise,         SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkSystray,              SLOT( slot_closeSystray() ) );

    VK_Supported_Formats_And_Codecs();
    VK_Check_is_Format_available();
    VK_set_available_Formats_in_Combox();

    slot_gst_formatVideoAudoicodec_available();
    VK_gst_Elements_available();

    QvkScreenManager *screenManager = new QvkScreenManager( this );
    // Fullscreen
    connect( screenManager, SIGNAL( signal_clear_widget() ),                          ui->comboBoxScreencastScreen, SLOT( clear() ) );
    connect( screenManager, SIGNAL( signal_screen_count_changed( QString, QString) ), this,                         SLOT( slot_screenCountChanged( QString, QString ) ) );
    // Area
    connect( screenManager, SIGNAL( signal_clear_widget() ),                          ui->comboBoxScreencastScreenArea, SLOT( clear() ) );
    connect( screenManager, SIGNAL( signal_screen_count_changed( QString, QString) ), this,                             SLOT( slot_screenCountChangedArea( QString, QString ) ) );
    connect( ui->comboBoxScreencastScreenArea, SIGNAL( currentIndexChanged( int) ),   vkRegionChoise, SLOT( slot_init() ) );
    screenManager->init();

    // *****************Begin Camera *********************************
    vkCameraController = new QvkCameraController(ui);
    Q_UNUSED(vkCameraController);
    vk_setCornerWidget( ui->tabWidgetCamera );
    // *****************End Camera ***********************************

    // *****************Begin Log *********************************
    vk_setCornerWidget( ui->tabWidgetLog );
    // *****************End Log ***********************************

#ifdef Q_OS_WIN
    vkCiscoOpenh264Controller = new QvkCiscoOpenh264Controller( vkSettings.getFileName(), ui );
    vkCiscoOpenh264Controller->showWaitDialog();
    vkCiscoOpenh264Controller->init();
#endif
#ifdef Q_OS_LINUX
    ui->line_cisco->hide();
    ui->frame_cisco->hide();
#endif

    vkSettings.readAll( ui, this );
    vkSettings.readAreaScreencast( vkRegionChoise );
    vkSettings.readCamera( vkCameraController );
    vkSettings.readSystrayAlternative( vkSystrayAlternative );
    vkSettings.readPlayerPathOpenFile( vkPlayer );
    vkSettings.readHaloColor( vkHalo );
    vkSettings.readShowclickColor( vkShowClick );

    vkGlobalShortcut->slot_checkbox_shortcut_start_clicked( true );
    vkGlobalShortcut->slot_checkbox_shortcut_pause_clicked( true );
    vkGlobalShortcut->slot_checkbox_shortcut_magnification_clicked( true );
    vkGlobalShortcut->slot_checkbox_shortcut_camera_clicked( true );
    vkGlobalShortcut->slot_checkbox_shortcut_showclick_clicked( true );
    vkGlobalShortcut->slot_checkbox_shortcut_halo_clicked( true );
    vkGlobalShortcut->slot_checkbox_shortcut_snapshot_clicked( true );

    // After reading the settings, we read the arguments and run
    QStringList arguments = QApplication::instance()->arguments();
    if ( arguments.contains( "gst_appsrc" ) == true ) {
        gst_appsrc = true;
        ui->pushButtonStart->setVisible( false );
        ui->pushButtonStop->setVisible( false );
        ui->horizontalLayout->insertWidget( 0, ui->pushButtonStopAppsrc );
        ui->horizontalLayout->insertWidget( 0, ui->pushButtonStartAppsrc );
    } else {
        ui->pushButtonStartAppsrc->setVisible( false );
        ui->pushButtonStopAppsrc->setVisible( false );
        if ( arguments.count() > 1  ) {
            qDebug().noquote() << global::nameOutput << "started from file:" << arguments.at(1);
            vkPlayer->setMediaFile( arguments.at(1) );
            vkPlayer->slot_play();
            ui->tabWidgetSideBar->setCurrentIndex( ui->tabWidgetSideBar->indexOf( ui->tabSidebarPlayer ) );
        }
    }

    QvkImageFromTabs *vkImageFromTabs = new QvkImageFromTabs( this );
    Q_UNUSED(vkImageFromTabs)

    is_videoFolderExists_and_haveWritePermission();

    openh264ProfileTest = false;
    if ( openh264ProfileTest == true ) {
        QvkPadsAndCaps *vkPadsAndCaps = new QvkPadsAndCaps;
        connect( vkPadsAndCaps, SIGNAL( signal_openh264encProfile( QStringList ) ), this, SLOT( slot_comboBoxOpenh264Profile( QStringList ) ) );
        vkPadsAndCaps->pad_profile();
    } else {
        ui->label_61->setVisible( false );
        ui->comboBox_openh264_profile->setVisible( false );
    }


    QString localeName = QLocale::system().name() + ".qm";
    QString localeCountry = QLocale::countryToString( locale.country() );
    QDir dir( ":/language", "*.qm" );
    QStringList list = dir.entryList();
    if ( list.contains( localeName ) == false )
    {
        QPalette palette;
        palette.setColor( QPalette::Window, Qt::white );
        palette.setColor( QPalette::WindowText, Qt::red );
        ui->label_translate->setAutoFillBackground( true );
        ui->label_translate->setPalette( palette );

        QString transifex = "<a href='https://app.transifex.com/vkohaupt/vokoscreen/'>Transifex</a>";
        ui->label_translate->setText( "<p align=center>For<b> " + localeCountry + "</b> is the language <b> " + localeName.replace( ".qm", "" ) + " </b> not 100% translated.<br>" \
                                      "Please help to translate your language on <b>" + transifex + "</b></p>");
    } else {
        ui->label_translate->hide();
        ui->verticalLayout_7->removeItem( ui->verticalSpacer );
    }
}


QvkMainWindow::~QvkMainWindow()
{
    delete ui;
}


void QvkMainWindow::slot_comboBoxOpenh264Profile( QStringList list )
{
    ui->comboBox_openh264_profile->clear();
    ui->comboBox_openh264_profile->addItems( list );
    ui->comboBox_openh264_profile->setCurrentText( "baseline" );
}


void QvkMainWindow::slot_GstreamerDebugLevelDialog( bool value )
{
    if ( value == true )
    {
        QString string;
        string += "1. Warning, please only activate if requested by the developer!<br>";
        string += "2. Warning, the storage device can fill up to 2GB/min.!<br>";
        string += "3. Warning, your machine can slow down extremely!";

        QPixmap pixmap( ":/pictures/status/warning.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        msgBox.setText( "<center><b>Warning</b></center><br>"  + string );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.setStandardButtons( QMessageBox::Ok | QMessageBox::Cancel );
        msgBox.setDefaultButton( QMessageBox::Cancel );
        int ret = msgBox.exec();

        if( ret == QMessageBox::Cancel )
        {
            ui->checkBoxGstreamerDebugLevel->click();
        }
    }
}


void QvkMainWindow::slot_GstreamerOpenFolder( bool value )
{
    Q_UNUSED(value);
    QvkSettings vkSettingsGstDebug;
    QFileInfo fileInfo( vkSettingsGstDebug.getFileName() );
    QString path = fileInfo.absolutePath();
    if ( QDesktopServices::openUrl( QUrl( "file:///" + path, QUrl::TolerantMode ) ) == false )
    {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        //: Display a MessageBox, if not found a filemanager.
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}


void QvkMainWindow::slot_GstreamerDebugLevel( bool value )
{
   if ( value == true ) {
       sliderGstDebugLevel->setEnabled( true );
   } else {
       sliderGstDebugLevel->setValue( 0 );
       sliderGstDebugLevel->setEnabled( false );
   }
}


bool QvkMainWindow::is_videoFolderExists_and_haveWritePermission()
{
    // Create Folder if not exists
    QDir dir( ui->lineEditVideoPath->text() );
    if ( !dir.exists() )
    {
        // check of QStandardPaths::MoviesLocation
        QDir dir( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
        if ( !dir.exists() )
        {
            bool myBool = dir.mkpath( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
            Q_UNUSED(myBool);
        }
        ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );
    }

    // Check write permission
    QString filename;
    filename = ui->lineEditVideoPath->text() + + "/vokoscreenNG-test-write.txt";
    bool value;
    QFileInfo fileInfo( filename );
    QFile file( filename );
    if ( file.open( QIODevice::ReadWrite ) )
    {
        QTextStream stream( &file );
        stream << "Test Test Test Test Test Test" << Qt::endl;
        file.close();
        qDebug().noquote() << global::nameOutput << "Video permission: Can write in" << fileInfo.absolutePath();
        qDebug().noquote();
        file.remove();
        value = true;
    } else {
        qDebug().noquote() << "Video permission: ERROR can not write in" << fileInfo.absolutePath();
        qDebug().noquote();
        QPixmap pixmap( ":/pictures/status/warning.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        QMessageBox *messageBox = new QMessageBox();
        QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowIcon( icon );
        messageBox->setWindowTitle( global::name + " " + global::version );
        messageBox->setIconPixmap( pixmap );
        messageBox->setTextFormat( Qt::RichText );
        messageBox->setText( ( "<b>No write access on video folder</b>" ) );
        messageBox->setInformativeText( "vokoscreenNG can not create a video on<br>" + \
                                        fileInfo.absolutePath() + "<br><br>" + \
                                        "<b>Possible Cause:</b><br>" +
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


void QvkMainWindow::slot_haveAudioDeviceSelected( bool bo )
{
    if ( bo == false ) {
        ui->labelInfoAudiocodec->setText( "------" );
    } else {
        ui->labelInfoAudiocodec->setText( ui->comboBoxAudioCodec->currentText() );
    }
}


void QvkMainWindow::slot_textToGuiLog( QString value )
{
    ui->textBrowserLog->append( value );
}


void QvkMainWindow::slot_setMaxFPS( int index )
{
    if ( index > -1 )
    {
        QList<QScreen *> screen = QGuiApplication::screens();
        sliderFrames->setMaximum( screen.at( index )->refreshRate() );
    }
}


void QvkMainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);

#ifdef Q_OS_WIN
    if ( vkCiscoOpenh264Controller->isShowCiscoFinishDialog == false )
    {
#endif
        vkSettings.saveAll( ui, this, false );
        vkSettings.saveAreaScreencast( vkRegionChoise->getXRecordArea() / vkRegionChoise->screen->devicePixelRatio(),
                                       vkRegionChoise->getYRecordArea() / vkRegionChoise->screen->devicePixelRatio(),
                                       vkRegionChoise->getWidth() / vkRegionChoise->screen->devicePixelRatio(),
                                       vkRegionChoise->getHeight() / vkRegionChoise->screen->devicePixelRatio() );
        vkSettings.saveCamera( vkCameraController->cameraWindow->geometry().x(), vkCameraController->cameraWindow->geometry().y() );
        vkSettings.saveSystrayAlternative( vkSystrayAlternative->vkSystrayAlternativeWindow->x(), vkSystrayAlternative->vkSystrayAlternativeWindow->y() );
        vkSettings.savePlayerPathOpenFile( vkPlayer->pathOpenFile );
        vkSettings.saveHaloColor( vkHalo->vkHaloPreviewWidget->getColor() );
        vkSettings.saveShowclickColor( vkShowClick->vkPreviewWidget->getColor() );

#ifdef Q_OS_WIN
    }
#endif

    if ( vkShowClick->vkSpezialCheckbox->isChecked() == true )
    {
        emit vkShowClick->vkSpezialCheckbox->signal_clicked( false );
    }

    emit signal_close();
    emit signal_close( false );
}


/*
 * CountDown
 */
void QvkMainWindow::slot_comboBoxScreencastScreenCountdown( bool )
{
#ifdef Q_OS_LINUX
    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        int index = ui->comboBoxScreencastScreen->currentIndex();
        QList<QScreen *> screen = QGuiApplication::screens();
        int left = static_cast<int>( screen.at( index )->geometry().left() * screen.at( index )->devicePixelRatio() );
        int top = static_cast<int>( screen.at( index )->geometry().top() * screen.at( index )->devicePixelRatio() );

        vkCountdown->x = left + screen.at( index )->geometry().width() / 2 - ( vkCountdown->Width / 2 );
        vkCountdown->y = top + screen.at( index )->geometry().height() / 2 - ( vkCountdown->Height / 2 );
    }

    if ( ui->radioButtonScreencastWindow->isChecked() == true )
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        vkCountdown->x = ( screen->geometry().width() / 2 ) - ( vkCountdown->Width / 2 );
        vkCountdown->y = ( screen->geometry().height() / 2 ) - ( vkCountdown->Height / 2 );
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        int index = ui->comboBoxScreencastScreenArea->currentIndex();
        QList<QScreen *> screen = QGuiApplication::screens();
        int left = static_cast<int>( screen.at( index )->geometry().left() * screen.at( index )->devicePixelRatio() );
        int top = static_cast<int>( screen.at( index )->geometry().top() * screen.at( index )->devicePixelRatio() );

        vkCountdown->x = left + screen.at( index )->geometry().width() / 2 - ( vkCountdown->Width / 2 );
        vkCountdown->y = top + screen.at( index )->geometry().height() / 2 - ( vkCountdown->Height / 2 );
    }
#endif

#ifdef Q_OS_WIN
    // Unter Windows muß der Index des Displays zur Anzege des Countdowns in unsorierter Reihenfolge bereitgestellt werden.
    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        int index = 0;
        QString nameDisplay = ui->comboBoxScreencastScreen->currentText().section( ":", 0, 0 ).trimmed();
        QList<QScreen *> screenList = QGuiApplication::screens(); // unsortiert
        for ( int i=0; i < screenList.count(); i++ ) {
            QString nameList = screenList.at(i)->name().remove( "." ).remove( "\\" ) ;
            if ( nameDisplay == nameList )
            {
                index = i;
            }
        }

        int left = static_cast<int>( screenList.at( index )->geometry().left() * screenList.at( index )->devicePixelRatio() );
        int top = static_cast<int>( screenList.at( index )->geometry().top() * screenList.at( index )->devicePixelRatio() );
        vkCountdown->x = left + screenList.at( index )->geometry().width() / 2 - ( vkCountdown->Width / 2 );
        vkCountdown->y = top + screenList.at( index )->geometry().height() / 2 - ( vkCountdown->Height / 2 );
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        int index = 0;
        QString nameDisplay = ui->comboBoxScreencastScreenArea->currentText().section( ":", 0, 0 ).trimmed();
        QList<QScreen *> screenList = QGuiApplication::screens(); // unsortiert
        for ( int i=0; i < screenList.count(); i++ ) {
            QString nameList = screenList.at(i)->name().remove( "." ).remove( "\\" ) ;
            if ( nameDisplay == nameList )
            {
                index = i;
            }
        }
        int left = static_cast<int>( screenList.at( index )->geometry().left() * screenList.at( index )->devicePixelRatio() );
        int top = static_cast<int>( screenList.at( index )->geometry().top() * screenList.at( index )->devicePixelRatio() );

        vkCountdown->x = left + screenList.at( index )->geometry().width() / 2 - ( vkCountdown->Width / 2 );
        vkCountdown->y = top + screenList.at( index )->geometry().height() / 2 - ( vkCountdown->Height / 2 );
    }
#endif
}



void QvkMainWindow::slot_framesReset()
{
    sliderFrames->setValue( 25 );
}


void QvkMainWindow::slot_x264Reset()
{
    ui->comboBoxx264Preset->setCurrentIndex( 1 );
    sliderX264->setValue( 17 );
    ui->comboBoxx264Profile->setCurrentIndex( 0 );
}


void QvkMainWindow::slot_openh264Reset()
{
    sliderOpenh264->setValue( 23 );
}


void QvkMainWindow::slot_vp8Reset()
{
    sliderVp8->setValue( 20 );
}


void QvkMainWindow::slot_videoCodecChanged( QString codec )
{
    ui->frameVideoCodecx264->setVisible( false );
    ui->frameVideoCodecOpenh264->setVisible( false );
    ui->frameVideoCodecVp8->setVisible( false );

    if ( codec == "x264"  )
    {
        ui->frameVideoCodecx264->setVisible( true );
    }

    if ( codec == "H.264" ) // openh264
    {
        ui->frameVideoCodecOpenh264->setVisible( true );
    }

    if ( codec == "VP8" )
    {
        ui->frameVideoCodecVp8->setVisible( true );
    }
}


void QvkMainWindow::slot_IfStartAudioCodecWidgetsSetEnabled()
{
    if ( isAudioDeviceSelected() == true )
    {
        ui->labelAudioCodec->setEnabled( false );
        ui->comboBoxAudioCodec->setEnabled( false );
    }
}


void QvkMainWindow::slot_IfStopAudioCodecWidgetsSetDisabled()
{
    if ( isAudioDeviceSelected() == true )
    {
        ui->labelAudioCodec->setEnabled( true );
        ui->comboBoxAudioCodec->setEnabled( true );
    }
}


void QvkMainWindow::slot_areaSetResolution( QString value )
{
    vkRegionChoise->setWidth( value.section( " ", 0, 0 ).toInt() );
    vkRegionChoise->setHeight( value.section( " ", 2, 2 ).toInt() );
}


void QvkMainWindow::slot_StartTimer( bool value )
{
    if ( value == true )
    {
        timerStartTimer->start( 1000 );
    }
    else
    {
        timerStartTimer->stop();
    }
}


void QvkMainWindow::slot_startTime()
{
    QTime time;
    if ( ( time.currentTime().hour() == sliderHour->value() ) and
         ( time.currentTime().minute() == sliderMinute->value() ) and
         ( time.currentTime().second() == 0 ) )
    {
        ui->pushButtonStart->setVisible( true );
        ui->pushButtonStart->click();
    }
}


void QvkMainWindow::slot_newVideoPath()
{
    QApplication::setDesktopSettingsAware( false );

    QvkDirDialog vkDirDialog( this );
    if ( vkDirDialog.exec() == QDialog::Accepted )
    {
        if ( !vkDirDialog.selectedFiles().empty() ) {
            ui->lineEditVideoPath->setText( vkDirDialog.selectedFiles().at(0) );
        }
    }

    QApplication::setDesktopSettingsAware( true );
}


void QvkMainWindow::slot_videoFileSystemWatcherSetNewPath()
{
    if ( !videoFileSystemWatcher->directories().isEmpty() )
    {
       videoFileSystemWatcher->removePaths( videoFileSystemWatcher->directories() );
    }
    videoFileSystemWatcher->addPath( ui->lineEditVideoPath->text() );
}


void QvkMainWindow::slot_videoFileSystemWatcherSetButtons()
{
  QDir dir( ui->lineEditVideoPath->text() );
  QStringList filters;
  filters << global::name +"*";
  QStringList List = dir.entryList( filters, QDir::Files, QDir::Time );

  if ( List.isEmpty() || ( ui->pushButtonStart->isEnabled() == false ) )
  {
    ui->pushButtonPlay->setEnabled( false );
  }
  else
  {
    ui->pushButtonPlay->setEnabled( true );
  }
}


void QvkMainWindow::resizeEvent( QResizeEvent *event )
{
    emit signal_resizeEvent( event );
}


void QvkMainWindow::showEvent( QShowEvent *event )
{
    Q_UNUSED(event)
#ifdef Q_OS_WIN
    if ( vkAudioController->radioButtonWASAPI->isChecked() ) {
        if ( VK_getSelectedAudioDevice().count() > 1 ) {
            if ( global::testWASAPI == false ) {
                QMetaObject::invokeMethod( this, "slot_afterWindowShown", Qt::ConnectionType::QueuedConnection );
            }
        }
    }
#endif
}


// here we can under Windows for WASAPI call a first record
void QvkMainWindow::slot_afterWindowShown() {
#ifdef Q_OS_WIN
    update();
    repaint();
    if ( vkAudioController->radioButtonWASAPI->isChecked() ) {
        if ( VK_getSelectedAudioDevice().count() > 1 ) {
            if ( global::testWASAPI == false ) {
                ui->pushButtonStart->click();
                QTimer::singleShot( 2000, Qt::PreciseTimer, ui->pushButtonStop, SLOT( click() ) );
            }
        }
    }
#endif
}


void QvkMainWindow::vk_setCornerWidget( QTabWidget *tabWidget )
{
#ifdef Q_OS_LINUX
    QPixmap pixmap( ":/pictures/cornerWidget/linux.png" );
#endif
#ifdef Q_OS_WIN
    QPixmap pixmap( ":/pictures/cornerWidget/windows.png" );
#endif
    pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    QLabel *label = new QLabel();
    label->setPixmap( pixmap );
    label->setEnabled( false );
    tabWidget->setCornerWidget( label, Qt::TopRightCorner);
}


bool QvkMainWindow::isAudioDeviceSelected()
{
    bool value = false;
    QList<QCheckBox *> listCheckBox = ui->scrollAreaAudioDevice->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( listCheckBox.at(i)->checkState() == Qt::Checked )
        {
            value = true;
            break;
        }
    }
    return value;
}


#ifdef Q_OS_LINUX
QString QvkMainWindow::VK_getXimagesrc()
{
    QString value = "";

    QString showPointer = "true";
    if( ui->checkBoxMouseCursorOnOff->checkState() == Qt::Checked )
    {
        showPointer = "false";
    }

    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
          QStringList stringList;
          stringList << "ximagesrc"
                     << "display-name=" + qgetenv( "DISPLAY" )
                     << "use-damage=false"
                     << "show-pointer=" + showPointer
                     << "startx=" + get_x_From_Screen()
                     << "starty=" + get_y_From_Screen()
                     << "endx="   + QString::number( get_x_From_Screen().toInt() + get_width_From_Screen().toInt()-1 )
                     << "endy="   + QString::number( get_y_From_Screen().toInt() + get_height_From_Screen().toInt()-1 );
          value = stringList.join( " " );
    }

    if( ui->radioButtonScreencastWindow->isChecked() == true )
    {
        QString xid;
        if ( ui->checkBoxStartTime->isChecked() == true )
        {
            xid = QString::number( QvkWinInfo::activeWindow() );
        }
        else
        {
            xid = QString::number( vkWinInfo->getWinID() );
        }

        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer
                   << "xid=" + xid;
        value = stringList.join( " " );
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer
                   << "startx=" + QString::number( vkRegionChoise->x() + vkRegionChoise->getXRecordArea() )
                   << "starty=" + QString::number( vkRegionChoise->y() + vkRegionChoise->getYRecordArea() )
                   << "endx="   + QString::number( vkRegionChoise->x() + vkRegionChoise->getXRecordArea() + vkRegionChoise->getWidth() - 1 )
                   << "endy="   + QString::number( vkRegionChoise->y() + vkRegionChoise->getYRecordArea() + vkRegionChoise->getHeight() - 1 );
        value = stringList.join( " " );
    }

    return value;
}
#endif


#ifdef Q_OS_WIN
QString QvkMainWindow::VK_getXimagesrc()
{
    QString value;
    QString showPointer = "true";

    if( ui->checkBoxMouseCursorOnOff->checkState() == Qt::Checked )
    {
        showPointer = "false";
    }

    if( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        int screenNumber = ( ui->comboBoxScreencastScreen->currentIndex() );
        QStringList stringList;
        stringList << "gdiscreencapsrc"
                   << "monitor=" + QString::number( screenNumber )
                   << "cursor=" + showPointer;
        value = stringList.join( " " );
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        int screenNumber = ( ui->comboBoxScreencastScreenArea->currentIndex() );
        QStringList stringList;
        stringList << "gdiscreencapsrc"
                   << "monitor=" + QString::number( screenNumber )
                   << "cursor=" + showPointer;
        value = stringList.join( " " );
    }

    return value;
}
#endif


QString QvkMainWindow::VK_getCapsFilter()
{
   QStringList stringList;
   stringList << "video/x-raw, framerate="
              << QString::number( sliderFrames->value() )
              << "/1";
   return QString( stringList.join( "" ) );
}

/*
 * For my better understanding is this comment in german.
 *
 * Einige encoder, wie z.b x264, benötigen zum encodieren gerade Werte.
 * Laut Dokumentation wird videoscale nur angewandt wenn sich die Quellauflösung zur Zielauflösung unterscheidet.
 * Wird ein Bereich oder ein Fenster aufgenommen das ungerade Werte enthält wird diese mittels videoscale korrigiert.
 * Für das Aufnehmen in einer Virtuellen Maschine die in einem Fenster läuft, wird die Vollbildaufnahme ebenfalls korrigiert.
 */
QString QvkMainWindow::VK_scale()
{
    QString value = "";
    int modulo = 2;

    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        int width = get_width_From_Screen().toInt();
        int height = get_height_From_Screen().toInt();

        if ( ( get_width_From_Screen().toInt() % modulo ) > 0 )
        {
            width = get_width_From_Screen().toInt() - ( get_width_From_Screen().toInt() % modulo ) + modulo;
        }

        if ( ( get_height_From_Screen().toInt() % modulo ) > 0 )
        {
            height = get_height_From_Screen().toInt() - ( get_height_From_Screen().toInt() % modulo ) + modulo;
        }

        value = "videoscale ! video/x-raw, width=" + QString::number( width ) + ", height=" + QString::number( height ) + " !";
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        int width = (int)vkRegionChoise->getWidth();
        int height = (int)vkRegionChoise->getHeight();

        if ( ( (int)vkRegionChoise->getWidth() % modulo ) > 0 )
        {
            width = (int)vkRegionChoise->getWidth() - ( (int)vkRegionChoise->getWidth() % modulo ) + modulo;
        }

        if ( ( (int)vkRegionChoise->getHeight() % modulo ) > 0 )
        {
            height = (int)vkRegionChoise->getHeight() - ( (int)vkRegionChoise->getHeight() % modulo ) + modulo;
        }

        value = "videoscale ! video/x-raw, width=" + QString::number( width ) + ", height=" + QString::number( height )  + " !";
    }

#ifdef Q_OS_LINUX
    if ( ui->radioButtonScreencastWindow->isChecked() == true )
    {
        int modulo = 2;
        QRectF rect = vkWinInfo->windowGeometryWithoutFrame( vkWinInfo->getWinID() );

        int width = (int)rect.width();
        int height = (int)rect.height();

        if ( ( width % modulo ) > 0 )
        {
            width = width - ( width % modulo );
        }

        if ( ( height % modulo ) > 0 )
        {
            height = height - ( height % modulo );
        }

        value = "videoscale ! video/x-raw, width=" + QString::number( width ) + ", height=" + QString::number( height )  + " !";
    }
#endif

    return value;
}


void QvkMainWindow::VK_gst_Elements_available()
{
    QStringList list;
#ifdef Q_OS_WIN
    list << "gdiscreencapsrc";
    list << "wasapisrc";
    list << "directsoundsrc";
    list << "bz2dec";
    list << "videocrop";
#endif
#ifdef Q_OS_LINUX
    list << "ximagesrc";
    list << "pulsesrc";
#endif
    list << "queue";
    list << "appsrc";
    list << "capsfilter";
    list << "videoconvert";
    list << "videorate";
    list << "videoscale";
    list << "h264parse";
    list << "audioconvert";
    list << "audiorate";
    list << "audiomixer";
    list << "audioresample";
    list << "filesink";

    for ( int i = 0; i < list.count(); i++ )
    {
        GstElementFactory *factory = gst_element_factory_find( QString( list.at(i) ).toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << list.at(i);
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << list.at(i);
            gst_object_unref( factory );
        }
    }
    qDebug();
}


// Check format, video and audoicodec on tab availability
void QvkMainWindow::slot_gst_formatVideoAudoicodec_available()
{
    // Delete all QLabel
    QList<QLabel *> listLabel = ui->scrollAreaWidgetContentsAvailable->findChildren<QLabel *>();
    for( int i = 0; i < listLabel.count(); i++ )
    {
        delete listLabel.at( i );
    }

    // Delete spacerItem
    for ( int i = 0; i < ui->gridLayoutAvailable->count(); ++i )
    {
        QLayoutItem *layoutItem = ui->gridLayoutAvailable->itemAt(i);
        if ( layoutItem->spacerItem() )
        {
            ui->gridLayoutAvailable->removeItem(layoutItem);
            delete layoutItem;
            --i;
        }
    }

    // Delete line
    QList<QFrame *> listFrame = ui->scrollAreaWidgetContentsAvailable->findChildren<QFrame *>();
    for( int i = 0; i < listFrame.count(); i++ )
    {
        delete listFrame.at( i );
    }

    // Adding all informations
    int rowCount = 0;
    for ( int i = 0; i < videoFormatsList.count(); i++ )
    {
        int rowMuxer = 1;
        int rowVideo = 1;
        int rowAudio = 1;
        QStringList listElements = QString( videoFormatsList.at(i) ).split( "," ); // listElement beinhaltet muxer, Video und Audio

        for ( int x = 0; x < listElements.count(); x++ )
        {
            bool available;
            QString element = QString( listElements.at( x ) ).section( ":", 1, 1 );
            GstElementFactory *factory = gst_element_factory_find( element.toLatin1() );
            if ( !factory )
            {
                available = false;
            }
            else
            {
                available = true;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "muxer" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                {
                    QIcon picture( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
                    icon = picture;
                }
                else
                {
                    QIcon picture( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
                    icon = picture;
                }
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowMuxer, 0 );

                ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowMuxer, 1 );
                rowMuxer++;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "videocodec" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;

                // If element available then check video codec
                if ( available == true )
                {
                    GstElement *source = gst_element_factory_create( factory, "source" );
                    if ( !source )
                    {
                        QIcon picture( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
                        icon = picture;
                    }
                    else
                    {
                        QIcon picture( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
                        icon = picture;
                        gst_object_unref( source );
                        gst_object_unref( factory );
                    }
                }
                else
                {
                    QIcon picture( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
                    icon = picture;
                }

                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowVideo, 2 );

                ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowVideo, 3 );
                rowVideo++;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "audiocodec" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                {
                    QIcon picture( QString::fromUtf8( ":/pictures/screencast/accept.png" ) );
                    icon = picture;
                }
                else
                {
                    QIcon picture( QString::fromUtf8( ":/pictures/screencast/missing.png" ) );
                    icon = picture;
                }

                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowAudio, 4 );
                ui->gridLayoutAvailable->addWidget( new QLabel( "  " + QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowAudio, 5 );
                rowAudio++;
            }
        }

        rowCount = ui->gridLayoutAvailable->rowCount();
        for ( int x = 0; x <= 5; x++ )
        {
            QFrame *line = new QFrame();
            line->setObjectName( QStringLiteral( "line" ) );
            line->setFrameShape( QFrame::HLine );
            line->setFrameShadow( QFrame::Sunken );
            ui->gridLayoutAvailable->addWidget( line, rowCount, x );

            QLabel *label = new QLabel;
            label->setText( " " );
            ui->gridLayoutAvailable->addWidget( label, rowCount, x );
        }
    }

    ui->gridLayoutAvailable->addItem( new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
                                      ui->gridLayoutAvailable->rowCount() + 1, 0 );
}


// This is the base for format, video and audiocodec
void QvkMainWindow::VK_Supported_Formats_And_Codecs()
{
    QString videocodec_x264enc = "";
    if ( isFlatpak == false ) {
        videocodec_x264enc = "videocodec:x264enc:x264";
    }

    QStringList MKV_QStringList = ( QStringList()
                                    << "muxer:matroskamux:mkv"
                                    << "videocodec:openh264enc:H.264"
                                #ifdef Q_OS_LINUX
                                    << videocodec_x264enc
                                #endif
                                    << "videocodec:vp8enc:VP8"
                                    << "audiocodec:vorbisenc:vorbis"
                                    << "audiocodec:flacenc:flac"
                                    << "audiocodec:opusenc:opus"
                                    << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList WEBM_QStringList = ( QStringList()
                                     << "muxer:webmmux:webm"
                                     << "videocodec:vp8enc:VP8"
                                     << "audiocodec:vorbisenc:vorbis"
                                     << "audiocodec:opusenc:opus"
                                   );

    QStringList AVI_QStringList = ( QStringList()
                                     << "muxer:avimux:avi"
                                     << "videocodec:openh264enc:H.264"
                                #ifdef Q_OS_LINUX
                                     << videocodec_x264enc
                                #endif
                                     << "videocodec:vp8enc:VP8"
                                     << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList MP4_QStringList = ( QStringList()
                                    << "muxer:mp4mux:mp4"
                                    << "videocodec:openh264enc:H.264"
                                #ifdef Q_OS_LINUX
                                    << videocodec_x264enc
                                #endif
                                    << "audiocodec:lamemp3enc:mp3"
                                    << "audiocodec:opusenc:opus"
                                    );

    // https://de.wikipedia.org/wiki/QuickTime
    QStringList MOV_QStringList = ( QStringList()
                                    << "muxer:qtmux:mov"
                                    << "videocodec:openh264enc:H.264"
                                #ifdef Q_OS_LINUX
                                    << videocodec_x264enc
                                #endif
                                    << "videocodec:vp8enc:VP8"
                                    << "audiocodec:lamemp3enc:mp3"
                                  );

    videoFormatsList.clear();
    videoFormatsList.append( MKV_QStringList.join( ","  ) );
    videoFormatsList.append( WEBM_QStringList.join( ","  ) );
    videoFormatsList.append( AVI_QStringList.join( "," ) );
    videoFormatsList.append( MP4_QStringList.join( ",") );
    videoFormatsList.append( MOV_QStringList.join( ",") );
}


void QvkMainWindow::VK_Check_is_Format_available()
{
    qDebug().noquote() << global::nameOutput << "Record symbols: + available, - not available";
    QStringList tempList;
    for ( int x = 0; x < videoFormatsList.count(); x++ )
    {
        QString stringAllKeys = videoFormatsList.at( x );
        QStringList listKeys = stringAllKeys.split( "," );
        QStringList listKey = listKeys.filter( "muxer" );
        QString muxer = QString( listKey.at( 0 ) ).section( ":", 1, 1 );

        GstElementFactory *factory = gst_element_factory_find( muxer.toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << muxer;
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << muxer;
            tempList << videoFormatsList.at( x );
            gst_object_unref( factory );
        }
    }
    videoFormatsList.clear();
    videoFormatsList << tempList;
}


void QvkMainWindow::VK_set_available_Formats_in_Combox()
{
    ui->comboBoxFormat->clear();

    for ( int x = 0; x < videoFormatsList.count(); x++  )
    {
        QString stringAllKeys = videoFormatsList.at( x );
        QStringList listKeys = stringAllKeys.split( "," );
        QStringList listKeyMuxer = listKeys.filter( "muxer" );

        QIcon icon;
        if ( QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ) == "mkv" ){
            icon = QIcon( ":/pictures/screencast/strip-mkv.png" );
        }
        if ( QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ) == "avi" ){
            icon = QIcon( ":/pictures/screencast/strip-avi.png" );
        }
        if ( QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ) == "webm" ){
            icon = QIcon( ":/pictures/screencast/strip-webm.png" );
        }
        if ( QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ) == "mp4" ){
            icon = QIcon( ":/pictures/screencast/strip-mp4.png" );
        }
        if ( QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ) == "mov" ){
            icon = QIcon( ":/pictures/screencast/strip-mov.png" );
        }

        ui->comboBoxFormat->addItem( icon, // Picture
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 2, 2 ), // suffix
                                     QString( listKeyMuxer.at( 0 ) ).section( ":", 1, 1 ) ); // muxer
    }
}


void QvkMainWindow::slot_set_available_VideoCodecs_in_Combox( QString suffix )
{
    ui->comboBoxVideoCodec->clear();
    
    QStringList listSuffix = videoFormatsList.filter( suffix );
    QString stringSuffix = listSuffix.at( 0 );
    QStringList listKeys = stringSuffix.split( "," );
    QStringList listKeyVideoCodec = listKeys.filter( "videocodec" );
    for ( int i = 0; i < listKeyVideoCodec.count(); i++ )
    {
        QString encoder = QString( listKeyVideoCodec.at( i ) ).section( ":", 1, 1 );

#ifdef Q_OS_WIN
        if ( ui->radioButton_cisco_off->isChecked() and ( encoder == "openh264enc" ) )
        {
            continue;
        }
#endif

        QString name = QString( listKeyVideoCodec.at( i ) ).section( ":", 2, 2 );
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        }
        else
        {
            QString message = global::nameOutput + " + " + encoder;
            GstElement *source = gst_element_factory_create( factory, "source" );
            if ( !source )
            {
                message = global::nameOutput + " - " + encoder + " available but codec is missing";
            }
            else
            {
                ui->comboBoxVideoCodec->addItem( name, encoder );
                gst_object_unref( source );
            }
            
            qDebug().noquote() << message;
            gst_object_unref( factory );
        }
    }
    
    if ( ui->comboBoxVideoCodec->count() == 0 )
    {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox messageBox;
        messageBox.setIconPixmap( pixmap );
        messageBox.setText( "<b>No videocodec found</b>" );

        QString string;
        string += "<center>Please install package</center><br>";
        string += "gstreamer-plugins-base<br>";
        string += "gstreamer-plugins-good<br>";
        string += "gstreamer-plugins-bad<br>";
        string += "gstreamer-plugins-ugly<br>";
        string += "gstreamer-plugins-libav<br>";
        messageBox.setInformativeText( string );
        messageBox.exec();

        ui->pushButtonStart->setEnabled( false );
    }
    else
    {
        ui->pushButtonStart->setEnabled( true );
    }
}


void QvkMainWindow::slot_set_available_AudioCodecs_in_Combox( QString suffix )
{
    ui->comboBoxAudioCodec->clear();

    QStringList listSuffix = videoFormatsList.filter( suffix );
    QString stringSuffix = listSuffix.at( 0 );
    QStringList listKeys = stringSuffix.split( "," );
    QStringList listKeyAudioCodec = listKeys.filter( "audiocodec" );
    for ( int i = 0; i < listKeyAudioCodec.count(); i++ )
    {
        QString encoder = QString( listKeyAudioCodec.at( i ) ).section( ":", 1, 1 );
        QString name =    QString( listKeyAudioCodec.at( i ) ).section( ":", 2, 2 );
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << encoder;
            ui->comboBoxAudioCodec->addItem( name, encoder );
            gst_object_unref( factory );
        }
    }
    qDebug();
}


QString QvkMainWindow::Vk_get_Videocodec_Encoder()
{
    QString value;
    QString encoder = ui->comboBoxVideoCodec->currentData().toString();
    if ( encoder == "x264enc" )
    {
        QStringList list;
        list << VK_scale();
        list << ui->comboBoxVideoCodec->currentData().toString();
        list << "qp-min=" + QString::number( sliderX264->value() );
        list << "qp-max=" + QString::number( sliderX264->value() );
        list << "speed-preset=" + ui->comboBoxx264Preset->currentText();
        list << "threads=" + QString::number( QThread::idealThreadCount() );
        list.removeAll( "" );
        value = list.join( " " );
        value.append( " ! video/x-h264, profile=" + ui->comboBoxx264Profile->currentText() );
    }

    if ( encoder == "openh264enc" )
    {
        QStringList list;
        list << ui->comboBoxVideoCodec->currentData().toString();
        list << "qp-min=" + QString::number( sliderOpenh264->value() );
        list << "qp-max=" + QString::number( sliderOpenh264->value() );
        list << "usage-type=camera"; // We need camera not screen. With screen and a fast sequence of images the video jerks.
        list << "complexity=low";
        list << "multi-thread=" + QString::number( QThread::idealThreadCount() );
        list << "slice-mode=auto"; // Number of slices equal to number of threads
        value = list.join( " " );
        if ( openh264ProfileTest == true ) {
            value.append( " ! video/x-h264, profile=" ).append( "\"" ).append( ui->comboBox_openh264_profile->currentText().append( "\"" ) );
        }
        value.append( " ! h264parse" );
    }

    if ( encoder == "vp8enc" )
    {
        QStringList list;
        list << "vp8enc";
        list << "min_quantizer=" + QString::number( sliderVp8->value() );
        list << "max_quantizer=" + QString::number( sliderVp8->value() );
        list << "cpu-used=" + QString::number( QThread::idealThreadCount() );
        list << "deadline=1000000";
        list << "threads=" + QString::number( QThread::idealThreadCount() );
        value = list.join( " " );
    }

    return value;
}


void QvkMainWindow::slot_preStart()
{
    if ( is_videoFolderExists_and_haveWritePermission() == false )
    {
        ui->pushButtonStop->setEnabled( false );
        ui->pushButtonStart->setEnabled( true );
        ui->pushButtonPause->setEnabled( false );
        ui->pushButtonPlay->setEnabled( false );
        ui->checkBoxResetAtNextStart->setChecked( true );
        ui->checkBoxResetAtNextStart->setEnabled( false );
        return;
    }

    if ( vkLimitDiskFreeSpace->isStorageOKMessagBoxByStart() == false )
    {
        wantRecording = false;
        ui->pushButtonStop->click();
        return;
    }

    if ( ui->checkBoxStopRecordingAfter->isChecked() == true )
    {
        int value = sliderStopRecordingAfterHouers->value()*60*60*1000;
        value += sliderStopRecordingAfterMinutes->value()*60*1000;
        value += sliderStopRecordingAfterSeconds->value()*1000;
        value += sliderSecondWaitBeforeRecording->value()*1000;
        value += sliderScreencastCountDown->value()*1000;
        timerStopRecordingAfter->setTimerType( Qt::PreciseTimer );
        timerStopRecordingAfter->start( value );
    }

    if ( ( ui->radioButtonScreencastFullscreen->isChecked() == true ) and ( ui->checkBoxStartTime->isChecked() == true ) )
    {
        slot_Start();
        return;
    }

#ifdef Q_OS_WIN
    if ( ( ui->radioButtonScreencastFullscreen->isChecked() == true ) and
         ( sliderScreencastCountDown->value() > 0 ) and
         ( vkAudioController->vkWASAPIController->wantCountdown == true ) )
#endif
#ifdef Q_OS_LINUX
    if ( ( ui->radioButtonScreencastFullscreen->isChecked() == true ) and ( sliderScreencastCountDown->value() > 0 ) )
#endif
    {
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkCountdown->startCountdown( sliderScreencastCountDown->value() );
        connect( vkCountdown, SIGNAL( signal_countDownCancel( bool ) ), this, SLOT( slot_cancel( bool ) ) );
        return;
    }

    if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
    {
        slot_Start();
        return;
    }

    if ( ( ui->radioButtonScreencastWindow->isChecked() == true ) and ( ui->checkBoxStartTime->isChecked() == true ) )
    {
        slot_Start();
        return;
    }

    if ( ( ui->radioButtonScreencastWindow->isChecked() == true ) and ( sliderScreencastCountDown->value() > 0 ) )
    {
        disconnect( vkWinInfo, nullptr, nullptr, nullptr );
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        connect( vkWinInfo,   SIGNAL( signal_windowChanged( bool ) ),   this,                SLOT( slot_startCounter( bool ) ) );
        connect( vkWinInfo,   SIGNAL( signal_showCursor( bool ) ),      ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo,   SIGNAL( signal_showCursor( bool ) ),      ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkWinInfo->slot_start();
        connect( vkCountdown, SIGNAL( signal_countDownCancel( bool ) ), this, SLOT( slot_cancel( bool ) ) );
        return;
    }

    if ( ui->radioButtonScreencastWindow->isChecked() == true )
    {
        disconnect( vkWinInfo, nullptr, nullptr, nullptr );
        connect( vkWinInfo, SIGNAL( signal_showCursor( bool ) ),    ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_showCursor( bool ) ),    ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkWinInfo, SIGNAL( signal_windowChanged( bool ) ), this,                SLOT( slot_Start() ) );
        vkWinInfo->slot_start();
        return;
    }

    if ( ( ui->radioButtonScreencastArea->isChecked() == true ) and ( ui->checkBoxStartTime->isChecked() == true ) )
    {
        slot_Start();
        return;
    }

#ifdef Q_OS_WIN
    if ( ( ui->radioButtonScreencastArea->isChecked() == true ) and
         ( sliderScreencastCountDown->value() > 0 ) and
         ( vkAudioController->vkWASAPIController->wantCountdown == true ) )
#endif
#ifdef Q_OS_LINUX
    if ( ( ui->radioButtonScreencastArea->isChecked() == true ) and ( sliderScreencastCountDown->value() > 0 ) )
#endif
    {
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkRegionChoise->recordMode( true );
        vkCountdown->startCountdown( sliderScreencastCountDown->value() );
        connect( vkCountdown, SIGNAL( signal_countDownCancel( bool ) ), this, SLOT( slot_cancel( bool ) ) );
        return;
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
       vkRegionChoise->recordMode( true );
       vkRegionChoise->repaint();
       vkRegionChoise->update();
       slot_Start();
       return;
    }
}


void QvkMainWindow::slot_cancel( bool value )
{
    Q_UNUSED(value)
    cancel = true;
    disconnect( vkCountdown, nullptr, nullptr, nullptr );
    ui->pushButtonStop->setEnabled( true );
    ui->pushButtonStop->click();
}


void QvkMainWindow::slot_startCounter( bool value )
{
    Q_UNUSED(value);
    if ( sliderScreencastCountDown->value() > 0 )
    {
        vkCountdown->startCountdown( sliderScreencastCountDown->value() );
    }
}


#ifdef Q_OS_LINUX
QString QvkMainWindow::VK_get_AudioSystem()
{
    return "pulsesrc";
}
#endif


QString QvkMainWindow::VK_getMuxer()
{
    QString value = ui->comboBoxFormat->currentData().toString();
    if ( ui->comboBoxFormat->currentData().toString() == "matroskamux" )
    {
        value = ui->comboBoxFormat->currentData().toString() + " name=mux writing-app=" + global::name + "_" + QString( global::version ).replace( " ", "_" );
    }
    else
    {
        value = ui->comboBoxFormat->currentData().toString() + " name=mux";
    }
    return value;
}


QStringList QvkMainWindow::VK_getSelectedAudioDeviceName()
{
    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
        if ( listQCheckBox.at(i)->checkState() == Qt::Checked )
        {
            list << listQCheckBox.at(i)->text();
        }
    }
    return list;
}


QStringList QvkMainWindow::VK_getSelectedAudioDevice()
{
    QStringList list;
    QList<QCheckBox *> listQCheckBox = ui->scrollAreaWidgetContentsAudioDevices->findChildren<QCheckBox *>();
    for ( int i = 0; i < listQCheckBox.count(); i++ )
    {
        if ( listQCheckBox.at(i)->checkState() == Qt::Checked )
        {
            list << listQCheckBox.at(i)->accessibleName();
        }
    }
    return list;
}


QString QvkMainWindow::Pipeline_structured_output( QString pipeline )
{
    QString string;
    QString nl;
#ifdef Q_OS_LINUX
    nl = "\\";
    string = pipeline.prepend( "gst-launch-1.0 -e " + nl + "\n    " );
#endif
#ifdef Q_OS_WIN
    nl = "^";
    string = pipeline.prepend( "gst-launch-1.0.exe -e " + nl + "\n    " );
#endif
    string = pipeline.replace( "mux.", "mux. " + nl + "\n   " );
    string = pipeline.replace( "mix.", "mix. " + nl + "\n   " );
    string = pipeline.replace( "!", nl + "\n        !" );
    string.append( "\n" );
    return string;
}


void QvkMainWindow::slot_Start()
{
    if ( ui->checkBoxMinimizedWhenRecordingStarts->isChecked() == true  )
    {
        setWindowState( Qt::WindowMinimized );
    }

    QThread::msleep( static_cast<unsigned long>( sliderSecondWaitBeforeRecording->value()) * 1000 );

    QStringList VK_PipelineList;
    VK_PipelineList << VK_getXimagesrc();
    VK_PipelineList << VK_getCapsFilter();
#ifdef Q_OS_WIN
    if ( ui->radioButtonScreencastArea->isChecked() == true ) {
        QString top = QString::number( vkRegionChoise->getYRecordArea() );
        QString left = QString::number( vkRegionChoise->getXRecordArea() );
        int int_right = ui->comboBoxScreencastScreenArea->currentData().toString().section( " ", 2, 2 ).split( "=" ).at( 1 ).toInt() - vkRegionChoise->getWidth() - vkRegionChoise->getXRecordArea();
        QString right = QString::number( int_right );
        int int_bottom = ui->comboBoxScreencastScreenArea->currentData().toString().section( " ", 3, 3 ).split( "=" ).at( 1 ).toInt() - vkRegionChoise->getHeight() - vkRegionChoise->getYRecordArea();
        QString bottom = QString::number( int_bottom );
        VK_PipelineList << QString( "videocrop " ) + "top=" + top + " " + "left=" + left + " " + "right=" + right + " " + "bottom=" + bottom;
    }
#endif
    VK_PipelineList << "videoconvert";
    VK_PipelineList << "videorate";
    VK_PipelineList << "queue max-size-bytes=1073741824 max-size-time=10000000000 max-size-buffers=1000";
    VK_PipelineList << Vk_get_Videocodec_Encoder();

    // Only if one or more audiodevice is selected
    if ( ( VK_getSelectedAudioDevice().count() > 0 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        VK_PipelineList << "queue";
        VK_PipelineList << "mux.";
    }

    // Pipeline for one selected audiodevice
    if ( ( VK_getSelectedAudioDevice().count() == 1 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        #ifdef Q_OS_LINUX
            VK_PipelineList << VK_get_AudioSystem().append( " device=" ).append( VK_getSelectedAudioDevice().at(0) )
                                                   .append( " client-name=" ).append( global::nameOutput + "." + QString( VK_getSelectedAudioDeviceName().at(0) ).replace( " ", "-") );
            VK_PipelineList << "audio/x-raw, channels=2";
            VK_PipelineList << "audioconvert";
            VK_PipelineList << "audiorate";
            VK_PipelineList << "queue max-size-bytes=1000000 max-size-time=10000000000 max-size-buffers=1000";
            VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
            VK_PipelineList << "queue";
            VK_PipelineList << "mux.";
        #endif

        #ifdef Q_OS_WIN
            if ( vkAudioController->radioButtonWASAPI->isChecked() )
            {
                if ( VK_getSelectedAudioDevice().at(0).section( ":::", 1, 1 ) == "Playback" )
                {
                    soundEffect->setSource( QUrl::fromLocalFile( ":/sound/wasapi.wav" ) );
                    soundEffect->setLoopCount( QSoundEffect::Infinite );
                    soundEffect->setVolume( 0.0 );
                    soundEffect->play();
                    qDebug().noquote() << global::nameOutput << "[WASAPI] Soundeffect run";
                    VK_PipelineList << QString( "wasapisrc loopback=true low-latency=true role=multimedia device=" ).append( VK_getSelectedAudioDevice().at(0).section( ":::", 0, 0 ) );
                }
                else
                {
                    VK_PipelineList << QString( "wasapisrc low-latency=true role=multimedia device=" ).append( VK_getSelectedAudioDevice().at(0).section( ":::", 0, 0 ) );
                }
                VK_PipelineList << "audioconvert";
                VK_PipelineList << "audiorate";
                VK_PipelineList << "queue max-size-bytes=1000000 max-size-time=10000000000 max-size-buffers=1000";
                VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
                VK_PipelineList << "queue";
                VK_PipelineList << "mux.";
            }

            if ( vkAudioController->radioButtonDirectSound->isChecked() )
            {
                VK_PipelineList << QString( "directsoundsrc device-name=" ).append( "'" + VK_getSelectedAudioDevice().at(0) + "'" );
                VK_PipelineList << "audio/x-raw, channels=2";
                VK_PipelineList << "audioconvert";
                VK_PipelineList << "audiorate";
                VK_PipelineList << "queue max-size-bytes=1000000 max-size-time=10000000000 max-size-buffers=1000";
                VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
                VK_PipelineList << "queue";
                VK_PipelineList << "mux.";
            }
        #endif
    }

    // Pipeline for more as one audiodevice
    if ( ( VK_getSelectedAudioDevice().count() > 1 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        // Geräte sortieren so das Playback Geräte zuerst erscheinen
        #ifdef Q_OS_WIN
            QStringList listDevices;
            QStringList listSource;
            QStringList listPlayer;
            for ( int x = 0; x < VK_getSelectedAudioDevice().count(); x++ )
            {
                if ( VK_getSelectedAudioDevice().at(x).section( ":::", 1, 1 ) == "Playback" ) {
                    listPlayer << VK_getSelectedAudioDevice().at(x);
                } else {
                    listSource << VK_getSelectedAudioDevice().at(x);
                }
            }
            listDevices << listSource;
            listDevices << listPlayer;
        #endif

        for ( int x = 0; x < VK_getSelectedAudioDevice().count(); x++ )
        {
            #ifdef Q_OS_LINUX
                VK_PipelineList << VK_get_AudioSystem().append( " device=" ).append( VK_getSelectedAudioDevice().at(x) )
                                                       .append( " client-name=" ).append( global::nameOutput + "." + QString( VK_getSelectedAudioDeviceName().at(x) ).replace( " ", "-") );
                VK_PipelineList << "audioconvert";
                VK_PipelineList << "audioresample";
                VK_PipelineList << "queue";
                VK_PipelineList << "mix.";
            #endif

            #ifdef Q_OS_WIN
                if ( vkAudioController->radioButtonWASAPI->isChecked() == true )
                {
                    if ( soundEffect->isPlaying() == false ) {
                        soundEffect->setSource( QUrl::fromLocalFile( ":/sound/wasapi.wav" ) );
                        soundEffect->setLoopCount( QSoundEffect::Infinite );
                        soundEffect->setVolume( 0.0 );
                        soundEffect->play();
                        qDebug().noquote() << global::nameOutput << "[WASAPI] Soundeffect run";
                    }

                    if ( listDevices.at(x).section( ":::", 1, 1 ) == "Playback" ) {
                        VK_PipelineList << QString( "wasapisrc loopback=true low-latency=true role=multimedia device=" ).append( listDevices.at(x).section( ":::", 0, 0 ) );
                        VK_PipelineList << "audioconvert";
                        VK_PipelineList << "audioresample";
                        VK_PipelineList << "queue";
                        VK_PipelineList << "mix.";
                    } else {
                        VK_PipelineList << QString( "wasapisrc low-latency=true role=multimedia device=" ).append( listDevices.at(x).section( ":::", 0, 0 ) );
                        VK_PipelineList << "audioconvert";
                        VK_PipelineList << "audioresample";
                        VK_PipelineList << "queue";
                        VK_PipelineList << "mix.";
                    }
                }

                if ( vkAudioController->radioButtonDirectSound->isChecked() )
                {
                    VK_PipelineList << QString( "directsoundsrc device-name=" ).append( "'" + listDevices.at(x) + "'" );
                    VK_PipelineList << "audioconvert";
                    VK_PipelineList << "queue";
                    VK_PipelineList << "mix.";
                }
            #endif
        }
        VK_PipelineList << "audiomixer name=mix";
        VK_PipelineList << "audioconvert";
        VK_PipelineList << "audiorate";
        VK_PipelineList << "queue";
        VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
        VK_PipelineList << "queue";
        VK_PipelineList << "mux.";
    }

    VK_PipelineList << VK_getMuxer();
    VK_PipelineList.removeAll( "" );

    QString newVideoFilename;
#ifdef Q_OS_WIN
    if ( vkAudioController->radioButtonWASAPI->isChecked() == true ) {
        if ( VK_getSelectedAudioDevice().count() > 1 ) {
            if ( global::testWASAPI == false ) {
                newVideoFilename = global::name + "-" + "TEST_WASAPI" + "." + ui->comboBoxFormat->currentText();
                VK_PipelineList << "filesink location=\"" + wasapiTemporaryDir.path() + "/" + newVideoFilename + "\"";
            } else {
                newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
                VK_PipelineList << "filesink location=\"" + ui->lineEditVideoPath->text() + "/" + newVideoFilename + "\"";
            }
        } else {
            newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
            VK_PipelineList << "filesink location=\"" + ui->lineEditVideoPath->text() + "/" + newVideoFilename + "\"";
        }
    }

    if ( vkAudioController->radioButtonDirectSound->isChecked() == true ) {
        newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
        VK_PipelineList << "filesink location=\"" + ui->lineEditVideoPath->text() + "/" + newVideoFilename + "\"";
    }
#endif

#ifdef Q_OS_LINUX
    newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
    VK_PipelineList << "filesink location=\"" + ui->lineEditVideoPath->text() + "/" + newVideoFilename + "\"";
#endif

    // Write settings to log
    vkSettings.saveAll( ui, this, true );

    QString VK_Pipeline = VK_PipelineList.join( VK_Gstr_Pipe );
    VK_Pipeline = VK_Pipeline.replace( "mix. !", "mix." );
    VK_Pipeline = VK_Pipeline.replace( "mux. !", "mux." );

    qDebug();
    qDebug().noquote() << global::nameOutput << "Free disk space at the beginning of the recording:" << ui->labelFreeSize->text() << "MB";
    qDebug();
    qDebug().noquote() << global::nameOutput << "Start record with:" << VK_Pipeline;
    qDebug();
    qDebug().noquote() << Pipeline_structured_output( VK_Pipeline );

    QByteArray byteArray = VK_Pipeline.toUtf8();
    const gchar *line = byteArray.constData();
    GError *error = Q_NULLPTR;
    pipeline = gst_parse_launch( line, &error );

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
    if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
    if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
    if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_ASYNC"   << "Returncode =" << ret;   } // 2
    if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "Start was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3
    if ( ret == GST_STATE_CHANGE_FAILURE )
    {
        qDebug().noquote() << global::name << "Unable to set the pipeline to the playing state.";
        gst_object_unref( pipeline );
        return;
    }

    emit signal_newVideoFilename( newVideoFilename );
}




void QvkMainWindow::slot_preStop()
{
    if ( ui->checkBoxStartTime->isChecked() == true )
    {
         ui->checkBoxStartTime->click();
    }

    if ( timerStopRecordingAfter->isActive() )
    {
        timerStopRecordingAfter->stop();
        ui->frameStopRecordingAfter->setEnabled( true );
    }
}


void QvkMainWindow::slot_Stop()
{
    if ( vkLimitDiskFreeSpace->isStorageOK() == false )
    {
        ui->checkBoxShowInSystray->click();
        ui->checkBoxShowInSystray->click();
    }


    if ( cancel == true )
    {
        cancel = false;
        goto Cancel;
    }


    if ( wantRecording == true )
    {
        // wait for EOS
        bool a = gst_element_send_event( pipeline, gst_event_new_eos() );
        Q_UNUSED(a);

        GstClockTime timeout = 5 * GST_SECOND;
        GstMessage *msg = gst_bus_timed_pop_filtered( GST_ELEMENT_BUS (pipeline), timeout, GST_MESSAGE_EOS );
        Q_UNUSED(msg);

        GstStateChangeReturn ret ;
        Q_UNUSED(ret);
        ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
        Q_UNUSED(ret);
        ret = gst_element_set_state( pipeline, GST_STATE_READY );
        Q_UNUSED(ret);
        ret = gst_element_set_state( pipeline, GST_STATE_NULL );
        Q_UNUSED(ret);
        gst_object_unref( pipeline );
        qDebug().noquote() << global::nameOutput << "Stop record";
    }

Cancel:

#ifdef Q_OS_WIN
   if ( soundEffect->isPlaying() == true ) {
      soundEffect->stop();
      qDebug().noquote() << global::nameOutput << "[WASAPI] Soundeffect stop";
   }
   vkAudioController->vkWASAPIController->wantCountdown = true;
   if ( global::testWASAPI == false ) {
       global::testWASAPI = true;
       wasapiTemporaryDir.remove();
       ui->labelInfoRecordTime->setText( "00:00:00" );
       ui->labelVideoSize->setText( "0" );
   }
#endif

    wantRecording = true;

    qDebug().noquote() << global::nameOutput << "Free disk space at the end of the recording:" << ui->labelFreeSize->text() << "MB";

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        // We wait one second and then show the button inside the frame.
        QThread::msleep( 1000 );
        vkRegionChoise->recordMode( false );
        vkRegionChoise->repaint();
        vkRegionChoise->update();
    }

    if ( ui->checkBoxMinimizedWhenRecordingStarts->isChecked() == true  )
    {
        showNormal();
    }
}


void QvkMainWindow::slot_Pause()
{
    if ( ui->pushButtonStart->isEnabled() == false )
    {
        qDebug().noquote() << global::nameOutput << "Pause was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PAUSED );
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_ASYNC" << "Returncode =" << ret;   }   // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "Pause was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3


        /* wait until it's up and running or failed */
        if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
        {
          g_error ("Failed to go into PAUSED state");
        }

    }
}


void QvkMainWindow::slot_Continue()
{
    if ( ( ui->pushButtonStart->isEnabled() == false ) and ( ui->pushButtonContinue->isEnabled() == true ) )
    {
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
        if ( ret == GST_STATE_CHANGE_FAILURE )   { qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_FAILURE" << "Returncode =" << ret;   } // 0
        if ( ret == GST_STATE_CHANGE_SUCCESS )   { qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_SUCCESS" << "Returncode =" << ret;   } // 1
        if ( ret == GST_STATE_CHANGE_ASYNC )     { qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_ASYNC" << "Returncode =" << ret;   }   // 2
        if ( ret == GST_STATE_CHANGE_NO_PREROLL ){ qDebug().noquote() << global::nameOutput << "Continue was clicked" << "GST_STATE_CHANGE_NO_PREROLL" << "Returncode =" << ret; }// 3

        /* wait until it's up and running or failed */
        if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
        {
          g_error ("Failed to go into PLAYING state");
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "Continue was clicked";
        }
    }
}


void QvkMainWindow::slot_Play()
{
    QDir dir( ui->lineEditVideoPath->text() );
    QStringList filters;
    filters << global::name + "*";
    QStringList videoFileList = dir.entryList( filters, QDir::Files, QDir::Time );

    qDebug().noquote() << global::nameOutput << "play video with vokoplayer" << videoFileList.at(0);
    ui->toolButtonPlayer->click();
    QString string;
    string.append( ui->lineEditVideoPath->text() );
    string.append( "/" );
    string.append( videoFileList.at( 0 ) );
    vkPlayer->setMediaFile( string );
    vkPlayer->slot_play();
}


void QvkMainWindow::slot_Folder()
{
    QString path;
    if ( lastButtonPressed == "start" ) {
        path = ui->lineEditVideoPath->text();
    }

    if ( lastButtonPressed == "snapshot" ) {
        path = ui->lineEditSnapshotImagePath->text();
    }

    if ( QDesktopServices::openUrl( QUrl( "file:///" + path, QUrl::TolerantMode ) ) == false )
    {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        
        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}


void QvkMainWindow::slot_logFolder()
{
    QUrl url( vklogController->get_logPath() );
    QString path = url.adjusted( QUrl::RemoveFilename ).toString();

    if ( QDesktopServices::openUrl( QUrl( "file:///" + path, QUrl::TolerantMode ) ) == false )
    {
        QPixmap pixmap( ":/pictures/status/information.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIconPixmap( pixmap );
        msgBox.exec();
    }
}


QString QvkMainWindow::get_x_From_Screen()
{
    QString value = ui->comboBoxScreencastScreen->currentData().toString().section( " ", 0, 0 ).split( "=" ).at( 1 );
    return value;
}

QString QvkMainWindow::get_y_From_Screen()
{
    QString value = ui->comboBoxScreencastScreen->currentData().toString().section( " ", 1, 1 ).split( "=" ).at( 1 );
    return value;
}

QString QvkMainWindow::get_width_From_Screen()
{
    QString value = ui->comboBoxScreencastScreen->currentData().toString().section( " ", 2, 2 ).split( "=" ).at( 1 );
    return value;
}

QString QvkMainWindow::get_height_From_Screen()
{
    QString value = ui->comboBoxScreencastScreen->currentData().toString().section( " ", 3, 3 ).split( "=" ).at( 1 );
    return value;
}


void QvkMainWindow::slot_screenCountChanged( QString stringText, QString stringData )
{
    ui->comboBoxScreencastScreen->addItem( stringText, stringData );
    qDebug().noquote() << global::nameOutput << "ItemText in Combobox:" << stringText;
    qDebug().noquote() << global::nameOutput << "ItemData in Combobox:" << stringData;
    qDebug();

#ifdef Q_OS_WIN
    QStringList list;
    for ( int i=0; i < ui->comboBoxScreencastScreen->count(); i++ )
    {
        list.append( ui->comboBoxScreencastScreen->itemText( i ) );
    }
    list.sort();
    ui->comboBoxScreencastScreen->clear();
    ui->comboBoxScreencastScreen->addItems( list );
#endif
}


void QvkMainWindow::slot_screenCountChangedArea( QString stringText, QString stringData )
{
    ui->comboBoxScreencastScreenArea->addItem( stringText, stringData );

#ifdef Q_OS_WIN
    // Unter Windows muß zur Aufnahme mit GStreamer das Display in sortierter Reihenfolge bereitgestellt werden. Display1, Display2, Display3 etc.
    QStringList list;
    for ( int i=0; i < ui->comboBoxScreencastScreenArea->count(); i++ )
    {
        list.append( ui->comboBoxScreencastScreenArea->itemText( i ) + "|" + ui->comboBoxScreencastScreenArea->itemData( i ).toString() );
    }
    list.sort();
    ui->comboBoxScreencastScreenArea->clear();

    for ( int i=0; i < list.count(); i++ ) {
        ui->comboBoxScreencastScreenArea->addItem( list.at(i).section( "|", 0, 0 ), list.at(i).section( "|", 1, 1 ) );
    }
#endif
}
