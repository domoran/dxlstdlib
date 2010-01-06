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

REGKEY macro base:req, key:req
    IFNDEF @@k_e_y_Handle@@
        .data?
            @@k_e_y_Handle@@ dd ?
        .code
    ENDIF

    .if rv(RegOpenKey, base, key, addr @@k_e_y_Handle@@) != ERROR_SUCCESS
       mov @@k_e_y_Handle@@,0
    .endif
    
    EXITM <@@k_e_y_Handle@@>

ENDM

SUBKEY macro base:req, key:req, index:req
    IFNDEF @@S_U_B_KEYname@@
       .data?
           @@S_U_B_KEYname@@ db 256 dup (?)
           @@S_U_B_newKey@@ db 256 dup (?)
       .code
    ENDIF
    
    mov esi, REGKEY (base, key)
    fn RegEnumKey, esi, index, addr @@S_U_B_KEYname@@, 256
    
    fn RegCloseKey, esi

    cst offset @@S_U_B_newKey@@, key
    mov eax, cat$(offset @@S_U_B_newKey@@, "\", offset @@S_U_B_KEYname@@)
    mov eax, REGKEY(base, addr @@S_U_B_newKey@@)
    
    EXITM <eax>
ENDM

REGVALUE macro key:req,val:req, size:req
    LOCAL keyVal
    
    IFNDEF @@v_a_l_type@@
        .data?
            @@v_a_l_type@@ dd ?
            @@s_t_r_size@@ dd ?
            keyVal db size dup (?)
        .code
    ENDIF
    mov @@v_a_l_type@@, REG_SZ    
    mov @@s_t_r_size@@, size
    
    .if rv(RegQueryValueEx, key, val, NULL, addr @@v_a_l_type@@, addr keyVal, addr @@s_t_r_size@@) != ERROR_SUCCESS
        mov DWORD PTR [keyVal], 0
        xor eax,eax
    .else
        mov eax, offset keyVal
    .endif
    
    EXITM <eax>
endm
