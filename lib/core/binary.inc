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



/*
// The following array was created using this code: 
char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' }
int i,j;  for (i = 0; i < 16; i++) { for (j = 0; j < 16; j++) print '"' hexChars[i] hexChars[j] '"' ", "; print "\n" }
*/

string hexChars[256] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", 
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", 
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", 
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", 
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", 
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F", 
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F", 
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F", 
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F", 
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", 
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", 
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF", 
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", 
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", 
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF", 
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
}

// for lightning fast hex conversion we prepare 4 references, that point to the 4 Bytes
// of an integer. For this we reserver 8 Bytes space with the hexVal array. Then
// we prepare 4 pointers (A,B,C,D) that have their first byte point to each byte of
// hexVal[0]:

//  hexVal[0]      hexVal[1] 
// 00 00 00 00    00 00 00 00
//           ^     ^  ^  ^
//           D     C  B  A

int hexVal[2] = {0,0};
int hexVal2[2] = {0,0};

int hexPtr = addr_ (&(hexVal[1]))
int &nibbleA = addr_ (--hexPtr)
int &nibbleB = addr_ (--hexPtr)
int &nibbleC = addr_ (--hexPtr)
int &nibbleD = addr_ (--hexPtr)

int hexPtr2 = addr_ (&(hexVal2[0]))
int &nibbleA2 = addr_ (hexPtr2++)
int &nibbleB2 = addr_ (hexPtr2++)
int &nibbleC2 = addr_ (hexPtr2++)
int &nibbleD2 = addr_ (hexPtr2++)

// For making a lightning fast int2hex we prepare 
Buffer bufResult = create(8)
string hexNumber(int a) {
    hexVal[0] = a // copy the value to our prepared array;
    bufResult = "0x";
    bufResult += hexChars[nibbleA & 0xFF]; 
    bufResult += hexChars[nibbleB & 0xFF]; 
    bufResult += hexChars[nibbleC & 0xFF]; 
    bufResult += hexChars[nibbleD & 0xFF];
    return stringOf bufResult; 
}

// Lookup table to inverse Hex Nibbles, created by this code: 

// bool high = true; 
// int i,j; for (j = 0; j < 256; j++) {
// if (j < 48 || j > 102) print "0," else { i = j - 48;
//   int val = i, add = 0
//   if  (val > 9) { val = i-17; add = 10; if  (val > 5) { val = i-49; }}
//   if (i%8 == 0) print "\n"
//   print "/* " (i+48) "<->"(charOf (i+48)) "*/ " 
//   if (val < 0) print "0," else print ((high ? 8 : 1)*(val + add)) ",";
//   }
// }


int inverseHexLow[256] = { 
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 48<->0*/ 0,/* 49<->1*/ 1,/* 50<->2*/ 2,/* 51<->3*/ 3,/* 52<->4*/ 4,/* 53<->5*/ 5,/* 54<->6*/ 6,/* 55<->7*/ 7,
    /* 56<->8*/ 8,/* 57<->9*/ 9,/* 58<->:*/ 0,/* 59<->;*/ 0,/* 60<-><*/ 0,/* 61<->=*/ 0,/* 62<->>*/ 0,/* 63<->?*/ 0,
    /* 64<->@*/ 0,/* 65<->A*/ 10,/* 66<->B*/ 11,/* 67<->C*/ 12,/* 68<->D*/ 13,/* 69<->E*/ 14,/* 70<->F*/ 15,/* 71<->G*/ 0,
    /* 72<->H*/ 0,/* 73<->I*/ 0,/* 74<->J*/ 0,/* 75<->K*/ 0,/* 76<->L*/ 0,/* 77<->M*/ 0,/* 78<->N*/ 0,/* 79<->O*/ 0,
    /* 80<->P*/ 0,/* 81<->Q*/ 0,/* 82<->R*/ 0,/* 83<->S*/ 0,/* 84<->T*/ 0,/* 85<->U*/ 0,/* 86<->V*/ 0,/* 87<->W*/ 0,
    /* 88<->X*/ 0,/* 89<->Y*/ 0,/* 90<->Z*/ 0,/* 91<->[*/ 0,/* 92<->\*/ 0,/* 93<->]*/ 0,/* 94<->^*/ 0,/* 95<->_*/ 0,
    /* 96<->`*/ 0,/* 97<->a*/ 10,/* 98<->b*/ 11,/* 99<->c*/ 12,/* 100<->d*/ 13,/* 101<->e*/ 14,/* 102<->f*/ 15,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
}

int inverseHexHigh[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 48<->0*/ 0,/* 49<->1*/ 16,/* 50<->2*/ 32,/* 51<->3*/ 48,/* 52<->4*/ 64,/* 53<->5*/ 80,/* 54<->6*/ 96,/* 55<->7*/ 112,
    /* 56<->8*/ 128,/* 57<->9*/ 144,/* 58<->:*/ 0,/* 59<->;*/ 0,/* 60<-><*/ 0,/* 61<->=*/ 0,/* 62<->>*/ 0,/* 63<->?*/ 0,
    /* 64<->@*/ 0,/* 65<->A*/ 160,/* 66<->B*/ 176,/* 67<->C*/ 192,/* 68<->D*/ 208,/* 69<->E*/ 224,/* 70<->F*/ 240,/* 71<->G*/ 0,
    /* 72<->H*/ 0,/* 73<->I*/ 0,/* 74<->J*/ 0,/* 75<->K*/ 0,/* 76<->L*/ 0,/* 77<->M*/ 0,/* 78<->N*/ 0,/* 79<->O*/ 0,
    /* 80<->P*/ 0,/* 81<->Q*/ 0,/* 82<->R*/ 0,/* 83<->S*/ 0,/* 84<->T*/ 0,/* 85<->U*/ 0,/* 86<->V*/ 0,/* 87<->W*/ 0,
    /* 88<->X*/ 0,/* 89<->Y*/ 0,/* 90<->Z*/ 0,/* 91<->[*/ 0,/* 92<->\*/ 0,/* 93<->]*/ 0,/* 94<->^*/ 0,/* 95<->_*/ 0,
    /* 96<->`*/ 0,/* 97<->a*/ 160,/* 98<->b*/ 176,/* 99<->c*/ 192,/* 100<->d*/ 208,/* 101<->e*/ 224,/* 102<->f*/ 240,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
}

int hex2Int (_y s) {
    int *ad = addr_ s; 
    hexVal[0]  = *ad // input array -> first 4 byte blocks of the string
    hexVal2[0] = 0;  // our output 
    
    // Compress first 4 Bytes into low word of output variable, converting the nibbles
    nibbleC2 += inverseHexLow  [nibbleA & 0xFF] 
    nibbleC2 += inverseHexHigh [nibbleB & 0xFF]
    nibbleD2 += inverseHexLow  [nibbleC & 0xFF]
    nibbleD2 += inverseHexHigh [nibbleD & 0xFF]

    // Compress next 4 Bytes into low word of output variable, converting the nibbles
    ad += 4
    hexVal[0] = *ad 
    nibbleA2 += inverseHexLow  [nibbleA & 0xFF] 
    nibbleA2 += inverseHexHigh [nibbleB & 0xFF]
    nibbleB2 += inverseHexLow  [nibbleC & 0xFF]
    nibbleB2 += inverseHexHigh [nibbleD & 0xFF]
    return hexVal2[0]; // return output value
}



