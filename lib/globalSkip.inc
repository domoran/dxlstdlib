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

string gGlobalSkipName_ () { return "dxlstdlib_globalSkip" }

void addEscaped (Buffer b, Buffer code) {
	int i 
	for i in 0:(length code)-1 do {
			if (code[i] == '\\' || code[i] == '"') b += '\\'
			b+= code[i]
	}
}

void addEscaped (Buffer b, string code) {
	Buffer buf = create(); buf += code; addEscaped(b, buf); delete buf
}

void evalTopNow_ (Buffer code, bool bEscape, bool bShield) {
		OleAutoObj oleDOORS = oleCreateAutoObject "DOORS.Application"
		if (null oleDOORS) error "Cannot create global objects und OLE. Check your DOORS installation!"		
		// Make code, shield it from global namespace if necessary by adding { ... } around the user code 
		Buffer b = create() 
		b += "string evalCode = \""
		if bShield then b+= "{"
		if (bEscape) addEscaped (b, code) else b += code
		if bShield then b+= "}"
		b+= "\"\nevalTop_ evalCode"
		
		// print "Running Code:" (tempStringOf b) "\n"
		OleAutoArgs args = create(); put (args, tempStringOf b); 
		oleMethod(oleDOORS, "runStr", args); 
		delete b; delete args;  
		oleCloseAutoObject oleDOORS; oleDOORS = null; args = null
}

void evalTopNow_ (Buffer code) { evalTopNow_ (code, true, true) }

void evalTopNow_ (string code, bool bEscape) { Buffer b=create();b+=code;evalTopNow_ (b, bEscape, true); delete b; }
void evalTopNow_ (string code) { Buffer b=create();b+=code;evalTopNow_ (b, true, true); delete b }

void initializeGlobalSkip_ () {
	noError(); string x = eval_ (gGlobalSkipName_); string err = lastError()
	if (!null err) {
		Buffer b = create(); b +=  "Skip " gGlobalSkipName_ " = createString()"
		evalTopNow_ (b, false, false)
		delete b
	}
}

initializeGlobalSkip_() 

void addGlobalStringSkip (string sKey, bool bString) {
	Buffer code = create() 
	code += "Skip sk = create" (bString ? "String" : "") "(); put (" 
	code += gGlobalSkipName_ 
	code += ", \"" 
	addEscaped (code, sKey) 
	code += "\", sk)"
	evalTopNow_ (code, true, true)
	delete code 		 
} 

void addGlobalFindSkipCode_ (Buffer code, string sSkipKey) {
	 code +=  "Skip sk = null; find (" 
	 code += gGlobalSkipName_ 
	 code += ", \"" 
	 addEscaped(code, sSkipKey)
	 code += "\", sk);\n" 
}

void addPutGlobalStringCode_ (Buffer code, string sKey, string sVal) {
	 code += "put (sk, \"" 
	 addEscaped(code, sKey)
	 code += "\", \"" 
	 addEscaped(code, sVal)
	 code += "\")\n" 
}

void copyToGlobalSkip (string sSkipKey, Skip sk) {
	Buffer bufCode = create()
	string sVal
	
	addGlobalFindSkipCode_ (bufCode, sSkipKey)
	
	for sVal in sk do {
		string sKey = (key sk) string
		addPutGlobalStringCode_ (bufCode, sKey, sVal)  
	} 
	
	evalTopNow_ ( bufCode, true, true) 
	delete bufCode
} 

Skip getGlobalSkip (string sSkipKey) {
	Buffer code = create(); 
	addGlobalFindSkipCode_ (code, sSkipKey)
	code += "return_ ((addr_ sk) int) \"\" "
	// print "Checking for Skip Code:" (tempStringOf code) "\n"
	noError(); string sAddr = eval_ tempStringOf code; string msg = lastError(); 
	if (!null msg) { print "Error:" msg "\n"; return null Skip } 
	int ad = intOf realOf sAddr 
	if (ad == 0) { print "Null address returned!\n"; return null Skip }
	return ((addr_ ad) Skip) 
}

void deleteGlobalSkip(string sSkipKey) {
	Buffer code = create(); 
	addGlobalFindSkipCode_ (code, sSkipKey)
	code += "if (!null sk) delete sk\n"
	code += "delete (" 
	code += gGlobalSkipName_()
	code += ",\"" 
	addEscaped(code, sSkipKey) 
	code += "\")"
	evalTopNow_ code
	delete code
} 

