# vokoscreenNG (New Generation)

* For **Windows** and **Linux(X11, Experimental Wayland support since 3.1.0 pre alpha)**
* vokoscreenNG is based of Qt and GStreamer.

## Download
* Windows https://linuxecke.volkoh.de/vokoscreen/vokoscreen-download.html

## Translate
Only **100% translated** languages will be added to vokoscreenNG.
You can translate to anytime, your translation will be included in the next bugfix release.
Please translate only at Transifex, it is very easy.
https://www.transifex.com/vkohaupt/vokoscreen/

## Installer
Windows and Linux installation routines are not provided, because each platform, installer and package manager has its own special features. That means make-install, metadata, desktop files, icons, manpage, logo must be create and provided itself. All the images and logos that come with vokoscreenNG are free. You can be change in size itself be used for your own installer or package manager.

## Code Contributions
If you want to contribute code, please ask beforehand whether your idea finds harmony. Your source code must be in separate files and published under the MIT-License.
Every file must include the License-header.
The License-header can be found here: https://github.com/vkohaupt/vokoscreenNG/blob/3.0/Third-Party-License/MIT-License


## Windows 10
### Requirements
* The integrated vokoscreenNG Player needs a codec pack.

### BuildRequires
* Required from version 3.0.5 to 3.0.9  
   https://download.qt.io/official_releases/qt/5.9/5.9.4/qt-opensource-windows-x86-5.9.4.exe  
   https://gstreamer.freedesktop.org/pkg/windows/1.16.1/gstreamer-1.0-devel-mingw-x86-1.16.1.msi  
   https://gstreamer.freedesktop.org/pkg/windows/1.16.1/gstreamer-1.0-mingw-x86-1.16.1.msi  

* Required from version 3.0.0 to 3.0.4  
   https://download.qt.io/official_releases/qt/5.9/5.9.4/qt-opensource-windows-x86-5.9.4.exe  
   https://gstreamer.freedesktop.org/pkg/windows/1.14.4/gstreamer-1.0-x86_64-1.14.4.msi  
   https://gstreamer.freedesktop.org/pkg/windows/1.14.4/gstreamer-1.0-devel-x86_64-1.14.4.msi  

### Environment
* GST_PLUGIN_PATH=C:\gstreamer\1.0\x86\lib\gstreamer-1.0
* GSTREAMER_1_0_ROOT_X86=C:\gstreamer\1.0\x86\


## Linux
### Run-time requirements

* Qt5 >= 5.9.4
* pulseaudio
* libwayland-client0 (since 3.0.9)
* gstreamer >= 1.12.5
* gstreamer-plugins-base
  - Audio: vorbis, opus
* gstreamer-plugins-good
  - Format: mkv, avi, webm, mp4, mov
  - Video: vp8
  - Audio: flac
* gstreamer-plugins-bad
  - Video: camera
  - Audio: **AAC only supported in vokoscreenNG 3.0.5 - 3.0.8**
* gstreamer-plugins-bad-orig-addon (openSUSE Packman)
  - Video: openh264
* gstreamer-plugins-ugly
  - Video: x264
  - Audio: mp3
* gstreamer-plugins-ugly-orig-addon (openSUSE Packman)
  - Video: x264
* gstreamer-plugins-libav
  - Player: x264
* 

#### Wayland (since 3.1.0)
* pipewire >= 0.3
* gstreamer-plugin-pipewire(openSUSE) gstreamer1.0-pipewire(Debian) >= 0.3.24
* xdg-desktop-portal >= 1.7.0
* xdg-desktop-portal-(kde, gtk, ...) >= 1.7.0

#### VAAPI only supported in vokoscreenNG 3.0.0 - 3.0.8

* gstreamer-plugins-vaapi 
  - Intel Hardwareenconding H.264 and MPEG2
* intel-vaapi-driver 
  - Intel Hardwareenconding H.264 and MPEG2
* 


### Build requirements
* pkgconfig
* libqt5-devel
* libqt5-linguist
* libX11-devel
* gstreamer-devel
* libpulse-devel (since 3.0.7)
* wayland-devel (since 3.0.9)

On Ubuntu these requirements can be satisfied by running `sudo apt install libgstreamer1.0-dev qt5-default qttools5-dev-tools libqt5x11extras5-dev qtmultimedia5-dev libqt5multimedia5-plugins libpulse-dev`

### Build instructions
1. Download the source / clone the repo and `cd` into its directory
2. `cd src`
3. `qmake`

## Install packages

* [Debian](https://tracker.debian.org/pkg/vokoscreen-ng): `sudo apt install vokoscreen-ng`
* [Fedora](https://src.fedoraproject.org/rpms/vokoscreenNG): `sudo dnf install vokoscreenNG`
* [openSUSE](https://software.opensuse.org/package/vokoscreenNG) (Tumbleweed and Leap 15.2): `sudo zypper install vokoscreenNG`
* [Snap](https://snapcraft.io/vokoscreen-ng): `sudo snap install vokoscreen-ng`
* [Ubuntu](https://launchpad.net/ubuntu/+source/vokoscreen-ng): `sudo apt install vokoscreen-ng`
* [Arch linux](https://www.archlinux.org/packages/community/x86_64/vokoscreen/): `sudo pacman -S vokoscreen`
* [NixOS](https://github.com/NixOS/nixpkgs/blob/master/pkgs/applications/video/vokoscreen-ng/default.nix): `nix-env -iA nixos.vokoscreen-ng`

![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-0.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-1.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-2.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-3.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-4.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-5.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-6.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-7.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/screencast-8.png)
