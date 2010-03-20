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

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      value dd ?
      buffer  db 128 dup(?)
      mhand dd ?
      childHand dd ?

    .data
      doors db "DOORSWindow",0      
      DXLWindowTitle db "DXL Interaction ",0
      DXLWindowTitleLen dd 16
      RichEditClass db "RichEdit20W", 0
            
    .code

start:

    call main
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤


    exit

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc
    LOCAL textLen:DWORD 
    LOCAL readBytes:DWORD 
    LOCAL textBuf:DWORD
    LOCAL flags:DWORD
    
    mov mhand, FUNC(FindWindowEx, NULL, NULL,addr doors, NULL);       we are looking for doors
    
    .while mhand != 0 ; got handle ?
        invoke GetWindowText, mhand, addr buffer, 127
        .if FUNC (cmpmem, addr buffer, addr DXLWindowTitle, DXLWindowTitleLen) != 0
        
            mov childHand, FUNC(FindWindowEx, mhand, NULL,addr RichEditClass, NULL);       we are looking for doors

            .while childHand != 0 
                mov flags, FUNC(SendMessage, childHand, EM_GETOPTIONS, 0,0) 
                
                .if (flags & ECO_READONLY) 
                    mov textLen, FUNC(SendMessage, childHand,  WM_GETTEXTLENGTH, 0 ,0)
                    add textLen, 2
                    
                    mov textBuf, alloc(textLen)
                    
                        mov readBytes, FUNC(SendMessage, childHand, WM_GETTEXT, textLen, textBuf)                        
                        print textBuf
                    
                    free textBuf
                    
                    invoke SendMessage, childHand, WM_SETTEXT, 0, NULL
                    invoke SendMessage, mhand, WM_CLOSE, 0,0
                    mov childHand, 0
                    mov mhand, 0
                .else

                    mov childHand, FUNC(FindWindowEx, mhand, childHand ,addr RichEditClass, NULL);       we are looking for doors
                .endif

            .ENDW
            
        .endif

     .if (mhand != 0) 
        mov mhand, FUNC(FindWindowEx, NULL, mhand ,addr doors, NULL);       we are looking for doors
     .endif
                
    .ENDW


    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
