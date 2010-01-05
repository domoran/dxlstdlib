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

    includelib \masm32\lib\advapi32.lib
    
    include ..\..\asm_includes\regMacros.inc
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data
      usage    db "getReg.exe will print out the value of an registry entry. It can be used for example"
               db " to find the path to a windows program, or check if a program is installed from the command line. It is used the following way:",10,10,13
               db "       getReg.exe <base> <key path> <value>",10,13
               db " ",10,13
               db "where:",10,13
               db "    <base>     is one of:",10,13
               db "                    HKCR -> the key will be searched in HKEY_CLASSES_ROOT",10,13
               db "                    HKCU -> the key will be searched in HKEY_CURRENT_USER",10,13
               db "                    HKLM -> the key will be searched in HKEY_LOCAL_MACHINE",10,13
               db "                    HKU  -> the key will be searched in HKEY_USERS",10,13
               db "                    HKCC -> the key will be searched in HKEY_CURRENT_CONFIG        ",10,13
               db "    <key path> is the path to the registry key    ",10,13
               db "    <value>    the name of the value to print ",10,13
               db "    ",10,13
               db "Example: ",10,13
               db "    getReg.exe HKCU SOFTWARE\Microsoft\Office\11.0\Word\InstallRoot Path",10,13
               db " ",10,13
               db "--> returns the path where microsoft Office 2003 is installed",10,13
     .code


start:
    call main
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    exit

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc
    LOCAL base: DWORD
    LOCAL key[1024]:DWORD
    LOCAL keyHandle:DWORD
    LOCAL value:DWORD

    invoke  getcl_ex, 1, addr key
    cmp eax, 1
    jnz errMain

    ; read the base key from the commandline and store it in 'base'
    mov base, 0
    
    .if rv(szCmp, addr key, "HKCR") != 0
        mov base, HKEY_CLASSES_ROOT
        ;print "HKEY_CLASSES_ROOT",10,13
    .endif    
    
    .if rv(szCmp, addr key, "HKCU") != 0
        mov base, HKEY_CURRENT_USER
        ;print "HKEY_CURRENT_USER",10,13
    .endif    

    .if rv(szCmp, addr key, "HKU") != 0
        mov base, HKEY_USERS
        ;print "HKEY_USERS",10,13
    .endif    

    .if rv(szCmp, addr key, "HKLM") != 0
        mov base, HKEY_LOCAL_MACHINE
        ;print "HKEY_LOCAL_MACHINE",10,13
    .endif    


    .if rv(szCmp, addr key, "HKCC") != 0
        mov base, HKEY_CURRENT_CONFIG
        ;print "HKEY_CURRENT_CONFIG",10,13
    .endif    

    cmp base, 0
    jz errMain

    invoke  getcl_ex, 2, addr key    
    cmp eax, 1
    jnz errMain

    m2m keyHandle, REGKEY(base, addr key)
    cmp keyHandle,0
    jz errKey

    invoke  getcl_ex, 3, addr key    
    cmp eax, 1
    jnz errMain

    mov value, REGVALUE(keyHandle, addr key, 1023)
    cmp value,0
    jz errVal

    print value,10,13

    jmp endMain

errMain:
    print addr usage   
    jmp endMain

errKey:
     print "NO_SUCH_KEY", 10,13
     jmp endMain

errVal:
    print "NO_SUCH_VALUE",10,13
    jmp endMain

endMain: 
    .if keyHandle != 0
        invoke RegCloseKey, keyHandle
    .endif
    
    ret
main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
