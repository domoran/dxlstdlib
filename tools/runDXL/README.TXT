cmdDOORS contains three files: 

- the runScript.vbs which allows you to execute a DXL file in your current open DOORS session and return the output of the script. 
- the doorsOutput.exe tool which will read the output from the DXL editor window in DOORS and close the window (this tool is used by runScript.vbs)

These two tools allow a nice integration into your favorite code editor software. Just run your DXL file by executing: 

cscript.exe /nologo runScript.vbs <path> 

where <path> is the path to the DXL script you want to run. 

