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


int gErrorCount = 0

void reportTestError (string fn, int nr, string is, string should) {
    print "Error in test nr. " nr " for function '" fn "'.\nResult: |" is "|\nShould be: |" should "|\n"    
    gErrorCount++
}

void test (string fn, int nr, string is, string should) {
    if (is != should) reportTestError(fn, nr, is, should) 
}

void reportErrors() {
    if (gErrorCount == 0) print "OK\n"
    if (gErrorCount == 1) print "1 Error\n"
    if (gErrorCount > 1) print gErrorCount " Errors\n"
}

