@echo off
del ..\doc\DXLLIB.chm
if NOT "%ERRORLEVEL%" == "0" goto ErrorGen XXX ELSE echo %ERRORLEVEL%

..\tools\dxl_doxygen\DXL_doxygen.exe ..\doc\config\Doxyfile
if NOT "%ERRORLEVEL%" == "0" goto ErrorGen XXX ELSE echo %ERRORLEVEL%

start ../doc/HTML/index.html
start ../doc/DXLLIB.chm

goto AllesOK

:ErrorGen

pause

:AllesOK
