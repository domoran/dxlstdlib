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

REM --------- create documentation using the modified doxygen ---------------------------
..\tools\dxl_doxygen\DXL_doxygen.exe ..\doc\config\Doxyfile
if NOT "%ERRORLEVEL%" == "0" goto ErrorGen

REM --------- Check for HTML Workshop installation ---------------------------
FOR /F "delims=;" %%i in ('..\tools\getReg\getReg.exe HKCU "Software\Microsoft\HTML Help Workshop" InstallDir') DO SET HCCDIR=%%i
if "%HCCDIR%"=="NO_SUCH_KEY" goto HCCnotInstalled

echo.
echo -----------------------------
echo Generating compiled Help file
echo -----------------------------

REM --------- Check if we have found a directory for HCC (future versions might change the key) ---------------------------
if "%HCCDIR%"=="NO_SUCH_VALUE" goto HCCinstallError


REM --------- generate the CHM help file ---------------------------
"%HCCDIR%\hhc.exe" ..\doc\html\index.hhp
if NOT "%ERRORLEVEL%"=="1" goto CHMERROR


REM --------- if we are not doing a release then end the script here ---------------------------
REM --------- otherwise show the CHM file and end then               ---------------------------
if "<%1>" == "<release>" goto AllOK
start ../doc/DXLLIB.chm
goto AllOK


REM --------- Help workshop is not installed. If we are making a release this is not acceptable ---------------------------
REM --------- otherwise it is okay, we start the generated HTML help                            ---------------------------
:HCCnotInstalled
if "<%1>" == "<release>" goto releaseErr
start ../doc/HTML/index.html
goto AllOK


REM --------- Help workshop is installed, but we did not find the directory. If we make a realese -------------------------
REM --------- we need to exit the release script here. Otherwise start the HTML help              -------------------------
:HCCinstallError
echo.
echo CHM File could not be created!
echo ------------------------------
echo HTML Workshop seems to be installed, but the installation path 
echo could not be determined. Maybe your HTML Workshop version is too
echo new. Please report a bug with the HTML workshop version. 
echo.

if "<%1>" == "<release>" goto releaseErr

pause
start ../doc/HTML/index.html
goto AllOK

REM --------- There was an error during CHM compilation. For a release this is not acceptable.   -------------------------
REM --------- Otherwise we will tell the user and start the HTML help.                           -------------------------
:CHMERROR
echo.
echo The CHM help file for the dxl standard library could not be compiled. 
echo.

if "<%1>" == "<release>" goto releaseErr

pause
start ../doc/HTML/index.html
goto AllOK

:releaseErr
echo.
echo To make a release you MUST have Microsoft HTML Workshop installed!
echo Install it and try again
echo.
pause
EXIT  

:ErrorGen
pause

:AllOK
