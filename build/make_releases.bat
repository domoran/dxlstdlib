REM @echo off

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

pushd
cd ..
for /F "delims=;" %%i in ('cd') do set DXLSTDLIBDIR=%%i
popd

cd "%DXLSTDLIBDIR%\build" && call make_documentation.bat release
call make_clean.bat

cd "%DXLSTDLIBDIR%"

rd releases /S /Q
mkdir releases

XCOPY  /I /Y *.* releases\user /EXCLUDE:build\exclude\make_user_exclude.txt
for /F %%i in ('dir /B /A:D') do XCOPY /E /I /Y %%i releases\user\%%i /EXCLUDE:build\exclude\make_user_exclude.txt
rmdir releases\user\.svn

XCOPY  /I /Y *.* releases\developer /EXCLUDE:build\exclude\make_developer_exclude.txt
for /F %%i in ('dir /B /A:D') do XCOPY /E /I /Y %%i releases\developer\%%i /EXCLUDE:build\exclude\make_developer_exclude.txt
rmdir releases\developer\.svn

for /F %%i in ('date /T') do set dt=%%i
 
cd "%DXLSTDLIBDIR%\releases\developer" && "%DXLSTDLIBDIR%\tools\zip\zip.exe" -r "%DXLSTDLIBDIR%\releases\developer_%dt:.=_%.zip" .
cd "%DXLSTDLIBDIR%\releases\user" && "%DXLSTDLIBDIR%\tools\zip\zip.exe" -r "%DXLSTDLIBDIR%\releases\user_%dt:.=_%.zip" .