#include "QvkSettings.h" 

#include <QFile>
#include <QApplication>
#include <QColor>
using namespace std;

QvkSettings::QvkSettings(){}
QvkSettings::~QvkSettings(){}

void QvkSettings::readAll()
{
    // Read from file VERSION progname and versionsnumber
    QSettings versionSettings(":/VERSION", QSettings::IniFormat );
    versionSettings.beginGroup("Info");
      bool beta = versionSettings.value( "Beta" ).toBool();
      QString Beta;
      if ( beta )
        Beta = "Beta";
      else
        Beta = "";
      
      ProgName = versionSettings.value( "Progname" ).toString();
      Version = versionSettings.value( "Version" ).toString() + " " + Beta;
    versionSettings.endGroup();
    
    // Einstellungen aus .conf einlesen
    QSettings settings( getProgName(), getProgName() );
    
    // Reset all settings at next start from vokoscreen
    settings.beginGroup( "Miscellaneous" );
      int reset = settings.value( "Reset", 0 ).toUInt();
      if ( reset == 2 )
      {
        settings.clear();
      }
    settings.endGroup();
    
    // Dient nur zum anlegen des Profils damit ffmpeglog erstellt werden kann
    settings.beginGroup("vokoscreen");
      settings.setValue("Version", getVersion());
    settings.endGroup();
  
    settings.beginGroup( "Audio" );
      AudioOnOff = settings.value( "AudioOnOff", 2 ).toUInt();
    settings.endGroup();
    
    settings.beginGroup("Alsa" );
      AlsaSelect = settings.value( "Alsa", false ).toBool();
    settings.endGroup();

    settings.beginGroup("Pulse");
      PulseSelect = settings.value( "Pulse", true ).toBool();
    settings.endGroup();

    settings.beginGroup("Record");
      FullScreenSelect = settings.value( "FullScreen", true ).toBool();
      WindowSelect = settings.value( "Window", false ).toBool();
      AreaSelect = settings.value( "Area", false ).toBool();
    settings.endGroup();
    
    settings.beginGroup( "Miscellaneous" );
      VideoPath = settings.value( "VideoPath" ).toString();
      VideoPlayer = settings.value( "Videoplayer" ).toString();
      GIFPlayer = settings.value( "GIFplayer" ).toString();
      Minimized = settings.value( "Minimized", 0 ).toUInt();
      MinimizedByStart = settings.value( "MinimizedByStart", 0 ).toUInt();
      Countdown = settings.value( "Countdown", 0 ).toUInt();
      QFile file;
      if ( file.exists( qApp->applicationDirPath().append( "/bin/ffmpeg" ) ) == true )
      {
        vokoscreenWithLibs = true;
        Recorder = qApp->applicationDirPath().append( "/bin/ffmpeg" );
      }
      else
      {
        vokoscreenWithLibs = false;
        Recorder = settings.value( "Recorder", "ffmpeg" ).toString();
      }
    settings.endGroup();
    
    settings.beginGroup( "Videooptions" );
      // Als Beispiel:
      // Frames könnte "" sein dann wird Wert 0 zugewiesen
      // er könnte aber auch größer 200 haben. In beiden Fällen wird der Standardwert 25 zugewiesen.
      Frames = settings.value( "Frames", 25 ).toInt();
      if ( ( Frames == 0 ) or ( Frames > 200 ) )
      {
        Frames = 25;
      }
      
      VideoCodec = settings.value( "Videocodec", "libx264" ).toString();
      x264Lossless = settings.value( "X264Lossless", 0 ).toInt();
      AudioCodec = settings.value( "Audiocodec", "libvorbis" ).toString();
      VideoContainer = settings.value( "Format", "mkv" ).toString();
      HideMouse = settings.value( "HideMouse").toUInt();
    settings.endGroup();

    settings.beginGroup( "GUI" );
      X = settings.value( "X", "100" ).toUInt();
      Y = settings.value( "Y", "100" ).toUInt();
      Tab = settings.value( "Tab", "0" ).toUInt();
      Systray = settings.value( "Systray", 2 ).toUInt();
    settings.endGroup();
    
    settings.beginGroup( "Area" );
        AreaX = settings.value( "X", 200 ).toInt();
        AreaY = settings.value( "Y", 200 ).toInt();
        AreaWidth = settings.value( "Width", 200 ).toUInt();
        AreaHeight = settings.value( "Height", 200 ).toUInt();
    settings.endGroup();

    settings.beginGroup( "Webcam" );
        //settings.setValue( "WebcamName", myUi.webcamComboBox->currentText() );
        webcamName = settings.value( "WebcamName" ).toString();
        //settings.setValue( "WebcamDevice", myUi.webcamComboBox->currentData() );
        //settings.setValue( "WebcamResolution", myUi.resolutionComboBox->currentText() );
        webcamOnOff = settings.value( "OnOff", 0 ).toUInt();
        webcamX = settings.value( "X", 400 ).toUInt();
        webcamY = settings.value( "Y", 400 ).toUInt();
        webcamWidth = settings.value( "Width", 320 ).toUInt();
        webcamHeight = settings.value( "Height", 240 ).toUInt();
        webcamMirrored = settings.value( "Mirrored", 0 ).toInt();
        webcamBorder = settings.value( "Border", true ).toBool();
        webcamOverFullScreen = settings.value( "OverFullScreen", false ).toBool();
        webcamButtonTopMiddle = settings.value( "Top", false ).toBool();
        webcamButtonRightMiddle = settings.value( "Right", false ).toBool();
        webcamButtonBottomMiddle = settings.value( "Bottom", false ).toBool();
        webcamButtonLeftMiddle = settings.value( "Left", false ).toBool();
        if ( ( getWebcamButtonTopMiddle() == false ) and ( getWebcamButtonRightMiddle() == false ) and ( getWebcamButtonLeftMiddle() == false ) )
        {
           webcamButtonBottomMiddle = true;
        }
        webcamGray = settings.value( "Gray", false ).toBool();
        webcamInvert = settings.value( "Invert", false ).toBool();
    settings.endGroup();
    
    settings.beginGroup( "Magnifier" );
        magnifierOnOff = settings.value( "OnOff", 0 ).toUInt();
        magnifierFormValue = settings.value( "FormValue", 2 ).toUInt();
    settings.endGroup();
    
    settings.beginGroup( "ShowClick" );
        showClickOnOff    = settings.value( "OnOff", 0 ).toInt();
        showClickTime     = settings.value( "Showtime", 5 ).toDouble();
        showClickCircle   = settings.value( "Circle", 70 ).toInt();
        showClickRadiant  = settings.value( "Radiant", false ).toBool();
        showClickColor    = settings.value( "Color", QColor( Qt::red ) ).value<QColor>();
        showClickOpacity  = settings.value( "Opacity", 0.5 ).toDouble();
    settings.endGroup();
    
    settings.beginGroup( "ShowKey" );
        showKeyOnOff = settings.value( "OnOff", 0 ).toInt();
    settings.endGroup();
}

bool QvkSettings::isVokoscreenWithLibs()
{
  return vokoscreenWithLibs;
}


QString QvkSettings::getVersion()
{
  return Version; 
}

QString QvkSettings::getProgName()
{
  return ProgName; 
}

int QvkSettings::getAudioOnOff()
{
  return AudioOnOff; 
}

bool QvkSettings::getAlsaSelect()
{
  return AlsaSelect;
}

bool QvkSettings::getPulseSelect()
{
  return PulseSelect;
}

bool QvkSettings::getFullScreenSelect()
{
  return FullScreenSelect;
}

bool QvkSettings::getWindowSelect()
{
  return WindowSelect;
}

bool QvkSettings::getAreaSelect()
{
  return AreaSelect; 
}

QString QvkSettings::getVideoPath()
{
  return VideoPath;
}

QString QvkSettings::getVideoPlayer()
{
  return VideoPlayer; 
}

QString QvkSettings::getGIFPlayer()
{
  return GIFPlayer;
}

int QvkSettings::getMinimized()
{
  return Minimized; 
}

int QvkSettings::getMinimizedByStart()
{
  return MinimizedByStart;
}

int QvkSettings::getCountdown()
{
  return Countdown; 
}

int QvkSettings::getFrames()
{
  return Frames; 
}

QString QvkSettings::getVideoCodec()
{
  return VideoCodec; 
}

int QvkSettings::getX264Lossless()
{
  return x264Lossless;
}

QString QvkSettings::getAudioCodec()
{
  return AudioCodec; 
}

QString QvkSettings::getVideoContainer()
{
  return VideoContainer; 
}

int QvkSettings::getHideMouse()
{
  return HideMouse; 
}

QString QvkSettings::getRecorder()
{
  return Recorder;  
}



// Gui
int QvkSettings::getX()
{
  return X; 
}

int QvkSettings::getY()
{
  return Y;  
}

int QvkSettings::getTab()
{
  return Tab; 
}

int QvkSettings::getSystray()
{
  return Systray;  
}


// Area
int QvkSettings::getAreaX()
{
  return AreaX;
}

int QvkSettings::getAreaY()
{
  return AreaY;
}

int QvkSettings::getAreaWidth()
{
  return AreaWidth;
}

int QvkSettings::getAreaHeight()
{
  return AreaHeight;
}

// Webcam
QString QvkSettings::getWebcamName()
{
    return webcamName;
}

int QvkSettings::getWebcamOnOff()
{
  return webcamOnOff; 
}

int QvkSettings::getWebcamX()
{
  return webcamX;
}

int QvkSettings::getWebcamY()
{
  return webcamY;
}

int QvkSettings::getWebcamHeight()
{
 return webcamHeight; 
}
 
int QvkSettings::getWebcamWidth()
{
  return webcamWidth;
}

int QvkSettings::getWebcamMirrored()
{
  return webcamMirrored;
}

bool QvkSettings::getWebcamGray()
{
  return webcamGray;
}

bool QvkSettings::getWebcamInvert()
{
  return webcamInvert;
}

bool QvkSettings::getWebcamBorder()
{
  return webcamBorder;  
}

bool QvkSettings::getWebcamOverFullScreen()
{
  return webcamOverFullScreen;
}

bool QvkSettings::getWebcamButtonTopMiddle()
{
  return webcamButtonTopMiddle; 
}

bool QvkSettings::getWebcamButtonRightMiddle()
{
  return webcamButtonRightMiddle;
}

bool QvkSettings::getWebcamButtonBottomMiddle()
{
  return webcamButtonBottomMiddle;
}

bool QvkSettings::getWebcamButtonLeftMiddle()
{
  return webcamButtonLeftMiddle;
}


// Magnifier
int QvkSettings::getMagnifierOnOff()
{
  return magnifierOnOff;
}

int QvkSettings::getMagnifierFormValue()
{
  return magnifierFormValue; 
}


// ShowClick
int QvkSettings::getShowClickOnOff()
{
  return showClickOnOff;
}

bool QvkSettings::getShowClickRadiant()
{
  return showClickRadiant; 
}

QColor QvkSettings::getShowClickColor()
{
  return showClickColor;
}

double QvkSettings::getShowClickOpacity()
{
  return showClickOpacity;
}

int QvkSettings::getShowClickCircle()
{
  return showClickCircle; 
}

double QvkSettings::getShowClickTime()
{
  return showClickTime;
}

// ShowKey
int QvkSettings::getShowKeyOnOff()
{
  return showKeyOnOff; 
}
