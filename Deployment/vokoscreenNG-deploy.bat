rem Install qt-opensource-windows-x86-5.9.4.exe
rem Install gstreamer-1.0-x86-1.14.4.msi
rem Install gstreamer-1.0-devel-x86-1.14.4.msi
rem Create a folder
rem Copy vokoscreenNG.exe in this folder
rem Copy this file in this folder
rem Start this file

rem too find gcc
PATH=%PATH%;C:\Qt\Qt5.9.4\Tools\mingw530_32\bin

C:\Qt\Qt5.9.4\5.9.4\mingw53_32\bin\windeployqt.exe --release vokoscreenNG.exe

copy C:\gstreamer\1.0\x86\bin\libgobject-2.0-0.dll
copy C:\gstreamer\1.0\x86\bin\libglib-2.0-0.dll
copy C:\gstreamer\1.0\x86\bin\libgstreamer-1.0-0.dll
copy C:\gstreamer\1.0\x86\bin\libwinpthread-1.dll
copy C:\gstreamer\1.0\x86\bin\libffi-7.dll
copy C:\gstreamer\1.0\x86\bin\libintl-8.dll
copy C:\gstreamer\1.0\x86\bin\libgmodule-2.0-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstwinscreencap.dll
copy C:\gstreamer\1.0\x86\bin\libgstbase-1.0-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstcoreelements.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstvideoconvert.dll
copy C:\gstreamer\1.0\x86\bin\libgstvideo-1.0-0.dll
copy C:\gstreamer\1.0\x86\bin\liborc-0.4-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstvideorate.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstx264.dll
copy C:\gstreamer\1.0\x86\bin\libgstpbutils-1.0-0.dll
copy C:\gstreamer\1.0\x86\bin\libx264-148.dll
copy C:\gstreamer\1.0\x86\bin\libgsttag-1.0-0.dll
copy C:\gstreamer\1.0\x86\bin\libgstaudio-1.0-0.dll
copy C:\gstreamer\1.0\x86\bin\libz.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstmatroska.dll
copy C:\gstreamer\1.0\x86\bin\libbz2.dll
copy C:\gstreamer\1.0\x86\bin\libgstriff-1.0-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstdirectsoundsrc.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstaudioconvert.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstaudiorate.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstvorbis.dll
copy C:\gstreamer\1.0\x86\bin\libvorbisenc-2.dll
copy C:\gstreamer\1.0\x86\bin\libvorbis-0.dll
copy C:\gstreamer\1.0\x86\bin\libogg-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstvpx.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstlame.dll
copy C:\gstreamer\1.0\x86\bin\libmp3lame-0.dll
copy C:\gstreamer\1.0\x86\bin\libopus-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstopus.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstopusparse.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstavi.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstisomp4.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstflac.dll
copy C:\gstreamer\1.0\x86\bin\libFLAC-8.dll
copy C:\gstreamer\1.0\x86\bin\libgstrtp-1.0-0.dll
copy C:\gstreamer\1.0\x86\lib\gstreamer-1.0\libgstvideoscale.dll
