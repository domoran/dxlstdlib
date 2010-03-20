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

SET MASMPATH=xxx
for /F "delims=;" %%i in ('cd') DO set CURPATH=%%i
pushd

FOR %%i IN ('A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO ( 
	IF EXIST "%%i:\masm32" (	
		set MASMPATH=%%i:\masm32
		%%i:
	)
)

if "%MASMPATH%"=="none" goto errMasm

echo PATH=%MASMPATH%
echo DRV =%MASMDRV%

    if exist "%CURPATH%\doorsOutput.obj" del "%CURPATH%\doorsOutput.obj"
    if exist "%CURPATH%\doorsOutput.exe" del "%CURPATH%\doorsOutput.exe"

    %MASMPATH%\bin\ml /c /coff /Fo"%CURPATH%\doorsOutput.obj" "%CURPATH%\doorsOutput.asm" 
    if errorlevel 1 goto errasm

    %MASMPATH%\bin\PoLink /SUBSYSTEM:CONSOLE "%CURPATH%\doorsOutput.obj" /OUT:"%CURPATH%\doorsOutput.exe"
    if errorlevel 1 goto errlink
    dir "%CURPATH%\doorsOutput.*"
    copy "%CURPATH%\doorsOutput.exe" "%CURPATH%\.."

    goto TheEnd

  :errlink
    echo _
    echo Link error
    goto TheEnd

  :errasm
    echo _
    echo Assembly Error
    goto TheEnd
    
  :errMasm
    echo _
    echo MASM could not be found
    goto TheEnd

  :TheEnd
popd
pause
