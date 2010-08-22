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

Stream writeInt(Stream x, int a) {	
	string v = addr_ (&a)
	x << charOf ( 0x000000FF & a  )
	x << charOf ( 0x000000FF & a / 0x100 )
	x << charOf ( 0x000000FF & a / 0x10000 )
	x << charOf ( 0x000000FF & a / 0x1000000 )	
	return x
}

Stream readInt(Stream x, int& c) {	
	int nr = 0	
	int ad = (addr_ (&nr)) int 
	char &ch0 = addr_ ad 
	char &ch1 = addr_ (ad + 1)
	char &ch2 = addr_ (ad + 2)
	char &ch3 = addr_ (ad + 3)	
	x >> ch0; x >> ch1; x >> ch2; x >> ch3;    
	c = nr	
	return x
}
 
void readText (Stream x, Buffer &b) {
	int nr = 0 
	readInt (x, nr)  
	b = create nr
	x >> b
} 

void writeText (Stream x, Buffer b) {
	writeInt (x, 1+length b)
	x << b 
}