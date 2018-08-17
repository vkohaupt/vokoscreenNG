#ifndef QVKSETTINGS_H 
#define QVKSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QColor>

class QvkSettings: public QObject
{ 
Q_OBJECT
public:
  QvkSettings();
  virtual ~QvkSettings();
  void readAll();

  QString getVersion();
  QString getProgName();
  
  // Audio
  int getAudioOnOff();
  
  // Alsa
  bool getAlsaSelect();
  
  // Pulse
  bool getPulseSelect();
  
  // Record
  bool getFullScreenSelect();
  bool getWindowSelect();
  bool getAreaSelect();
  
  // Miscellaneous
  QString getVideoPath();
  QString getVideoPlayer();
  QString getGIFPlayer();
  int getMinimized();
  int getMinimizedByStart();
  int getCountdown();
  QString getRecorder();
  
  // Videooptions
  int getFrames();
  QString getVideoCodec();
  int getX264Lossless();
  QString getAudioCodec();
  QString getVideoContainer();
  int getHideMouse();

  // Gui
  int getX();
  int getY();
  int getTab();
  int getSystray();
  
  // Area
  int getAreaX();
  int getAreaY();
  int getAreaWidth();
  int getAreaHeight();
  
  // webcam
  QString getWebcamName();
  int getWebcamOnOff();
  int getWebcamX();
  int getWebcamY();
  int getWebcamHeight();
  int getWebcamWidth();
  int getWebcamMirrored();
  bool getWebcamGray();
  bool getWebcamInvert();
  bool getWebcamBorder();
  bool getWebcamOverFullScreen();
  bool getWebcamButtonTopMiddle();
  bool getWebcamButtonRightMiddle();
  bool getWebcamButtonBottomMiddle();
  bool getWebcamButtonLeftMiddle();

  // Magnifier
  int getMagnifierOnOff();
  int getMagnifierFormValue();
  
  // ShowClick
  int    getShowClickOnOff();
  int    getShowClickCircle();
  bool   getShowClickRadiant();
  QColor getShowClickColor();
  double getShowClickOpacity();
  double getShowClickTime();

  // ShowKey
  int getShowKeyOnOff();
  
  
public slots:
  bool isVokoscreenWithLibs();
  
private slots:


signals:

  
protected:  

  
private:
  QString ProgName;
  QString Version;

  int AudioOnOff;
  bool AlsaSelect;
  bool PulseSelect;
  bool FullScreenSelect;
  bool WindowSelect;
  bool AreaSelect;
  int Countdown;
  
  QString VideoPath;
  QString VideoPlayer;
  QString GIFPlayer;
  int Minimized;
  int MinimizedByStart;
  
  int Frames;
  QString VideoCodec;
  int x264Lossless;
  QString AudioCodec;
  QString VideoContainer;
  int HideMouse;
  QString Recorder;
  
  bool vokoscreenWithLibs;
  
  // Gui
  int X;
  int Y;
  int Tab;
  int Systray;
  
  // Area
  int AreaX;
  int AreaY;
  int AreaWidth;
  int AreaHeight;

  // webcam
  QString webcamName;
  int webcamOnOff;
  int webcamX;
  int webcamY;
  int webcamHeight;
  int webcamWidth;
  int webcamMirrored;
  bool webcamGray;
  bool webcamInvert;
  bool webcamBorder;
  bool webcamOverFullScreen;
  bool webcamButtonTopMiddle;
  bool webcamButtonRightMiddle;
  bool webcamButtonBottomMiddle;
  bool webcamButtonLeftMiddle;
  
  // Magnifier
  int magnifierOnOff;
  int magnifierFormValue;
  
  // ShowClick
  int showClickOnOff;
  int showClickCircle;
  bool showClickRadiant;
  QColor showClickColor;
  double showClickOpacity;
  double showClickTime;
  
  // ShowKey
  int showKeyOnOff;
  
};

#endif
