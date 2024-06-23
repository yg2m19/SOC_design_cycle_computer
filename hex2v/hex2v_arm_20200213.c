// Copyright (c) 1999 Thomas Coonan (tcoonan@mindspring.com)
//
//    This source code is free software; you can redistribute it
//    and/or modify it in source code form under the terms of the GNU
//    General Public License as published by the Free Software
//    Foundation; either version 2 of the License, or (at your option)
//    any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
//

// Intel HEX to Verilog converter.
//
// Usage:
//    hex2v_arm <infile> <bytes_per_word> [<rom_name_for_assign>]
//
// You probably want to simply redirect the output into a file.
//

// this is a quick and dirty modification of an AVR version which was a modification of a PIC version!
// - the whole thing really needs to be re-written
// this version supports one byte per word

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

// Input and Output file streams.
FILE *fpi;

// Well.. Let's read stuff in completely before outputting.. Programs
// should be pretty small..
//
#define MAX_MEMORY_SIZE  65536
#define MS_BYTE_FIRST  0
struct {
   unsigned int  nAddress;
   unsigned int  byData;
} Memory[MAX_MEMORY_SIZE];

char szLine[80];
unsigned int  start_address, address, ndata_bytes, ndata_words, record_type;
unsigned int  data, word_data, byte_count;
unsigned int  nMemoryCount;
unsigned int  bytes_per_word;

int main (int argc, char *argv[])
{
   int  i;
   int  output_as_assign=0;

   if ((argc != 3) && (argc != 4)) {

      printf ("\nIntel HEX File to Verilog memory file for ARM");
      printf ("\nUsage: hex2v_arm <infile> <bytes_per_word> [<rom_name_for_assign>]");
      printf ("\n");
      return 0;
   }
   
   if (argc == 4) output_as_assign=1;


   // Open input HEX file
   fpi=fopen(argv[1], "r");
   if (!fpi) {

      printf("\nCan't open input file %s.\n", argv[1]);
      return 1;
   }
   
   if ( strcmp( argv[2], "1" ) == 0 ) {
      bytes_per_word = 1;
   } else if ( strcmp( argv[2], "2" ) == 0 ) {
      bytes_per_word = 2;
   } else if ( strcmp( argv[2], "4" ) == 0  ) {
      bytes_per_word = 4;
   } else {
      printf("\nexpected bytes per word == 1, 2 or 4\n", argv[1]);
     return 0;
   }

   // Read in the HEX file
   //
   // !! Note, that things are a little strange for us, because the PIC is
   //    a 12-bit instruction, addresses are 16-bit, and the hex format is
   //    8-bit oriented!!
   //
   nMemoryCount = 0;
   while (!feof(fpi)) {
      // Get one Intel HEX line
      fgets (szLine, 80, fpi);
      if (strlen(szLine) >= 10) {
         // This is the AVR, with its 16-bit "words".  We're interested in these
         // words and not the bytes.  Read 4 hex digits at a time for each
         // address.
         //
         sscanf (&szLine[1], "%2x%4x%2x", &ndata_bytes, &start_address, &record_type);
         if (start_address >= 0 && start_address <= 20000 && ndata_bytes > 0 && record_type == 0 ) {

            // printf ("\n%d  - %d  (%d)", start_address, start_address+ndata_bytes-1, record_type);

            // Suck up data bytes starting at 9th byte.
            i = 9;

            // Words.. not bytes..
            ndata_words   = ndata_bytes/bytes_per_word;
            start_address = start_address/bytes_per_word;

            // Spit out all the data that is supposed to be on this line.
            for (address = start_address; address < start_address + ndata_words; address++) {
	       word_data = 0;
	       
	       for ( byte_count = 0 ; byte_count < bytes_per_word; byte_count++ ) {
	         sscanf (&szLine[i], "%02x", &data);
	         if ( MS_BYTE_FIRST )
		   word_data = ( word_data << 8 ) + data;
		 else
		   word_data = word_data  + ( data << (8*byte_count));
                 i += 2;
	       }
	       
	       
               // Scan out 4 hex digits for a word.  This will be one address.
               // sscanf (&szLine[i], "%04x", &data);

               // Need to swap bytes...
               // data = ((data >> 8) & 0x00ff) | ((data << 8) & 0xff00);
               // i += 4;

               // Store in our memory buffer
	       if ( nMemoryCount >= MAX_MEMORY_SIZE ) {
	         // this test is to avoid a segmentation fault
	         printf ("\nERROR - Size of intel hex file exceeds maximum that this program can cope with.\n");
	         printf ("          (%d %d-byte words)\n\n", MAX_MEMORY_SIZE, bytes_per_word);
		 exit (0);
	       }
               Memory[nMemoryCount].nAddress = address;
               Memory[nMemoryCount].byData   = word_data;
               nMemoryCount++;
            }

         }
      }
   }
   fclose (fpi);

   // Now output the Verilog $readmemh format!
   //
   for (i = 0; i < nMemoryCount; i++) {
      if ( output_as_assign )
        printf( "\n  assign %s[ %d ] = %d'h", argv[3], Memory[i].nAddress, bytes_per_word*8 );
      else
        printf( "\n@%04X ", Memory[i].nAddress);
      
      if (bytes_per_word == 2) 
       printf ("%04X", Memory[i].byData);
      else if (bytes_per_word == 4) 
       printf ("%08X", Memory[i].byData);
      else
       printf ("%02X", Memory[i].byData);

      if ( output_as_assign )
        printf( ";" );
   }

   printf ("\n");

}

