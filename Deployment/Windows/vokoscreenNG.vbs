currentdir=Left(WScript.ScriptFullName,InStrRev(WScript.ScriptFullName,"\"))
Set WshShell = CreateObject("WScript.Shell")
SET env = WshShell.Environment("Process")
env("GSTREAMER_1_0_ROOT_X86") = currentdir
env("GST_PLUGIN_PATH") = currentdir
WshShell.Exec("vokoscreenNG.exe")
Set WshShell = Nothing
