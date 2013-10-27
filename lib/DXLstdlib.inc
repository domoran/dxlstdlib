/*
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
*/

bool gsk_stdlib_error_ignore_ = false

string getCaller (int nr) {
    string s = dxlHere(); 
    int len = length s    
    int posStart = 0; int posEnd = 0; int count = 0; 
    // find the nr-th newline character in s
    while (posEnd < len && (s[posEnd] != '\n' || ++count < nr)) posEnd++;
    // store the start point
    posStart = posEnd; posEnd++
    // find the next newline, store the endPoint
    while (posEnd < len && s[posEnd] != '\n') posEnd ++
    // Skip the newline at the end of the line
    if (posStart < len-2) posStart++
    // Skip the newline at the start of the line
    if (posEnd   > 0    ) posEnd--
    return s[posStart:posEnd]
}

//! \brief RuntimeError is an internal class for generating runtime errors in the DXL standard library. 
struct RuntimeError {};

RuntimeError gsk_stdlib_currentError_ = null RuntimeError 
RuntimeError gsk_stdlib_catchError_   = null RuntimeError 

DxlObject DxlObjectOf (RuntimeError err) { return (addr_ err) DxlObject }

string errorCode    (RuntimeError err) { if (null err) return ""; DxlObject x = DxlObjectOf err; return (x->"code") string }
string errorMessage (RuntimeError err) { if (null err) return ""; DxlObject x = DxlObjectOf err; return (x->"msg" ) string }
string errorPosition(RuntimeError err) { if (null err) return ""; DxlObject x = DxlObjectOf err; return (x->"pos" ) string }

string errorCode    () { return errorCode     gsk_stdlib_catchError_ }
string errorMessage () { return errorMessage  gsk_stdlib_catchError_ }
string errorPosition() { return errorPosition gsk_stdlib_catchError_ }

void try() { gsk_stdlib_error_ignore_ = true }

bool catch(RuntimeError &e) { 
    bool result = !null gsk_stdlib_currentError_
    // do we have a current error ?
    if (result) {
        // move it to catch_error and clear the current error 
        // -> current error is only valid during catch ...
        e = gsk_stdlib_currentError_    
        gsk_stdlib_catchError_   = gsk_stdlib_currentError_    
        gsk_stdlib_currentError_ = null
    } else {
        // There is no current error ... reset the catch error ...
        // -> caught error can be accessed until the next catch ...
        gsk_stdlib_catchError_ = null
    }
    
    // after a catch we are not ignoring errors anymore ...
    gsk_stdlib_error_ignore_ = false;     
    
    return result
}

bool catch() { RuntimeError e; return catch(e) }

RuntimeError raiseError (string code, string msg) {     
    string s = getCaller 2

    if (!gsk_stdlib_error_ignore_) {
        print "Halting program because of an error in\nFile=" s "\nCode=" code "\nMessage:" msg "\n"        
        halt
    } else {
        DxlObject x = new() 
        x->"msg"  = msg; x->"code" = code; x->"pos"  = s
        gsk_stdlib_currentError_ = (addr_ x) RuntimeError
    }

    return gsk_stdlib_currentError_
}