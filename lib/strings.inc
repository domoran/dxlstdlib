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

/*! \file strings.inc 
    \brief Basic string manipulation functions
 
This file will contain functions for string manipulation, like splitting, joining, sorting and many more. Check 
in later to see the progress.
*/


/*! \return the contents of s where leading whitespaces (space,\\n,\\t,\\r) are removed from the beginning and the end
    \param s the string where leading and trailing whitespaces shall be removed
    \brief efficient removal of whitespaces at beginning and end
    This function is designed so that no unnecessary intermediate strings are created. That is why it runs on 'char' level.
*/  
string trimWhitespace(string s) {

   int first = 0
   int last = ( length s ) - 1
   while ( last > 0 && isspace( s[ last ] ) ) last--
   while ( isspace( s[ first ] ) && first < last ) first++
   if ( first == last && isspace s[first] ) return ""
   return s[ first:last ]
}

/*! \example example_trimWhitespace.dxl 
    This is an example on how to use the trimWhitespace(string) function. */

/*! \return Returns sSource, where the searchstring \em sSearch is replaced by sReplace 
    \param sSource the source string in which \em sSearch shall be replaced
    \param sSearch the search string that shall be replaced
    \param sReplace the string by which \em sSearch shall be replaced
    \brief Replaces a part of the string by another string
*/

Buffer gbuf_replace_sb1 = create(10000);
Buffer gbuf_replace_sb2 = create(10000);  
string replace (string sSource, string sSearch, string sReplace) {
    int iLen = length sSource
    if (iLen == 0) return ""
    
    int iLenSearch = length(sSearch)
    
    if (iLenSearch == 0) { 
        raiseError ("Parameter error", "in strings.inc/replace: search string must not be empty")
        return "" 
    }
    
    // read the first char for latter comparison -> speed optimization
    char firstChar = sSearch[0]
    
    Buffer s = create() 
    int pos = 0, d1,d2;    
    int i
    
    gbuf_replace_sb1 = sSource; 
    gbuf_replace_sb2 = sSearch;
    
    while (pos < iLen) { 
        char ch = gbuf_replace_sb2[pos]; 
        bool found = true
        
        if (ch != firstChar) {pos ++; s+= ch; continue}
        for (i = 1; i < iLenSearch; i++) 
           if (gbuf_replace_sb1[pos+i] != gbuf_replace_sb2[i]) { found = false; break }
        if (!found) {pos++; s+= ch; continue}
        s += sReplace
        pos += iLenSearch
    }
    
    string result = stringOf s
    delete s
    return result
}