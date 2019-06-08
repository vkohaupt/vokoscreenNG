cd /d %~d0%~p0

set currentdir=%cd%
set GSTREAMER_1_0_ROOT_X86=%currentdir%
set GST_PLUGIN_PATH=%currentdir%
rem echo %Path%

rem Mit einem vorangestelltem "start" wird keine Konsole geöffnet
start vokoscreenNG.exe
