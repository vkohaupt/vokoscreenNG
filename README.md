# vokoscreenGST

* Vokoscreen 2.5.0 ist the last version with ffmpg.
* vokoscreenGST is the devlopname for vokoscreen 3.0 and based of Qt and gstreamer.
* It is written from scratch

# progress
* record fullscreen                       --> OK
* record window                           --> OK
* record area                             --> in development
* detect systemVideoplayer for play last record --> OK
* free adjustable video folder            --> OK
* countdown                               --> OK
* do not record mouse cusor               --> OK
* audio on/off                            --> OK
* adjustable fps                          --> OK
* webcam                                  --> Ready but not installed
* detect system Format                    --> in development
* detect system videocodecs               --> in development
* detect system audiocodec                --> in development
* hotkey                                  --> in development
* ...
* ...
* ...

# Windows 10
### BuildRequires
* https://download.qt.io/official_releases/qt/5.9/5.9.2/qt-opensource-windows-x86-5.9.2.exe
* https://gstreamer.freedesktop.org/pkg/windows/1.12.4/gstreamer-1.0-x86-1.12.4.msi
* https://gstreamer.freedesktop.org/pkg/windows/1.12.4/gstreamer-1.0-devel-x86-1.12.4.msi

# Linux
### Requires:
* Qt5 >= 5.9.4
* gstreamer >= 1.12.5
* gstreamer-plugins-bad
* gstreamer-plugins-bad-orig-addon
* gstreamer-plugins-good-extra
* pulseaudio

### BuildRequires:
* pkgconfig
* libqt5-devel
* libqt5-linguist
