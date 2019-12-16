/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
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
#include "QvkGlobalShortcut.h"
#include "QvkLogController.h"
#include "global.h"
#include "QvkScreenManager.h"
#include "QvkLicenses.h"
#include "QvkLoadExtension.h"

#include <QDebug>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMimeDatabase>
#include <QStringList>
#include <QLibraryInfo>
#include <QThread>
#include <QMessageBox>
#include <QStyleFactory>

#ifdef Q_OS_LINUX
  #include <QX11Info>
#endif

#ifdef Q_OS_LINUX
QvkMainWindow::QvkMainWindow(QWidget *parent) : QMainWindow(parent),
                                                ui(new Ui::formMainWindow),
                                                vkWinInfo(new QvkWinInfo),
                                                vkCountdown(new QvkCountdown),
                                                vkRegionChoise(new QvkRegionChoise)
#endif

#ifdef Q_OS_WIN
QvkMainWindow::QvkMainWindow(QWidget *parent) : QMainWindow(parent),
                                                ui(new Ui::formMainWindow),
                                                vkWinInfo(new QvkWinInfo),
                                                vkCountdown(new QvkCountdown),
                                                vkRegionChoise(new QvkRegionChoise)
#endif
{
    ui->setupUi(this);

    sliderScreencastCountDown = new QvkSpezialSlider( Qt::Horizontal );
    ui->gridLayout_9->addWidget( sliderScreencastCountDown, 2, 1 );
    sliderScreencastCountDown->setObjectName("sliderScreencastCountDown");
    sliderScreencastCountDown->setTracking( true );
    sliderScreencastCountDown->setMinimum( 0 );
    sliderScreencastCountDown->setMaximum( 30 );
    sliderScreencastCountDown->setValue( 0 );
    sliderScreencastCountDown->show();

    sliderFrames = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_5->addWidget( sliderFrames );
    sliderFrames->setObjectName("sliderFrames");
    sliderFrames->setTracking( true );
    sliderFrames->setMinimum( 10 );
    sliderFrames->setMaximum( 99 );
    sliderFrames->setValue( 25 );
    sliderFrames->show();

    sliderX264 = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_x264->insertWidget( 2, sliderX264 );
    sliderX264->setObjectName("sliderX264");
    sliderX264->setTracking( true );
    sliderX264->setMinimum( 0 );
    sliderX264->setMaximum( 50 );
    sliderX264->setValue( 17 );
    sliderX264->show();

    sliderOpenh264 = new QvkSpezialSlider( Qt::Horizontal );
    ui->horizontalLayout_openh264->insertWidget( 1, sliderOpenh264 );
    sliderOpenh264->setObjectName("sliderOpenh264");
    sliderOpenh264->setTracking( true );
    sliderOpenh264->setMinimum( 0 );
    sliderOpenh264->setMaximum( 50 );
    sliderOpenh264->setValue( 23 );
    sliderOpenh264->show();

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

    vkTheme = new QvkTheme( ui );
    Q_UNUSED(vkTheme);

    QvkLogController *vklogController = new QvkLogController( ui );
    Q_UNUSED(vklogController);

    vkRegionChoise->setFrameColor( Qt::darkGreen );

    setWindowTitle( global::name + " " + global::version );
    QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
    setWindowIcon( icon );

    QvkMagnifierController *vkMagnifierController = new QvkMagnifierController(ui);
    Q_UNUSED(vkMagnifierController);

    vkPlayer = new QvkPlayer( this, ui );

    QvkHelp *vkHelp = new QvkHelp( this, ui );

    QvkLicenses *vkLicenses = new QvkLicenses( ui );

    /* Wayland
     * If start with "./name -platform wayland" comes a Memory access error
     * On Wayland we have to time no access to GlobalShortcuts
     * We must disable this function for Wayland but not for X11 and Windows
     */
#ifdef Q_OS_LINUX
    if ( QX11Info::isPlatformX11() == true )
    {
        QvkGlobalShortcut *vkGlobalShortcut = new QvkGlobalShortcut( this, ui );
        Q_UNUSED(vkGlobalShortcut);
    }
#endif

#ifdef Q_OS_Win
    QvkGlobalShortcut *vkGlobalShortcut = new QvkGlobalShortcut( this, ui );
    Q_UNUSED(vkGlobalShortcut);
#endif

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
    qDebug().noquote() << global::nameOutput << "Operating system:" << QSysInfo::prettyProductName();
    qDebug().noquote() << global::nameOutput << "CPU Architecture:" << QSysInfo::currentCpuArchitecture();
    qDebug().noquote() << global::nameOutput << global::name << "running as:" << QGuiApplication::platformName() << "client";

#ifdef Q_OS_LINUX
    qDebug().noquote() << global::nameOutput << global::name << "running on:" << qgetenv( "XDG_SESSION_TYPE" ).toLower();
#endif
    qDebug().noquote() << global::nameOutput << "Desktop:" << qgetenv( "XDG_CURRENT_DESKTOP" );
    qDebug().noquote() << global::nameOutput << "Icon-Theme:" << QIcon::themeName();
    qDebug().noquote() << global::nameOutput << "Styles:" << QStyleFactory::keys();
    qDebug().noquote() << global::nameOutput << "Qt-PluginsPath:     " << QLibraryInfo::location( QLibraryInfo::PluginsPath );
    qDebug().noquote() << global::nameOutput << "Qt-TranslationsPath:" << QLibraryInfo::location( QLibraryInfo::TranslationsPath );
    qDebug().noquote() << global::nameOutput << "Qt-LibraryPath:     " << QLibraryInfo::location( QLibraryInfo::LibrariesPath );
    qDebug().noquote() << global::nameOutput << "Settings:" << vkSettings.getFileName();
    qDebug().noquote() << global::nameOutput << "Log:" << vklogController->get_logPath();
#ifdef Q_OS_LINUX
    qDebug().noquote() << global::nameOutput << "CompositingManager running:" << QX11Info::isCompositingManagerRunning();
#endif
    qDebug();

    resolutionStringList << "320 x 200 CGA 16 : 10"
                         << "320 x 240 QCGA 4 : 3"
                         << "640 x 480 VGA 4 : 3"
                         << "720 x 480 NTSC 3 : 2"
                         << "800 x 480 WVGA 5 : 3"
                         << "800 x 600 SVGA 4 : 3"
                         << "854 x 450 WVGA 16 : 9"
                         << "768 x 567 PAL 4 : 3"
                         << "1024 x 768 XVGA 4 : 3"
                         << "1152 x 768 N/A 3 : 2"
                         << "1280 x 720 HD-720 16 : 9"
                         << "1280 x 768 WXGA 5 : 3"
                         << "1280 x 800 WXGA 16 : 10"
                         << "1280 x 1024 SXGA 5 : 4"
                         << "1920 x 1080 HD1080 16 : 9";

    vk_setCornerWidget( ui->tabWidgetScreencast );

    // Bar for start, stop etc.
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonContinue,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastFullscreen, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastWindow,     SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastArea,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxScreencastScreen,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelScreencastCountdownText,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), sliderScreencastCountDown, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameAudio,            SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), sliderFrames,              SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this, SLOT( slot_IfStartAudioCodecWidgetsSetEnabled() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoCodecx264,   SLOT( setEnabled( bool ) )   );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoCodecVaapih264,   SLOT( setEnabled( bool ) )   );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameVideoPath,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameLimitOfFreeDiskSpace, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameStartTime,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameStopRecordingAfter, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->checkBoxMinimizedWhenRecordingStarts, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStart() ) );

    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStop,        SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonStart,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPause,       SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->pushButtonPlay,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastFullscreen, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastWindow,     SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->radioButtonScreencastArea,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxScreencastScreen,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelScreencastCountdownText,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), sliderScreencastCountDown, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameAudio,            SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFrames,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), sliderFrames,              SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelFormat,           SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxFormat,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->labelVideoCodec,       SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->comboBoxVideoCodec,    SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this, SLOT( slot_IfStopAudioCodecWidgetsSetDisabled() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoCodecx264,   SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoCodecVaapih264,   SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxMouseCursorOnOff,SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameVideoPath,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameLimitOfFreeDiskSpace, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->frameStartTime,        SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxStopRecordingAfter, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), ui->checkBoxMinimizedWhenRecordingStarts, SLOT( setDisabled( bool ) ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_preStop() ) );
    connect( ui->pushButtonStop, SIGNAL( clicked( bool ) ), this,                      SLOT( slot_Stop() ) );

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


    // Tab 1 Screen
#ifdef Q_OS_WIN
    ui->radioButtonScreencastWindow->setEnabled( false );
#endif

    ui->radioButtonScreencastFullscreen->setText( tr("Fullscreen") ); // QT Creator sets an ampersand, translation now here
    ui->radioButtonScreencastWindow->setText( tr("Window") ); // QT Creator sets an ampersand, translation now here

    connect( ui->radioButtonScreencastFullscreen, SIGNAL( toggled( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastFullscreen, SIGNAL( toggled( bool ) ), ui->comboBoxAreaSize, SLOT( setDisabled( bool ) ) );//**

    connect( ui->radioButtonScreencastWindow, SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastWindow, SIGNAL( toggled( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastWindow, SIGNAL( toggled( bool ) ), ui->comboBoxAreaSize, SLOT( setDisabled( bool ) ) );

    connect( this,                            SIGNAL( signal_close()  ), vkRegionChoise,   SLOT( close() ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), vkRegionChoise,   SLOT( setVisible( bool ) ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), ui->comboBoxScreencastScreen, SLOT( setDisabled( bool ) ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), ui->toolButtonScreencastAreaReset, SLOT( setEnabled( bool ) ) );
    connect( ui->radioButtonScreencastArea,   SIGNAL( toggled( bool ) ), ui->comboBoxAreaSize, SLOT( setEnabled( bool ) ) );

    connect( ui->toolButtonScreencastAreaReset, SIGNAL( clicked( bool ) ), vkRegionChoise, SLOT( slot_areaReset() ) );
    connect( ui->toolButtonScreencastAreaReset, SIGNAL( clicked( bool ) ), this,           SLOT( slot_areaReset() ) );
    connect( ui->pushButtonStart,     SIGNAL( clicked( bool ) ), this, SLOT( slot_disableAreaWidgets() ) );
    connect( ui->pushButtonStop ,     SIGNAL( clicked( bool ) ), this, SLOT( slot_enableAreaWidgets() ) );

    ui->comboBoxAreaSize->addItems( resolutionStringList );
    connect( ui->comboBoxAreaSize, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( slot_areaSetResolution( QString ) ) );


    // Tab 2 Audio and Videocodec
#ifdef Q_OS_LINUX
    QvkAudioPulse *vkAudioPulse = new QvkAudioPulse( ui );
    connect( vkAudioPulse, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), this,                   SLOT( slot_audioIconOnOff( bool ) ) );
    connect( vkAudioPulse, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->labelAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( vkAudioPulse, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->comboBoxAudioCodec, SLOT( setEnabled( bool ) ) );
    vkAudioPulse->init();
#endif

#ifdef Q_OS_WIN
    QvkAudioWindows *vkAudioWindows = new QvkAudioWindows( ui );
    connect( vkAudioWindows, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), this,                   SLOT( slot_audioIconOnOff( bool ) ) );
    connect( vkAudioWindows, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->labelAudioCodec,    SLOT( setEnabled( bool ) ) );
    connect( vkAudioWindows, SIGNAL( signal_haveAudioDeviceSelected( bool ) ), ui->comboBoxAudioCodec, SLOT( setEnabled( bool ) ) );
    vkAudioWindows->init();
#endif
    connect( vkTheme, SIGNAL( signal_newTheme() ), this, SLOT( slot_audioRedCross() ) );

    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_VideoCodecs_in_Combox( QString ) ) );
    connect( ui->comboBoxFormat, SIGNAL( currentTextChanged( QString ) ), this, SLOT( slot_set_available_AudioCodecs_in_Combox( QString ) ) );

    connect( ui->comboBoxVideoCodec, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( slot_videoCodecChanged( QString ) ) );
    connect( ui->toolButtonx264Reset, SIGNAL( clicked( bool ) ), this, SLOT( slot_x264Reset() ) );
    connect( ui->toolButtonOpenh264Reset, SIGNAL( clicked( bool ) ), this, SLOT( slot_openh264Reset() ) );

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
    connect( ui->PushButtonVideoPath, SIGNAL( clicked( bool ) ),        this, SLOT( slot_newVideoPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetNewPath() ) );
    connect( ui->lineEditVideoPath,   SIGNAL( textChanged( QString ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    connect( videoFileSystemWatcher,  SIGNAL( directoryChanged( const QString& ) ), this, SLOT( slot_videoFileSystemWatcherSetButtons() ) );
    ui->lineEditVideoPath->setText( QStandardPaths::writableLocation( QStandardPaths::MoviesLocation ) );

    ui->comboBoxScale->addItems( resolutionStringList );
    connect( ui->checkBoxScale,   SIGNAL( toggled( bool ) ), ui->comboBoxScale, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStart, SIGNAL( clicked( bool ) ), ui->frameScale, SLOT( setEnabled( bool ) ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked( bool ) ), ui->frameScale, SLOT( setDisabled( bool ) ) );

    vkSystrayAlternative = new QvkSystrayAlternative( this, ui, sliderShowInSystrayAlternative );
    if ( QSystemTrayIcon::isSystemTrayAvailable() == true )
    {
        connect( ui->checkBoxShowInSystray, SIGNAL( clicked( bool ) ), this, SLOT( slot_setVisibleSystray( bool ) ) );
        ui->frameShowInSystrayAlternative->hide();
    } else
    {
        connect( ui->checkBoxShowInSystrayAlternative, SIGNAL( clicked( bool ) ), vkSystrayAlternative, SLOT( setVisible( bool ) ) );
        ui->frameShowInSystray->hide();
    }

    // Tab 5 Available muxer, encoder etc.
    QIcon iconAvailable = style()->standardIcon( QStyle::SP_DialogApplyButton );
    QSize size = iconAvailable.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    ui->labelAvalible->setPixmap( iconAvailable.pixmap( size, QIcon::Normal, QIcon::On ));

    QIcon iconNotAvailable = style()->standardIcon( QStyle::SP_DialogCancelButton );
    ui->labelNotAvailable->setPixmap( iconNotAvailable.pixmap( size, QIcon::Normal, QIcon::On ));
    // End Tabs

    // Tab 6 About
    QPixmap pixmap( ":/pictures/screencast/author.png" );
    ui->labelAutorPicture->setPixmap( pixmap );
    ui->labelSourcecodeUrl->setText( "<a href='https://github.com/vkohaupt/vokoscreenNG'>" + tr( "Sourcecode" ) + "</a>" );
    ui->labelWebSiteUrl->setText( "<a href='http://linuxecke.volkoh.de/vokoscreen/vokoscreen.html'>" + tr( "Homepage" ) + "</a>" );
    ui->labelLanguageUrl->setText( "<a href='https://www.transifex.com/projects/p/vokoscreen/'>" + tr( "Translations" ) + "</a>" );
    ui->labelDonateUrl->setText( "<a href='http://linuxecke.volkoh.de/vokoscreen/vokoscreen-donate.html'>" + tr( "Donate" ) + "</a>" );

    // Close GUI
    connect( this,      SIGNAL( signal_close() ),       ui->pushButtonContinue, SLOT( click() ) );
    connect( this,      SIGNAL( signal_close() ),       ui->pushButtonStop,     SLOT( click() ) );
    connect( this,      SIGNAL( signal_close( bool ) ), ui->checkBoxCameraOnOff,SLOT( setChecked( bool ) ) );
    connect( this,      SIGNAL( signal_close() ),       vkHelp,                 SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkLicenses,             SLOT( close() ) );
    connect( this,      SIGNAL( signal_close() ),       vkSystrayAlternative,   SLOT( close() ) );

    VK_Supported_Formats_And_Codecs();
    VK_Check_is_Format_available();
    VK_set_available_Formats_in_Combox();

    VK_gst_formatVideoAudoicodec_available();
    VK_gst_Elements_available();

    QvkScreenManager *screenManager = new QvkScreenManager;
    connect( screenManager, SIGNAL( signal_clear_widget() ),                          ui->comboBoxScreencastScreen, SLOT( clear() ) );
    connect( screenManager, SIGNAL( signal_screen_count_changed( QString, QString) ), this,                         SLOT( slot_screenCountChanged( QString, QString ) ) );
    emit qApp->screenAdded(Q_NULLPTR);


    // *****************Begin Camera *********************************
    vkCameraController = new QvkCameraController(ui);
    Q_UNUSED(vkCameraController);
    vk_setCornerWidget( ui->tabWidgetCamera );
    // *****************End Camera ***********************************


    // *****************Begin Log *********************************
    vk_setCornerWidget( ui->tabWidgetLog );
    connect( ui->pushButtonSendReport, SIGNAL( clicked( bool ) ), this, SLOT( slot_sendReport() ) );
    // *****************End Log ***********************************

    ui->label_Upate->clear();
    QPalette palette = QPalette( ui->label_Upate->palette() );
    palette.setColor( palette.WindowText, QColor( Qt::magenta ) );
    ui->label_Upate->setPalette( palette );
    connect( &version, SIGNAL( signal_newVersionAvailable( QString ) ), this, SLOT( slot_newVersionAvailable( QString ) ) );
    connect( ui->checkBoxLookForUpdates, SIGNAL( toggled( bool ) ), &version, SLOT( slot_doDownload( bool ) ) );


    QvkLoadExtension *loadExtension = new QvkLoadExtension( this, ui );
    loadExtension->loadExtension();

    // Hint:
    vkSettings.readAll( ui, this );
    vkSettings.readAreaScreencast( vkRegionChoise );
    vkSettings.readCamera( vkCameraController );
    vkSettings.readSystrayAlternative( vkSystrayAlternative );
    vkSettings.readPlayerPathOpenFile( vkPlayer );

    // After reading the settings, we read the arguments and run
    QStringList arguments = QApplication::instance()->arguments();
    if ( arguments.count() > 1  )
    {
        qDebug().noquote() << global::nameOutput << "started from file:" << arguments.at(1);
        vkPlayer->setMediaFile( arguments.at(1) );
        vkPlayer->slot_play();
        ui->tabWidgetSideBar->setCurrentIndex( ui->tabWidgetSideBar->indexOf( ui->tabSidebarPlayer ) );
    }
}


QvkMainWindow::~QvkMainWindow()
{
    delete ui;
}


void QvkMainWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    vkSettings.saveAll( ui, this, false );
    vkSettings.saveAreaScreencast( vkRegionChoise->getXRecordArea() / vkRegionChoise->screen->devicePixelRatio(),
                                   vkRegionChoise->getYRecordArea() / vkRegionChoise->screen->devicePixelRatio(),
                                   vkRegionChoise->getWidth() / vkRegionChoise->screen->devicePixelRatio(),
                                   vkRegionChoise->getHeight() / vkRegionChoise->screen->devicePixelRatio() );
    vkSettings.saveCamera( vkCameraController->cameraWindow->geometry().x(), vkCameraController->cameraWindow->geometry().y() );
    vkSettings.saveSystrayAlternative( vkSystrayAlternative->x(), vkSystrayAlternative->y() );
    vkSettings.savePlayerPathOpenFile( vkPlayer->pathOpenFile );
    emit signal_close();
    emit signal_close( false );
}


void QvkMainWindow::slot_newVersionAvailable( QString update )
{
    if ( ui->checkBoxLookForUpdates->isChecked() == true )
    {
        if ( global::version < update )
        {
            ui->label_Upate->setText( "New Version available: " + update  );
        }
        else
        {
            ui->label_Upate->setText( "No update available" );
        }
    }
    else
    {
        ui->label_Upate->clear();
    }
}


void QvkMainWindow::slot_vokoPlayer()
{
    vkPlayer->show();
}


#ifdef Q_OS_LINUX
void QvkMainWindow::showEvent( QShowEvent *event )
{
    Q_UNUSED(event);
    // Call slot "afterWindowShown" after the window has been shown
    QMetaObject::invokeMethod( this, "slot_afterWindowShown", Qt::ConnectionType::QueuedConnection );

}


void QvkMainWindow::slot_afterWindowShown()
{
    if ( onlyOnce == false )
    {
        onlyOnce = true;

        if ( qgetenv( "XDG_SESSION_TYPE" ).toLower() == "wayland" )
        {
            qDebug().noquote() << global::nameOutput << tr( "Desktop session is a Wayland session" );
            QMessageBox *messageBox = new QMessageBox();
            QIcon icon( QString::fromUtf8( ":/pictures/logo/logo.png" ) );
            messageBox->setWindowIcon( icon );
            messageBox->setWindowTitle( global::name + " " + global::version );
            messageBox->setIcon( QMessageBox::Information );
            messageBox->setText( tr( "Wayland desktop session detected" ) );
            messageBox->setInformativeText( tr( "To time Wayland is not supported. A screencast show a black screen. Please logout and start a X11 Desktop session" ) );
            messageBox->exec();
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "Desktop session is a X11 session";
        }

        // This is only for GNOME
        if ( qgetenv( "XDG_CURRENT_DESKTOP" ).toLower() == "gnome" )
        {
            // Problem 1. Limit the availableSize without the top Menue
            QScreen *screen = QGuiApplication::primaryScreen();
            vkRegionChoise->screenWidth  = screen->availableSize().width();
            vkRegionChoise->screenHeight = screen->availableSize().height();

            // Problem 2. At the first start of area, the Gnome menu hide.
            if ( ui->radioButtonScreencastArea->isChecked() == true )
            {
                ui->radioButtonScreencastFullscreen->click();
                ui->radioButtonScreencastArea->click();
                return;
            }

            if ( ui->radioButtonScreencastFullscreen->isChecked() == true )
            {
                ui->radioButtonScreencastArea->click();
                ui->radioButtonScreencastFullscreen->click();
                return;
            }

            if ( ui->radioButtonScreencastWindow->isChecked() == true )
            {
                ui->radioButtonScreencastArea->click();
                ui->radioButtonScreencastWindow->click();
                return;
            }
        }
    }
}
#endif


void QvkMainWindow::slot_x264Reset()
{
    ui->comboBoxx264Preset->setCurrentIndex( 1 );
    sliderX264->setValue( 17 );
}


void QvkMainWindow::slot_openh264Reset()
{
    sliderOpenh264->setValue( 23 );
}


void QvkMainWindow::slot_videoCodecChanged( QString codec )
{
    ui->frameVideoCodecx264->setVisible( false );
    ui->frameVideoCodecVaapih264->setVisible( false );
    ui->frameVideoCodecOpenh264->setVisible( false );

    if ( codec == "x264"  )
    {
        ui->frameVideoCodecx264->setVisible( true );
    }

    if ( codec == "H.264 (Intel GPU)" )
    {
        ui->frameVideoCodecVaapih264->setVisible( true );
    }

    if ( codec == "openh264" )
    {
        ui->frameVideoCodecOpenh264->setVisible( true );
    }
}


void QvkMainWindow::slot_sendReport()
{
#ifdef Q_OS_WIN
    QString eol = "\r\n";
#endif
#ifdef Q_OS_LINUX
    QString eol = "\n";
#endif
    QStringList stringList;
    stringList << "mailto:";
    stringList << "vkohaupt@freenet.de";
    stringList << "?";
    stringList << "subject=";
    stringList << QString( global::name + QString( " " ) +  global::version );
    stringList << "&";
    stringList << "body=";
    stringList << "Your comment";
    stringList << eol;
    stringList << eol;
    stringList << eol;
    stringList << "Report:";
    stringList << eol;
    stringList << ui->textBrowserLog->toPlainText();
    stringList << eol;
    stringList << "--------------------------------";
    stringList << eol;
    stringList << "Settings:";
    stringList << eol;
    stringList << vkSettings.getFileName();
    stringList << eol;

    // read conf
    QFile file( vkSettings.getFileName() );
    if( !file.open( QIODevice::ReadOnly) )
    {
        QMessageBox::information( Q_NULLPTR, "error", file.errorString() );
    }
    QTextStream in( &file );
    while( !in.atEnd() )
    {
        QString line = in.readLine() + eol;
        stringList << line;
    }
    file.close();

    QString string = stringList.join( "" );
    bool b = QDesktopServices::openUrl( QUrl( string, QUrl::TolerantMode ) );
    Q_UNUSED(b);
}


void QvkMainWindow::slot_setVisibleSystray( bool value )
{
    if ( value == false )
    {
        vkSystray->slot_closeSystray();
        delete vkSystray;
    }

    if ( value == true )
    {
        vkSystray = new QvkSystray(ui);
        vkSystray->init();
        connect( vkSystray, SIGNAL( signal_SystemtrayIsClose() ), this,      SLOT( close() ) );
        connect( this,      SIGNAL( signal_close() ),             vkSystray, SLOT( slot_closeSystray() ) );
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


void QvkMainWindow::slot_areaReset()
{
    ui->comboBoxAreaSize->setCurrentIndex( 0 );
}


void QvkMainWindow::slot_disableAreaWidgets()
{
   if ( ui->radioButtonScreencastArea->isChecked() == true  )
   {
       ui->toolButtonScreencastAreaReset->setEnabled( false );
       ui->comboBoxAreaSize->setEnabled( false );
   }
}


void QvkMainWindow::slot_enableAreaWidgets()
{
   if ( ui->radioButtonScreencastArea->isChecked() == true  )
   {
       ui->toolButtonScreencastAreaReset->setEnabled( true );
       ui->comboBoxAreaSize->setEnabled( true );
   }
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
    QString dir = QFileDialog::getExistingDirectory( this,
                                                     "",
                                                     QStandardPaths::writableLocation( QStandardPaths::HomeLocation ),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dir > "" )
    {
        ui->lineEditVideoPath->setText( dir );
    }
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


void QvkMainWindow::vk_setCornerWidget( QTabWidget *tabWidget )
{
#ifdef Q_OS_LINUX
    QPixmap pixmap( ":/pictures/linux.png" );
#endif
#ifdef Q_OS_WIN
    QPixmap pixmap( ":/pictures/windows.png" );
#endif
    pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    QLabel *label = new QLabel();
    label->setPixmap( pixmap );
    label->setEnabled( false );
    tabWidget->setCornerWidget( label, Qt::TopRightCorner);
}

// If new Theme, set or not set red cross
void QvkMainWindow::slot_audioRedCross()
{
    if ( isAudioDeviceSelected() == true )
    {
        slot_audioIconOnOff( true );
    }
    else
    {
        slot_audioIconOnOff( false );
    }
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

/*
 * Set a new icon with a red cross
 */
void QvkMainWindow::slot_audioIconOnOff( bool state )
{
    if ( ( ( isAudioDeviceSelected() == false ) and ( state == false ) ) or
         ( ( isAudioDeviceSelected() == false ) and ( state == true ) ) or
         ( ( isAudioDeviceSelected() == true )  and ( state == false ) ) )
    {
        QIcon myIcon = vkTheme->VK_getIcon( "audio-input-microphone", ":/pictures/screencast/microphone.png" );
        QSize size = ui->tabWidgetScreencast->iconSize();
        QPixmap workPixmap( myIcon.pixmap( size ) );
        QPainter painter;
        QPen pen;
        painter.begin( &workPixmap );
        pen.setColor( Qt::red );
        pen.setWidth( 2 );
        painter.setPen( pen );
        painter.drawLine ( 5, 5, size.width()-5, size.height()-5 );
        painter.drawLine ( 5, size.height()-5, size.width()-5, 5 );
        painter.end();
        vkTheme->makeAndSetValidIcon( ui->tabWidgetScreencast,
                                      ui->tabWidgetScreencast->indexOf( ui->tabAudio ),
                                      QIcon( workPixmap ) );
    }
    else{
        vkTheme->makeAndSetValidIcon( ui->tabWidgetScreencast,
                                      ui->tabWidgetScreencast->indexOf( ui->tabAudio ),
                                      vkTheme->VK_getIcon( "audio-input-microphone", ":/pictures/screencast/microphone.png" ) );
    }
}


#ifdef Q_OS_LINUX
QString QvkMainWindow::VK_getXimagesrc()
{
    QString showPointer = "true";
    if( ui->checkBoxMouseCursorOnOff->checkState() == Qt::Checked )
    {
        showPointer = "false";
    }

    if( ( ui->radioButtonScreencastFullscreen->isChecked() == true ) and ( ui->comboBoxScreencastScreen->currentData().toInt() == -1  ) )
    {
        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer;
        QString value = stringList.join( " " );
        return value;
    }

    if ( ( ui->radioButtonScreencastFullscreen->isChecked() == true ) and ( ui->comboBoxScreencastScreen->currentIndex() <= ui->comboBoxScreencastScreen->count()-1 ) )
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
          QString value = stringList.join( " " );
          return value;
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

        QString value = stringList.join( " " );
        return value;
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        int compensation_x = 0;
        int compensation_y = 0;

        // Number of pixels must be divisible by two
        int width = vkRegionChoise->getWidth();
        if ( ( width % 2 ) == 1 )
        {
            compensation_x = 1;
        }

        // Number of pixels must be divisible by two
        int height = vkRegionChoise->getHeight();
        if ( ( height % 2 ) == 1 )
        {
            compensation_y = 1;
        }

        qreal gnomehack = 0;
        if ( qgetenv( "XDG_CURRENT_DESKTOP" ).toLower() == "gnome" )
        {
            QList<QScreen *> screen = QGuiApplication::screens();
            gnomehack = screen.at( ui->comboBoxScreencastScreen->currentIndex() )->availableGeometry().top();
        }

        QStringList stringList;
        stringList << "ximagesrc"
                   << "display-name=" + qgetenv( "DISPLAY" )
                   << "use-damage=false"
                   << "show-pointer=" + showPointer
                   << "startx=" + QString::number( vkRegionChoise->getXRecordArea() )
                   << "starty=" + QString::number( vkRegionChoise->getYRecordArea() + gnomehack )
                   << "endx="   + QString::number( vkRegionChoise->getXRecordArea() + vkRegionChoise->getWidth() - 1 - compensation_x)
                   << "endy="   + QString::number( vkRegionChoise->getYRecordArea() + gnomehack + vkRegionChoise->getHeight() - 1 - compensation_y);
        QString value = stringList.join( " " );
        return value;
    }

    return ""; // prophylactic no error at compiletime
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
        QStringList stringList;
        stringList << "gdiscreencapsrc"
                   << "monitor=" + QString::number( ui->comboBoxScreencastScreen->currentIndex() )
                   << "cursor=" + showPointer;
        value = stringList.join( " " );
    }

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        QStringList stringList;
        stringList << "gdiscreencapsrc"
                   << "cursor=" + showPointer
                   << "x=" + QString::number( vkRegionChoise->getXRecordArea() )
                   << "y=" + QString::number( vkRegionChoise->getYRecordArea() )
                   << "width=" + QString::number( vkRegionChoise->getWidth() )
                   << "height=" + QString::number( vkRegionChoise->getHeight() );
        value = stringList.join( " " );
    }

    return value;
}
#endif


QString QvkMainWindow::VK_getCapsFilter()
{
   QStringList stringList;
   stringList << "capsfilter caps=video/x-raw,framerate="
              << QString::number( sliderFrames->value() )
              << "/1";
   return QString( stringList.join( "" ) );
}


QString QvkMainWindow::VK_getVideoScale()
{
    QString value = ui->comboBoxScale->currentText();
    QStringList valuList = value.split( " " );
    value = "videoscale ! capsfilter caps=video/x-raw,width=" + valuList.at(0) + ",height=" + valuList.at(2);
    return value;
}


void QvkMainWindow::VK_gst_Elements_available()
{
    QStringList list;
#ifdef Q_OS_WIN
    list << "gdiscreencapsrc";
#endif
#ifdef Q_OS_LINUX
    list << "ximagesrc";
    list << "pulsesrc";
#endif
    list << "queue";
    list << "capsfilter";
    list << "videoconvert";
    list << "videorate";
    list << "audioconvert";
    list << "audiorate";
    list << "filesink";
    list << "videoscale";
    list << "h264parse";
    list << "adder";

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
void QvkMainWindow::VK_gst_formatVideoAudoicodec_available()
{
    int rowCount = 0;
    for ( int i = 0; i < globalFormatsList.count(); i++ )
    {
        int rowMuxer = 1;
        int rowVideo = 1;
        int rowAudio = 1;
        QStringList listElements = QString( globalFormatsList.at(i) ).split( "," ); // listElement beinhaltet muxer, Video und Audio

        for ( int x = 0; x < listElements.count(); x++ )
        {
            // Mimetype is not needed
            if ( listElements.at(x).contains( QRegExp( "*mimetype*", Qt::CaseInsensitive, QRegExp::Wildcard ) ) )
            {
                continue;
            }

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
                gst_object_unref( factory );
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "muxer" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = style()->standardIcon( QStyle::SP_DialogCancelButton );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowMuxer, 0 );

                ui->gridLayoutAvailable->addWidget( new QLabel( QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowMuxer, 1 );
                rowMuxer++;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "videocodec" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = style()->standardIcon( QStyle::SP_DialogCancelButton );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowVideo, 2 );

                ui->gridLayoutAvailable->addWidget( new QLabel( QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowVideo, 3 );
                rowVideo++;
            }

            if ( QString( listElements.at( x ) ).section( ":", 0, 0 ) == "audiocodec" )
            {
                QLabel *labelPicture = new QLabel();
                QIcon icon;
                if ( available == true )
                    icon = style()->standardIcon( QStyle::SP_DialogApplyButton );
                else
                    icon = style()->standardIcon( QStyle::SP_DialogCancelButton );
                QSize size = icon.actualSize( QSize( 16, 16 ), QIcon::Normal, QIcon::On );
                labelPicture->setPixmap( icon.pixmap( size, QIcon::Normal, QIcon::On ));
                labelPicture->setAlignment( Qt::AlignRight );
                ui->gridLayoutAvailable->addWidget( labelPicture, rowCount + rowAudio, 4 );
                ui->gridLayoutAvailable->addWidget( new QLabel( QString( listElements.at( x ) ).section( ":", 2, 2 ) ), rowCount + rowAudio, 5 );
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
    QStringList MKV_QStringList = ( QStringList()
                                    << "muxer:matroskamux:mkv"
                                    << "videomimetype:video/x-matroska"
                                    << "audiomimetype:audio/x-matroska"
                                    << "videocodec:x264enc:x264"
                                #ifdef Q_OS_LINUX
                                    << "videocodec:openh264enc:openh264"
                                    << "videocodec:vaapih264enc:H.264 (Intel GPU)"
                                    << "videocodec:vaapimpeg2enc:MPEG-2 (Intel GPU)"
                                #endif
                                    << "videocodec:vp8enc:VP8"
                                    << "audiocodec:vorbisenc:vorbis"
                                    << "audiocodec:flacenc:flac"
                                    << "audiocodec:opusenc:opus"
                                    << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList WEBM_QStringList = ( QStringList()
                                     << "muxer:webmmux:webm"
                                     << "videomimetype:video/webm"
                                     << "audiomimetype:audio/webm"
                                     << "videocodec:vp8enc:VP8"
                                     << "audiocodec:vorbisenc:vorbis"
                                     << "audiocodec:opusenc:opus"
                                   );

    QStringList AVI_QStringList = ( QStringList()
                                     << "muxer:avimux:avi"
                                     << "videomimetype:video/x-msvideo"
                                     << "audiomimetype:audio/x-msvideo"
                                     << "videocodec:x264enc:x264"
                                #ifdef Q_OS_LINUX
                                    << "videocodec:openh264enc:openh264"
                                    << "videocodec:vaapih264enc:H.264 (Intel GPU)"
                                    << "videocodec:vaapimpeg2enc:MPEG-2 (Intel GPU)"
                                #endif
                                     << "videocodec:vp8enc:VP8"
                                     << "audiocodec:lamemp3enc:mp3"
                                   );

    QStringList MP4_QStringList = ( QStringList()
                                    << "muxer:mp4mux:mp4"
                                    << "videomimetype:video/mp4"
                                    << "audiomimetype:audio/mpeg"
                                    << "videocodec:x264enc:x264"
                                #ifdef Q_OS_LINUX
                                    << "videocodec:openh264enc:openh264"
                                    << "videocodec:vaapih264enc:H.264 (Intel GPU)"
                                    << "videocodec:vaapimpeg2enc:MPEG-2 (Intel GPU)"
                                #endif
                                    << "audiocodec:lamemp3enc:mp3"
                                    << "audiocodec:opusenc:opus"
                                  );

    QStringList MOV_QStringList = ( QStringList()
                                    << "muxer:qtmux:mov"
                                    << "videomimetype:video/mp4"
                                    << "audiomimetype:audio/mpeg"
                                    << "videocodec:x264enc:x264"
                                #ifdef Q_OS_LINUX
                                    << "videocodec:openh264enc:openh264"
                                    << "videocodec:vaapih264enc:H.264 (Intel GPU)"
                                    << "videocodec:vaapimpeg2enc:MPEG-2 (Intel GPU)"
                                #endif
                                    << "videocodec:vp8enc:VP8"
                                    << "audiocodec:lamemp3enc:mp3"
                                    << "audiocodec:opusenc:opus"
                                  );

    videoFormatsList.clear();
    videoFormatsList.append( MKV_QStringList.join( ","  ) );
    videoFormatsList.append( WEBM_QStringList.join( ","  ) );
    videoFormatsList.append( AVI_QStringList.join( "," ) );
    videoFormatsList.append( MP4_QStringList.join( ",") );
    videoFormatsList.append( MOV_QStringList.join( ",") );

    globalFormatsList << videoFormatsList;
}


void QvkMainWindow::VK_Check_is_Format_available()
{
    qDebug().noquote() << global::nameOutput << "Symbols: + available, - not available";
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

        QMimeDatabase mimeDatabase;
        QStringList listKeyVideoMimetype = listKeys.filter( "videomimetype" );
        QMimeType mimetype = mimeDatabase.mimeTypeForName( QString( listKeyVideoMimetype.at( 0 ) ).section( ":", 1 ) );
        QIcon icon = vkTheme->VK_getIcon( mimetype.iconName(), ":/pictures/screencast/strip.png" );

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
        QString name =    QString( listKeyVideoCodec.at( i ) ).section( ":", 2, 2 );
        GstElementFactory *factory = gst_element_factory_find( encoder.toLatin1() );
        if ( !factory )
        {
            qDebug().noquote() << global::nameOutput << "-" << encoder;
        }
        else
        {
            qDebug().noquote() << global::nameOutput << "+" << encoder;
            ui->comboBoxVideoCodec->addItem( name, encoder );
            gst_object_unref( factory );
        }
    }

    if ( ui->comboBoxVideoCodec->count() == 0  )
        ui->pushButtonStart->setEnabled( false);
    else
        ui->pushButtonStart->setEnabled( true );
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
    QString vk_idealThreadCount;
    if ( QThread::idealThreadCount() == -1 )
    {
        vk_idealThreadCount = "0";
    }
    else
    {
        vk_idealThreadCount = QString::number( QThread::idealThreadCount() );
    }

    QString value;
    QString encoder = ui->comboBoxVideoCodec->currentData().toString();
    if ( encoder == "x264enc" )
    {
        QStringList list;
        list << ui->comboBoxVideoCodec->currentData().toString();
        list << "qp-min=" + QString::number( sliderX264->value() );
        list << "qp-max=" + QString::number( sliderX264->value() );
        list << "speed-preset=" + ui->comboBoxx264Preset->currentText();
        list << "threads=" + vk_idealThreadCount;
        value = list.join( " " );
    }

    if ( encoder == "openh264enc" )
    {
        QStringList list;
        list << ui->comboBoxVideoCodec->currentData().toString();
        list << "qp-min=" + QString::number( sliderOpenh264->value() );
        list << "qp-max=" + QString::number( sliderOpenh264->value() );
        list << "usage-type=screen";
        list << "complexity=low";
        list << "multi-thread=" + vk_idealThreadCount;
        value = list.join( " " );
        value.append( " ! h264parse" );
    }

    if ( encoder == "x265enc" )
    {
        value = "x265enc";
    }

    if ( encoder == "vaapih264enc" )
    {
        value = "vaapih264enc";
    }

    if ( encoder == "vaapimpeg2enc" )
    {
            value = "vaapimpeg2enc";
    }

    if ( encoder == "vp8enc" )
    {
        QStringList list;
        list << "vp8enc";
        list << "min_quantizer=20";
        list << "max_quantizer=20";
        list << "cpu-used=" + vk_idealThreadCount;
        list << "deadline=1000000";
        list << "threads=" + vk_idealThreadCount;
        value = list.join( " " );
    }

    return value;
}


void QvkMainWindow::slot_preStart()
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


    if ( ( ui->radioButtonScreencastFullscreen->isChecked() == true ) and  ( sliderScreencastCountDown->value() > 0 ) )
    {
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkCountdown->startCountdown( sliderScreencastCountDown->value() );
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


    if ( ( ui->radioButtonScreencastArea->isChecked() == true ) and ( sliderScreencastCountDown->value() > 0 ) )
    {
        disconnect( vkCountdown, nullptr, nullptr, nullptr );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonStop,  SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countdownBegin( bool ) ),  ui->pushButtonPause, SLOT( setDisabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonStop,  SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), ui->pushButtonPause, SLOT( setEnabled( bool ) ) );
        connect( vkCountdown, SIGNAL( signal_countDownfinish( bool ) ), this,                SLOT( slot_Start() ) );
        vkRegionChoise->recordMode( true );
        vkCountdown->startCountdown( sliderScreencastCountDown->value() );
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


#ifdef Q_OS_WIN
QString QvkMainWindow::VK_get_AudioSystem()
{
    return "directsoundsrc";
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
#ifdef Q_OS_LINUX
    string = pipeline.prepend( "gst-launch-1.0 -e \\\n    " );
#endif
#ifdef Q_OS_WIN
    string = pipeline.prepend( "gst-launch-1.0.exe -e \\\n    " );
#endif
    string = pipeline.replace( "mux.", "mux. \\\n   " );
    string = pipeline.replace( "mix.", "mix. \\\n   " );
    string = pipeline.replace( "!", "\\\n        !" );
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
    VK_PipelineList << "videoconvert";
    if ( ui->checkBoxScale->isChecked() )
    {
       VK_PipelineList << VK_getVideoScale();
    }
    VK_PipelineList << "videorate";
    VK_PipelineList << Vk_get_Videocodec_Encoder();

    // Only if one or more audiodevice is selected
    if ( ( VK_getSelectedAudioDevice().count() > 0 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        VK_PipelineList << "queue";
        VK_PipelineList << "mux.";
    }

    // Pipeline for one selected audiodevice
    // 2019-09-28 tested and ok on opensuse 15.0 und nativ Windows 10 from USB-Stick
    if ( ( VK_getSelectedAudioDevice().count() == 1 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        #ifdef Q_OS_LINUX
            VK_PipelineList << VK_get_AudioSystem().append( " device=" ).append( VK_getSelectedAudioDevice().at(0) );
        #endif

        #ifdef Q_OS_WIN
            VK_PipelineList << VK_get_AudioSystem().append( " device-name=" ).append( "'" + VK_getSelectedAudioDevice().at(0) +"'" );
        #endif

        VK_PipelineList << "audioconvert";
        VK_PipelineList << "audiorate";
        VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
        VK_PipelineList << "queue";
        VK_PipelineList << "mux.";
    }

    // Pipeline for more as one audiodevice
    if ( ( VK_getSelectedAudioDevice().count() > 1 ) and ( ui->comboBoxAudioCodec->count() > 0 ) )
    {
        for ( int x = 0; x < VK_getSelectedAudioDevice().count(); x++ )
        {
            #ifdef Q_OS_LINUX
                VK_PipelineList << VK_get_AudioSystem().append( " device=" ).append( VK_getSelectedAudioDevice().at(x) );
                VK_PipelineList << "queue";
                VK_PipelineList << "mix.";
            #endif

            #ifdef Q_OS_WIN
                VK_PipelineList << VK_get_AudioSystem().append( " device-name=" ).append( "'" + VK_getSelectedAudioDevice().at(x) +"'" );
                VK_PipelineList << "queue";
                VK_PipelineList << "mix.";
            #endif
        }
        VK_PipelineList << "adder name=mix";
        VK_PipelineList << "audioconvert";
        VK_PipelineList << "audiorate";
        VK_PipelineList << ui->comboBoxAudioCodec->currentData().toString();
        VK_PipelineList << "queue";
        VK_PipelineList << "mux.";
    }

    VK_PipelineList << VK_getMuxer();

    QString newVideoFilename = global::name + "-" + QDateTime::currentDateTime().toString( "yyyy-MM-dd_hh-mm-ss" ) + "." + ui->comboBoxFormat->currentText();
    VK_PipelineList << "filesink location=" + ui->lineEditVideoPath->text() + "/" + newVideoFilename;

    // Write settings to log
    vkSettings.saveAll( ui, this, true );

    QString VK_Pipeline = VK_PipelineList.join( VK_Gstr_Pipe );
    VK_Pipeline = VK_Pipeline.replace( "mix. !", "mix." );
    VK_Pipeline = VK_Pipeline.replace( "mux. !", "mux." );

    qDebug().noquote() << global::nameOutput << "Start record with:" << VK_Pipeline;
    qDebug( " " );
    qDebug().noquote() << Pipeline_structured_output( VK_Pipeline );

    QByteArray byteArray = VK_Pipeline.toUtf8();
    const gchar *line = byteArray.constData();
    GError *error = Q_NULLPTR;
    pipeline = gst_parse_launch( line, &error );

    // Start playing
    GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
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

    if ( ui->radioButtonScreencastArea->isChecked() == true )
    {
        vkRegionChoise->recordMode( false );
        vkRegionChoise->repaint();
        vkRegionChoise->update();
    }
}


void QvkMainWindow::slot_Stop()
{
    if ( vkLimitDiskFreeSpace->isStorageOK() == false )
    {
        ui->checkBoxShowInSystray->click();
        ui->checkBoxShowInSystray->click();
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
        ret = gst_element_set_state( pipeline, GST_STATE_READY );
        ret = gst_element_set_state( pipeline, GST_STATE_NULL );
        gst_object_unref( pipeline );
        qDebug().noquote() << global::nameOutput << "Stop record";
    }

    wantRecording = true;

}


void QvkMainWindow::slot_Pause()
{
    if ( ui->pushButtonStart->isEnabled() == false )
    {
        qDebug().noquote() << global::nameOutput << "Pause was clicked";
        GstStateChangeReturn ret = gst_element_set_state( pipeline, GST_STATE_PAUSED ); // so wie es aussieht hängt er nur mit Audio
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

    qDebug().noquote() << global::nameOutput << "play video with vokoplayer";
    ui->tabWidgetSideBar->setCurrentIndex( ui->tabWidgetSideBar->indexOf( ui->tabSidebarPlayer ) );
    QString string;
    string.append( ui->lineEditVideoPath->text() );
    string.append( "/" );
    string.append( videoFileList.at( 0 ) );
    vkPlayer->setMediaFile( string );
    vkPlayer->slot_play();
}

void QvkMainWindow::slot_Folder()
{
    if ( QDesktopServices::openUrl( QUrl( ui->lineEditVideoPath->text(), QUrl::TolerantMode ) ) == false )
    {
        QMessageBox msgBox( this );
        msgBox.setText( tr( "No filemanager found." ) + "\n" + tr( "Please install a filemanager." ) );
        msgBox.setWindowTitle( global::name + " " + global::version );
        msgBox.setIcon( QMessageBox::Information );
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
}
