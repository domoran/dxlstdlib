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

//-----------------------------------------------------------------------------
//                               
//  Function   : generateAutoRunDXLFolderString
//                               
//  Description: generates a string containing '#include's of all
//                 the files in the specified path (relative to doorsHome)
//                                                                        
//-----------------------------------------------------------------------------
string generateAutoRunDXLFolderString(string sFilesPath)
{
    string sEvalStr = ""
    string sIncStart = "#include <" sFilesPath "\\"
    string s
    string sLoc
    string sDXLCheck = null
    sLoc = sFilesPath
    
    Stat st = create(sLoc)
    if (!null st)
    {
        for s in directory sLoc do
        {
            int len = length s
            if len < 5 then continue
            
            if(s == "." || s == ".." || 
                 (s[len-4:len-1] != ".inc" && 
                  s[len-4:len-1] != ".dxl") ) {
                    //do nothing
            } else {
                sDXLCheck = null
                sDXLCheck = checkDXL(sIncStart s ">")
                if(null sDXLCheck)
                {
                    sEvalStr = sEvalStr sIncStart s ">\n"
                }
                else
                {
                    //didn't pasre properly, so don't include it
                    print sDXLCheck "\n"
                }
            }
        }
    } else print "No files in or unknown directory " sLoc "\n"
    
    
    return(sEvalStr)
}

//-----------------------------------------------------------------------------
//                               
//  Function   : autorun
//                               
//  Description: runs all the files in the specified directory as DXL.
//                 This is used for startup, finish, formal menu functions
//                 and project menu functions.
//  Notes:         eval_ runs the code in a separate dxl context so any 
//                 declarations in dxl files in the specified path 
//                 will not persist.
//  Parameters: sFilesPath: path to the directory containing dxl files. This
//                path must be relative to doorsHome
//-----------------------------------------------------------------------------
void autorun(string sFilesPath)
{
    eval_(generateAutoRunDXLFolderString(sFilesPath))
}