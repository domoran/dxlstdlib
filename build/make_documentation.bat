@echo off

REM    Copyright 2010 by Mathias Mamsch
REM    This file is part of the DOORS Standard Library 

REM    The DOORS Standard Library  is free software: you can redistribute it and/or modify
REM    it under the terms of the GNU General Public License as published by
REM    the Free Software Foundation, either version 3 of the License, or
REM    (at your option) any later version.

REM    The DOORS Standard Library  is distributed in the hope that it will be useful,
REM    but WITHOUT ANY WARRANTY; without even the implied warranty of
REM    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM    GNU General Public License for more details.

REM    You should have received a copy of the GNU General Public License
REM    along with the DOORS Standard Library.  If not, see <http://www.gnu.org/licenses/>.

..\tools\dxl_doxygen\DXL_doxygen.exe ..\doc\config\Doxyfile
if NOT "%ERRORLEVEL%" == "0" goto ErrorGen

FOR /F "delims=;" %%i in ('..\tools\getReg\getReg.exe HKCU "Software\Microsoft\HTML Help Workshop" InstallDir') DO SET HCCDIR=%%i
if "%HCCDIR%"=="NO_SUCH_KEY" goto notInstalled

ECHO -----------------------------
ECHO Generating compiled Help file
ECHO -----------------------------

if "%HCCDIR%"=="NO_SUCH_VALUE" goto installError

"%HCCDIR%\hhc.exe" ..\doc\html\index.hhp
if NOT "%ERRORLEVEL%"=="1" goto CHMERROR

if NOT "<%%1>" == "<release>" goto AllOK

start ../doc/DXLLIB.chm

goto AllOK

:notInstalled
start ../doc/HTML/index.html
goto AllOK

:installError
ECHO CHM File could not be created!
ECHO ------------------------------
ECHO HTML Workshop seems to be installed, but the installation path 
ECHO could not be determined. Maybe your HTML Workshop version is to 
ECHO new. Please report a bug with the HTML workshop version. 
ECHO -
pause

start ../doc/HTML/index.html
goto AllOK

:CHMERROR
ECHO -
ECHO The CHM help file for the dxl standard library could not be compiled. 
pause
start ../doc/HTML/index.html
goto AllOK

:ErrorGen
pause

:AllOK
