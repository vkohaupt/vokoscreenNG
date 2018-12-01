# vokoscreenNG (New Generation)

* For **Linux** and **Windows**
* vokoscreenNG based of Qt and GStreamer.
* It is written from scratch.
* The old vokoscreen 2.5.0 https://github.com/vkohaupt/vokoscreen is the last version with ffmpg and will not more continue developed.

## Translate
Only **100% translate and reviewed** languages come in vokoscreen. This means, you translate and at self time you reviewed your language. You have both finish translate and reviewed, the automated pull Process can now pull your translated language to me in the Sourcecode.
Please translate only at Transifex, it is very easy.
https://www.transifex.com/vkohaupt/vokoscreen/




## Windows 10
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
* gstreamer >= 1.14.4
* gstreamer-plugins-bad
* gstreamer-plugins-bad-orig-addon
* gstreamer-plugins-good-extra
* pulseaudio

### BuildRequires:
* pkgconfig
* libqt5-devel
* libqt5-linguist
* libX11-devel
* gstreamer-devel
