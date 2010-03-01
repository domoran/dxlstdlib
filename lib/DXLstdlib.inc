#include <lib/general_functions/getCaller.inc>

//! \brief RuntimeError is an internal class for generating runtime errors in the DXL standard library. 
struct RuntimeError {};

DxlObject DxlObjectOf (RuntimeError err) { return (addr_ err) DxlObject }

Skip gsk_stdlib_globals_ = createString();

RuntimeError raiseError (string code, string msg) {     
    string s = getCaller 2
    print "Halting program because of an error in\nFile=" s "\nCode=" code "\nMessage:" msg "\n"
    halt

    return null RuntimeError
}