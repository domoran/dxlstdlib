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