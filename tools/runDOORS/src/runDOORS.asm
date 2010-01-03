comment * -----------------------------------------------------
    Copyright 2010 by Mathias Mamsch
    This file is part of the DOORS Standard Library 

    The DOORS Standard Library  is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DOORS Standard Library  is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the DOORS Standard Library.  If not, see <http://www.gnu.org/licenses/>.
----------------------------------------------------- *



; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
    include \masm32\include\masm32rt.inc
    include \masm32\include\advapi32.inc

    include ..\..\asm_includes\regMacros.inc
    include ..\..\asm_includes\doorsSetup.inc

    includelib \masm32\lib\advapi32.lib
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
        
    .data
      doorsKey db "SOFTWARE\Telelogic\DOORS",0   
      execDir db "ExecutablesDirectory",0
      hyphen db 34,0
      
      loginMessage1 db "------------------------------------------------------------------",10
                    db "                            runDOORS V.1.0                        ",10
                    db "------------------------------------------------------------------",10,10,0
                    
      loginMessage2 db "You need to specify the parameters for runDOORS.exe on the",10
                    db "first start. You will now be asked to enter the database, ",10
                    db "username and password used for starting Rational(R) DOORS(R).",10,10,0
                    
      loginMessage3 db "Your input will be stored UNENCRYPTED in the following file:",10,10,0
      
      loginMessage4 db "If your DOORS database is configured to use no passwords, then",10
                    db "just leave the password blank.",10,10,0 

      dataShort db "-d",0
      dataLong db "-data",0

      pwShort db "-P",0
      pwLong db "-password",0

      userShort db "-u",0
      userLong db "-user",0

    .code

start:

    call main
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤


    exit

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

printColor MACRO text:req, color:req
    IFNDEF @@STDOUT_HANDLE@@
        .data? 
            @@STDOUT_HANDLE@@ dd ?
        .code
        mov @@STDOUT_HANDLE@@, rv(GetStdHandle, STD_OUTPUT_HANDLE)
    ENDIF
    
    invoke SetConsoleTextAttribute, @@STDOUT_HANDLE@@, color
    print text
ENDM


getDOORSData proc
    LOCAL hConsole:DWORD
    LOCAL dataFile[256]: BYTE
    LOCAL uName:DWORD
    LOCAL database:DWORD
    LOCAL passwd:DWORD
    LOCAL fHandle:DWORD
    LOCAL fData:DWORD

    invoke ClearScreen

    printColor addr loginMessage1, 01Eh
    printColor addr loginMessage2, 08h    
    printColor addr loginMessage3, 0Eh

    ; calculate data file name
    mov esi, env$("USERPROFILE")
    .if esi == 0         
        print "Error. Environment variable USERPROFILE was not found! Check your windows installation."
        ret
    .endif 

    cst addr dataFile, esi 
    mov eax, cat$(addr dataFile,  "\doorsLogin.dat")

    ; output data file name
    print "    "
    print addr dataFile,10,10,13

    printColor addr loginMessage4,08h

    mov fData, alloc(1024) 
    mov byte ptr [fData],0

    mov esi, input ("Enter your Database (e.g. 36677@localhost): ")
    mov esi, cat$(fData, esi, addr dotcomma )

    mov esi, input("Enter your username (e.g. Administrator): ")    
    mov esi, cat$(fData, esi, addr dotcomma )

    mov esi, input("Enter your password (leave blank if none): ")
    mov esi, cat$(fData, esi )
    
    mov esi, OutputFile(addr dataFile, fData, len(fData))
    free fData

    ret
getDOORSData endp

CHECK_SUCCESS macro msg:req
    .if (eax != ERROR_SUCCESS)
        print msg
        jmp endMain
    .endif

endm




main proc
    LOCAL regKey:DWORD
    LOCAL keyHandle:DWORD
    LOCAL strVar[256]:BYTE       
    LOCAL keyName[256]:BYTE
    
    LOCAL user[256]:BYTE
    LOCAL database[512]:BYTE
    LOCAL password[512]:BYTE

    LOCAL strPtr:DWORD
    LOCAL strSize:DWORD
    LOCAL valType:DWORD
    LOCAL argError: DWORD
    LOCAL argCount: DWORD
    LOCAL newCmdLine: DWORD
    LOCAL hConsole: DWORD
    LOCAL hasPassword: DWORD
    LOCAL hasUser: DWORD
    LOCAL hasData: DWORD
    LOCAL dataFound: DWORD


    mov hasPassword, 0
    mov hasUser, 0
    mov hasData, 0
    mov dataFound, 0


    mov strPtr, ptr$(strVar)

    invoke RegOpenKey, HKEY_LOCAL_MACHINE, addr doorsKey, addr keyHandle

    .if (eax != ERROR_SUCCESS) 
        print "DOORS was not found!"
        jmp endMain    
    .endif

    invoke RegEnumKey, keyHandle, 0, addr keyName, 255
    CHECK_SUCCESS "Could not determine installed DOORS versions."   
    
    mov eax, cat$(strPtr, addr doorsKey, "\", addr keyName)

    invoke RegOpenKey, HKEY_LOCAL_MACHINE, strPtr, addr keyHandle
    CHECK_SUCCESS "Unkown error when opening DOORS registry key "

    mov strSize, 255
    mov valType, REG_SZ    
    invoke RegQueryValueEx, keyHandle, addr execDir, NULL, addr valType, strPtr, addr strSize
    CHECK_SUCCESS "Could not get executables directory value."
    
    mov ecx, cat$(strPtr, "\doors.exe")

    .if rv(exist,strPtr) == 0
        print "DOORS.exe could not be found at "
        print strPtr, 10,13        
        jmp endMain
    .endif
    
    ; scan the command line for configured parameters
    mov argCount, 1

    .while 1
        mov argError, rv(getcl_ex , argCount, addr keyName)
        
        .if argError != 1  
            .break      
        .endif

        .if hasPassword == 0
            mov hasPassword, rv(szCmp, addr keyName, addr pwShort) 
            or hasPassword, rv(szCmp, addr keyName, addr pwLong) 
        .endif
        
        .if hasUser == 0
            mov hasUser, rv(szCmp, addr keyName, addr userLong)
            or hasUser,  rv(szCmp, addr keyName, addr userShort) 
        .endif

        .if hasData == 0
            mov hasData, rv(szCmp, addr keyName, addr dataShort) 
            or hasData, rv(szCmp, addr keyName, addr dataLong)
        .endif

        inc argCount
        
    .endw
    ; read config data
    mov dataFound, rv(readDOORSData, addr database, addr user, addr password)

    ; start building the DOORS command line
    mov newCmdLine, alloc(1024)
    cst newCmdLine, addr hyphen
    mov eax, cat$(newCmdLine, strPtr, addr hyphen)
    
    .if dataFound        
        .if hasUser == 0 
            mov eax, cat$(newCmdLine, " -u ", addr hyphen, addr user, addr hyphen)                
        .endif 

        .if hasPassword == 0 
            .if rv(szLen, addr password) > 0
                mov eax, cat$(newCmdLine, " -p ", addr hyphen, addr password, addr hyphen)                
            .endif 
        .endif 

        .if hasData == 0 
            mov eax, cat$(newCmdLine, " -d ", addr hyphen, addr database, addr hyphen)                
        .endif 

    .endif    

    ; append the rest of the commandline
    mov argCount, 1
    
    .while 1
        mov argError, rv(getcl_ex , argCount, addr keyName)
        
        .if argError != 1  
            .break      
        .endif

        mov eax, cat$(newCmdLine, " ", addr hyphen, addr keyName, addr hyphen)

        inc argCount
        
    .endw

    invoke WinExec, newCmdLine, SW_SHOW	

    free newCmdLine

endMain: 
   
    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
