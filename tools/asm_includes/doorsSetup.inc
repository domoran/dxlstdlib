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

.data
      dotcomma db ";",0

.code

printColor MACRO text:req, color:req
    IFNDEF @@STDOUT_HANDLE@@
        .data? 
            @@STDOUT_HANDLE@@ dd ?
        .code        
    ENDIF
    
    mov @@STDOUT_HANDLE@@, rv(GetStdHandle, STD_OUTPUT_HANDLE)
    invoke SetConsoleTextAttribute, @@STDOUT_HANDLE@@, color
    print text
ENDM


LOGINFILE macro 
    IFNDEF @@LOGIN_FILENAME@@
        .data?
            @@LOGIN_FILENAME@@ dd ?
        .code
    ENDIF

    ; calculate the login file name on the first call
    mov @@LOGIN_FILENAME@@, alloc(512)
    push esi

    ; calculate data file name
    mov esi, env$("USERPROFILE")
    .if esi == 0         
        print "Error. Environment variable USERPROFILE was not found! Check your windows installation."
    .else
        cst @@LOGIN_FILENAME@@, esi 
        mov esi, cat$(@@LOGIN_FILENAME@@,  "\doorsLogin.dat")
    .endif         
    
    pop esi        

    EXITM <@@LOGIN_FILENAME@@>
ENDM

.code

readDOORSData proc refDB:DWORD, refUser:DWORD, refPasswd:DWORD
    LOCAL fHandle:DWORD
    LOCAL buffer: DWORD
    LOCAL fLen:DWORD
    LOCAL ofs:DWORD
    LOCAL pos:DWORD

    m2m fHandle, LOGINFILE()
    
    .if rv(exist,fHandle) == 0
        xor eax,eax
        ret 
    .endif

    mov buffer, alloc (1024)
    
    invoke read_disk_file, fHandle, addr buffer, addr fLen

    ; find the first semicolon
    mov ofs, rv(InString, 1, buffer, addr dotcomma)

    ; if it is not there -> readError
    .if ofs >= 080000000h || ofs == 0
        jmp readError
    .endif

    ; Calculate database part string length. If it is zero -> readError
    mov ecx, ofs
    dec ecx     
    test ecx, ecx
    jz readError
   
    ; Get the database part and copy it to refDB
    invoke szMid, buffer, refDB, 0, ecx

    ; save the last position
    m2m pos, ofs

    ; we start searching at the next index
    add ofs,1

    ; search for the second ;    
    mov ofs, rv(InString, ofs, buffer, addr dotcomma)

    ; if it is not there -> readError    
    .if ofs > 080000000h || ofs == 0
        jmp readError
    .endif

    ; calculate user part length. If it is zero -> readError
    mov ecx, ofs
    sub ecx, pos
    dec ecx
    test ecx, ecx
    jz readError

    ; get the user part
    invoke szMid, buffer, refUser, pos, ecx

    ; get the length of the buffer
    invoke szLen, buffer
    
    ; calculate the length of the password part        
    sub eax, ofs
    
    .if eax == 0
        ; if it is zero->make refPasswd an empty string
        mov byte ptr [refPasswd], 0       
    .else
        ; read the password part from the file
        invoke szMid, buffer, refPasswd, ofs, eax
    .endif
        
    free buffer
    mov eax, 1
    ret
readError:
    print "The DOORS setup file contains invalid data. It will be overwritten.",10,13
    xor eax, eax
    ret
readDOORSData endp



