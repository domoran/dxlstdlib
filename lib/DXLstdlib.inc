#include <lib/general_functions/getCaller.inc>

Skip gsk_stdlib_globals_ = createString();
bool gsk_stdlib_error_ignore_ = false

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
        // -> cought error can be accessed until the next catch ...
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