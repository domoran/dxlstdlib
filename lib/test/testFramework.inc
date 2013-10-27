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

void reportTestError_ (string fn, string should, string is, int offset) {    
    print "Error in " (getCaller (3+offset) ) " for function '" fn "'.\nResult: |" is "|\nShould be: |" should "|\n"    
    gErrorCount++
}

void test (string fn, string should, string is, int callerOffset) { if (is != should) reportTestError_(fn, should, is, callerOffset) }

void test (string fn, string should, string is) { test (fn, should, is, 2) }

void reportErrors() {
    if (gErrorCount == 0) print "OK\n"
    if (gErrorCount == 1) print "1 Error\n"
    if (gErrorCount > 1) print gErrorCount " Errors\n"
}

