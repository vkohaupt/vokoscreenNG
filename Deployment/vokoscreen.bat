cd /d %~d0%~p0

set currentdir=%cd%
set GSTREAMER_1_0_ROOT_X86=%currentdir%
set GST_PLUGIN_PATH=%currentdir%
echo %Path%

vokoscreen.exe


rem set GSTREAMER_1_0_ROOT_X86=C:\gstreamer\1.0\x86\
rem set GST_PLUGIN_PATH=C:\gstreamer\1.0\x86\lib\gstreamer-1.0
rem set Path=%Path%;C:\gstreamer\1.0\x86\bin
