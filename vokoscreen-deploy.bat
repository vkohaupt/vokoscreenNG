rem Install qt-opensource-windows-x86-5.9.2.exe
rem Install gstreamer-1.0-x86-1.12.4.msi

rem Create a folder
rem Copy vokoscreen.exe in this folder
rem Copy this file in this folder

rem Find gcc in PATH
PATH=%PATH%;C:\Qt\Qt5.9.2\Tools\mingw530_32\bin

C:\Qt\Qt5.9.2\5.9.2\mingw53_32\bin\windeployqt.exe --release vokoscreen.exe

copy "C:\gstreamer\1.0\x86\bin\libgobject-2.0-0.dll"
copy "C:\gstreamer\1.0\x86\bin\libglib-2.0-0.dll"
copy "C:\gstreamer\1.0\x86\bin\libgstreamer-1.0-0.dll"
copy "C:\gstreamer\1.0\x86\bin\libgmodule-2.0-0.dll"
copy "C:\gstreamer\1.0\x86\bin\libffi-7.dll"
copy "C:\gstreamer\1.0\x86\bin\libintl-8.dll"
