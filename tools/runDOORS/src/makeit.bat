@echo off

    if exist "runDOORS.obj" del "runDOORS.obj"
    if exist "runDOORS.exe" del "runDOORS.exe"

    s:\masm32\bin\ml /c /coff "runDOORS.asm"
    if errorlevel 1 goto errasm

    s:\masm32\bin\PoLink /SUBSYSTEM:CONSOLE "runDOORS.obj"
    if errorlevel 1 goto errlink
    dir "runDOORS.*"
    copy runDOORS.exe ..
    del runDOORS.obj
    del runDOORS.exe    
    goto TheEnd

  :errlink
    echo _
    echo Link error
    goto TheEnd

  :errasm
    echo _
    echo Assembly Error
    goto TheEnd
    
  :TheEnd



pause
