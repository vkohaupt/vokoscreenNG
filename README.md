# vokoscreenNG (New Generation)

* For **Linux** and **Windows**
* vokoscreenNG is based of Qt and GStreamer.

## Download
* Windows https://linuxecke.volkoh.de/vokoscreen/vokoscreen-download.html

## Translate
Only **100% translated** languages will be added to vokoscreenNG.
You can translate to anytime, your translation will be included in the next bugfix release.
Please translate only at Transifex, it is very easy.
https://www.transifex.com/vkohaupt/vokoscreen/


## Code Contributions
If you want to contribute code, you are free to do so. Your source code must be in separate files and published under the MIT-License.
Every file must include the License-header.
The License-header can be found here: https://github.com/vkohaupt/vokoscreenNG/blob/3.0/Third-Party-License/MIT-License


## Windows 10
### Requirements:
* The integrated vokoscreenNG Player needs a codec pack.
* I test with this codec pack: https://www.windows10codecpack.com/


### BuildRequires
* https://download.qt.io/official_releases/qt/5.9/5.9.4/qt-opensource-windows-x86-5.9.4.exe
* https://gstreamer.freedesktop.org/pkg/windows/1.14.4/gstreamer-1.0-x86-1.14.4.msi
* https://gstreamer.freedesktop.org/pkg/windows/1.14.4/gstreamer-1.0-devel-x86-1.14.4.msi


### Environment
* GST_PLUGIN_PATH=C:\gstreamer\1.0\x86\lib\gstreamer-1.0
* GSTREAMER_1_0_ROOT_X86=C:\gstreamer\1.0\x86\

## Linux
### Run-time requirements:
* Qt5 >= 5.9.4
* pulseaudio
* gstreamer >= 1.12.5
* gstreamer-plugins-bas
  - Audio: vorbis, opus
* gstreamer-plugins-good
  - Format: mkv, avi, webm, mp4, mov
  - Video: vp8
  - Audio: flac
* gstreamer-plugins-bad
  - Video: camera
* gstreamer-plugins-bad-orig-addon (openSUSE Packman)
  - Video: openh264
* gstreamer-plugins-ugly
  - Video: x264
  - Audio: mp3lame
* gstreamer-plugins-ugly-orig-addon (openSUSE Packman)
  - Video: x264
* gstreamer-plugins-libav
  - Player: x264
* gstreamer-plugins-vaapi and intel-vaapi-driver
  - Intel Hardwareenconding H.264 and MPEG2


### Build requirements
* pkgconfig
* libqt5-devel
* libqt5-linguist
* libX11-devel
* gstreamer-devel

On Ubuntu these requirements can be satisfied by running `sudo apt install libgstreamer1.0-dev qt5-default qttools5-dev-tools libqt5x11extras5-dev qtmultimedia5-dev`

## Install packages
* Ubuntu(unofficial backport)
  - sudo add-apt-repository ppa:ubuntuhandbook1/apps
  - sudo apt update && sudo apt install vokoscreen-ng
  - sudo apt install ubuntu-restricted-extras

* [Fedora](https://src.fedoraproject.org/rpms/vokoscreenNG): `sudo dnf install vokoscreenNG`
* [openSUSE](https://software.opensuse.org/package/vokoscreenNG) (Tumbleweed and Leap 15.2): `sudo zypper install vokoscreenNG`
* [Snap](https://snapcraft.io/vokoscreen-ng): `sudo snap install vokoscreen-ng`

![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-0.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-1.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-2.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-3.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-4.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-5.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-6.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-7.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-8.png)
