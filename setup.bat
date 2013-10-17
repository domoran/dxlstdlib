REM Locate the DXL Standard Library Root Directory
FOR /F "delims=; tokens=*" %%I in ("%0") DO pushd "%%~dpI" 
:searchRoot 
if exist LICENSE.txt (set DXLSTDLIBDIR=%CD%) else (cd .. & goto :searchRoot)
popd

"%DXLSTDLIBDIR%\tools\configureDOORS\configureDOORS.exe"
pause