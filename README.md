# vokoscreenNG (New Generation)

* For **Linux** and **Windows**
* vokoscreenNG based of Qt and GStreamer.
* It is written from scratch.
* The old vokoscreen 2.5.0 https://github.com/vkohaupt/vokoscreen is the last version with ffmpg and will not more continue developed.

## Translate
Only **100% translated** languages come in vokoscreenNG.
You can translate to anytime, in a bugfix release your translation will be included.
Please translate only at Transifex, it is very easy.
https://www.transifex.com/vkohaupt/vokoscreen/


## Source code posts
If you want write code, then can you do this. Your source code must be in separate files and published under the MIT-License.
Every file must include the License-header.
The License-header find you here. https://github.com/vkohaupt/vokoscreenNG/blob/3.0/Third-Party-License/MIT-License


## Windows 10
### Requires:
* The vokoscreenNG Player need a codec pack.
* I test with this codec pack
* windows.10.codec.pack.v2.1.5.setup.exe
* https://www.windows10codecpack.com/


### BuildRequires
* https://download.qt.io/official_releases/qt/5.9/5.9.4/qt-opensource-windows-x86-5.9.4.exe
* https://gstreamer.freedesktop.org/pkg/windows/1.14.4/gstreamer-1.0-x86-1.14.4.msi
* https://gstreamer.freedesktop.org/pkg/windows/1.14.4/gstreamer-1.0-devel-x86-1.14.4.msi


### Environment
* GST_PLUGIN_PATH=C:\gstreamer\1.0\x86\lib\gstreamer-1.0
* GSTREAMER_1_0_ROOT_X86=C:\gstreamer\1.0\x86\

## Linux
### Requires:
* Qt5 >= 5.9.4
* gstreamer >= 1.12.5
* gstreamer-plugins-bas
  - Audio: vorbis, opus
* gstreamer-plugins-good
  - Format: mkv, avi, webm, mp4, mov
  - Video: vp8
  - Audio: flac
* gstreamer-plugins-bad
  - Video: camera
* gstreamer-plugins-ugly
  - Video: x264
  - Audio: mp3lame
* gstreamer-plugins-ugly-orig-addon (opensuse packmann)
  - Video: x264
* gstreamer-plugins-libav
  - Player: x264
* gstreamer-plugins-vaapi and intel-vaapi-driver
  - Intel Hardwareenconding H.264 and MPEG2
* pulseaudio

### BuildRequires:
* pkgconfig
* libqt5-devel
* libqt5-linguist
* libX11-devel
* gstreamer-devel


![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-tab--1.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-tab--2.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-tab--3.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-tab--4.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-tab--5.png)

