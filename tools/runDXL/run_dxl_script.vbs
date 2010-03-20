
if wscript.Arguments.Count = 0 then 
	wscript.echo "You need to run this script with the DXL file to execute as a parameter." & vbLf & "Usage: run_dxl_script.vbs <file>"
	wscript.quit
end if

set fs = CreateObject("Scripting.FileSystemObject")

par0 = replace(wscript.Arguments(0),"\", "/")
scriptdir = replace(replace(lcase(wscript.scriptfullname), "run_dxl_script.vbs",""),"\", "/")
objectscript = scriptdir + "find_current_module.inc"

befehl = "#include <" + objectscript + ">"
commandcheck = "oleSetResult(checkDXL(""#include <" + par0 + ">""))"
set d = Nothing

on error resume next
set d = GetObject(, "Doors.Application")
on error goto 0

if d is Nothing then 
	WScript.echo "You need to start DOORS using the runDOORS.bat script, before using the editor intgration!"
	wscript.quit
end if

d.runStr commandcheck

if d.result = "" then 
	d.runstr befehl + vbLF + "#include <" + par0 + ">"
    set objShell = wscript.createObject("wscript.shell")
    set oExec = objShell.Exec ("%comspec% /C """ + scriptdir + "doorsOutput.exe" + """") 
    Do While oExec.Status = 0
        WScript.Sleep 50
    Loop
    wscript.echo oExec.StdOut.ReadAll()    
else
	wscript.echo d.result
end if