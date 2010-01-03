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
    includelib \masm32\lib\advapi32.lib

    include ..\..\asm_includes\doorsSetup.inc    
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      username db 128 dup (?)
      database db 512 dup (?)
      password db 128 dup (?)      
        
    .data
      hyphen db 34,0
      
      loginMessage1 db "------------------------------------------------------------------",10
                    db "                           setupDOORS V.1.0                       ",10
                    db "------------------------------------------------------------------",10,10,0
                    
      loginMessage2 db "setupDOORS.exe will allow you to set the database configuration",10
                    db "for the DXL Standard Library. Once set, you can use runDOORS.exe",10
                    db "to start DOORS using the supplied credentials.",10,10,0
                    
      loginMessage3 db "Your input will be stored UNENCRYPTED in the following file:",10,10,0
      
      loginMessage4 db "If your DOORS database is configured to use no passwords, then",10
                    db "just leave the password blank.",10,10,0 

      deleteMessage db "setupDOORS detected that you already supplied a DOORS setup:",10,10,0

    .code


getDOORSData proc
    LOCAL hConsole:DWORD
    LOCAL dataFile:DWORD
    LOCAL fHandle:DWORD
    LOCAL fData:DWORD

    invoke ClearScreen

    m2m dataFile, LOGINFILE()
    
    printColor addr loginMessage1, 01Eh
    printColor addr loginMessage2, 08h    
    printColor addr loginMessage3, 0Eh

    ; output data file name
    print "    "
    print dataFile,10,10,13

    printColor addr loginMessage4,08h

    mov fData, alloc(1024) 
    mov byte ptr [fData],0

    mov esi, input ("Enter your Database (e.g. 36677@localhost): ")
    mov esi, cat$(fData, esi, addr dotcomma )

    mov esi, input("Enter your username (e.g. Administrator): ")    
    mov esi, cat$(fData, esi, addr dotcomma )

    mov esi, input("Enter your password (leave blank if none): ")
    mov esi, cat$(fData, esi )
    
    mov esi, OutputFile(dataFile, fData, len(fData))
    free fData

    ret
getDOORSData endp


start:

    call main
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤


    exit

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤


deleteDOORSData proc
    invoke ClearScreen

    printColor addr loginMessage1, 01Eh
    printColor addr loginMessage2, 08h        
    
    printColor addr deleteMessage, 0Eh        

    print "in file: "
    printColor LOGINFILE(),0Fh
    print " ",10,10,13

    printColor "Database: ",0Fh
    printColor addr database,08h
    print " ",10
    
    printColor "Username: ",0Fh
    printColor addr username,08h
    print " ",10
    
    printColor "Password: ",0Fh
    printColor "xxxxxxxx",08h
    print " ",10,10,10
    
    printColor "Do you want to delete this data from your computer?",0Eh
    print " ",10
    
    print "(Press 'y' for YES, any other key to quit)",10,10
    printColor " ", 08h
    call wait_key
    
    .if al == 121 || al == 89        
        print "Deleting file ..."
        mov esi, LOGINFILE()
        invoke DeleteFile, esi
        .if eax == 0
            print "FAIL. Is the file still opened by another process?",10,13
        .else
            print "OK",10,13
        .endif
    .else
        print "Quitting."
    .endif 
    
    ret
deleteDOORSData endp

main proc
    LOCAL result:DWORD
    ; make sure that the first function where login file is used is on the top,
    ; because the macro will expand at the first point it is used
    mov result, rv(readDOORSData, addr database, addr username, addr password)
    
    .if result == 0
        call getDOORSData 

    .else
        call deleteDOORSData        
        inkey
    .endif

endMain: 
   
    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
