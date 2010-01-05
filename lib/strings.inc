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
/*! \example trimWhitespace.dxl 
    This is an example on how to use the trimWhitespace function. */
