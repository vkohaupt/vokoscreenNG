
<table>
    <tr>
        <td align="middle" colspan="2">&nbsp;
          <img src="https://vokoscreen.volkoh.de/3.0/picture/logo-web-2.png">
        </td>
    </tr>
    <tr>
      <td>
          <img src="https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-0.png" width="500" height="300">
      </td>
      <td>
          <img src="https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-7.png" width="500" height="300">
      </td>
    </tr>
</table>
  
vokoscreenNG for Windows and Linux is a powerful screencast creator in many languages to record the screen, an area or a window (Linux only). Recording of audio from multiple sources is supported. With the built-in camera support, you can make your video more personal. Other tools such as systray, magnifying glass, countdown, timer, Showclick and Halo support will help you do a good job. 


## Download
* Windows https://linuxecke.volkoh.de/vokoscreen/vokoscreen-download.html

## Translate
Only **100% translated** languages will be added to vokoscreenNG.
You can translate to anytime, your translation will be included in the next bugfix release.
Please translate only at Transifex, it is very easy.
https://app.transifex.com/vkohaupt/vokoscreen/

## Installer
Windows and Linux installation routines are not provided, because each platform, installer and package manager has its own special features. That means make-install, metadata, desktop files, icons, manpage, logo must be create and provided itself. All the images and logos that come with vokoscreenNG are free. You can be change in size itself be used for your own installer or package manager.

## Code Contributions
If you want to contribute code, please ask beforehand whether your idea finds harmony. Your source code must be in separate files and published under the MIT-License.
Every file must include the License-header.
The License-header can be found here:https://github.com/vkohaupt/vokoscreenNG/blob/3.0/info-licences-changelog-install/Third-Party-License/Third-Party-License

## Windows 10 64-bit
### BuildRequires
- Required from version 3.1.0

  The Qt installer needs the 32 bit version of vc_redist.x86.exe  
  https://aka.ms/vs/16/release/vc_redist.x86.exe  
  
  Qt Online Installer https://www.qt.io/download-thank-you?os=windows
    - Qt 5.15.2  
      [+] MinGW 8.1.0 64-bit  
      [+] Qt Network Authorization (Not sure if this is needed)  
    - Developer and Designer Tools  
      [+] Qt Creator 5.0.1 CDB Debugger Support  
      [+] Debugging Tools for Windows  
      [+] MinGW 8.1.0 64-bit  
    - openSSL 1.1.1 Toolkit  
      [+] openssl 64-bit binaries  
      
  https://gstreamer.freedesktop.org/data/pkg/windows/1.18.5/mingw/gstreamer-1.0-mingw-x86_64-1.18.5.msi  
  https://gstreamer.freedesktop.org/data/pkg/windows/1.18.5/mingw/gstreamer-1.0-devel-mingw-x86_64-1.18.5.msi  

### Environment
* No additional entries 


## Linux
### Run-time requirements

* Qt5 >= 5.9.4
* Qt5 >= 5.14 (vokoscreenNG >= 3.1.0 pre alpha)
* pulseaudio
* libwayland-client0 (since vokoscreenNG 3.0.9)
* gstreamer >= 1.12.5
* gstreamer-plugins-base
  - Audio: vorbis, opus
* gstreamer-plugins-good
  - Format: mkv, avi, webm, mp4, mov
  - Video: vp8
  - Audio: flac
* gstreamer-plugins-bad
  - Video: camera
  - Video: openh264
* gstreamer-plugins-ugly
  - Video: x264
  - Audio: mp3
* gstreamer-plugins-ugly-orig-addon (openSUSE Packman)
  - Video: x264
* gstreamer-plugins-libav
  - Player: x264
* 

#### Wayland (vokoscreenNG >= 3.1.0)
* pipewire >= 0.3
* gstreamer-plugin-pipewire(openSUSE) gstreamer1.0-pipewire(Debian) >= 0.3.24
* xdg-desktop-portal >= 1.7.0
* xdg-desktop-portal-(kde, gtk, ...) >= 1.7.0

### Build requirements
* pkgconfig
* libqt5-devel >= 5.14 (vokoscreenNG >= 3.1.0 pre alpha)
* libqt5-linguist >= 5.14 (vokoscreenNG >= 3.1.0 pre alpha)
* libX11-devel
* gstreamer-devel
* libpulse-devel (vokoscreenNG >= 3.0.7)
* wayland-devel (vokoscreenNG >= 3.0.9)

On Ubuntu these requirements can be satisfied by running `sudo apt install libgstreamer1.0-dev qt5-default qttools5-dev-tools libqt5x11extras5-dev qtmultimedia5-dev libqt5multimedia5-plugins libpulse-dev libwayland-dev`

### Build instructions
1. `Download the source / clone the repo and cd into its directory`
2. `cd src`
3. `qmake`
4. `make`

## Install packages

* [Debian](https://tracker.debian.org/pkg/vokoscreen-ng): `sudo apt install vokoscreen-ng`
* [Fedora](https://src.fedoraproject.org/rpms/vokoscreenNG): `sudo dnf install vokoscreenNG`
* [openSUSE](https://software.opensuse.org/package/vokoscreenNG) (Tumbleweed and Leap): `sudo zypper install vokoscreenNG`
* [Ubuntu](https://launchpad.net/ubuntu/+source/vokoscreen-ng): `sudo apt install vokoscreen-ng`
* [Arch linux](https://www.archlinux.org/packages/community/x86_64/vokoscreen/): `sudo pacman -S vokoscreen`
* [NixOS](https://github.com/NixOS/nixpkgs/blob/master/pkgs/applications/video/vokoscreen-ng/default.nix): `nix-env -iA nixos.vokoscreen-ng`

![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-0.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-1.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-2.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-3.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-4.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-5.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-6.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-7.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-8.png)
![Picture](https://vokoscreen.volkoh.de/3.0/picture/vokoscreenNG-9.png)
