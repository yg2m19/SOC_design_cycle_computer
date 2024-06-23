// Intel HEX to Verilog converter.
//
// Usage:
//    hex2v_arm <infile> <bytes_per_word> [<rom_name_for_assign>]
//
// The output can be reditected into a verilog file as required.
//

// Currently this does not cope with byte addresses greater than 16-bits
// - these wouldrequire support for additional record types:
//     02 - extended segment address record
//     04 - extended linear address record


#define DATA_RECORD 0
#define END_OF_FILE_RECORD 1
#define EXTENDED_SEGMENT_ADDRESS_RECORD 2
#define START_SEGMENT_ADDRESS_RECORD 3
#define EXTENDED_LINEAR_ADDRESS_RECORD 4
#define START_LINEAR_ADDRESS_RECORD 5

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#define MAX_MEMORY_SIZE  65536
#define MS_BYTE_FIRST  0
#define MAX_LINE_LENGTH  256

// Global variables for memory and for bytes per word
struct {
   unsigned int  nAddress;
   unsigned int  byData;
} DataArray[MAX_MEMORY_SIZE];

unsigned int BytesPerWord = 4;


int store_word (int index, int address, int data) {
  // printf ("Address:%d Data %08x  (%d-bytes)\n", address, data, BytesPerWord);

  if ( index >= MAX_MEMORY_SIZE ) {
    // this test is to avoid a segmentation fault
    // (I don't think the limit will be reached if we don't support extended address record types)
    fprintf( stderr, "ERROR - Size of intel hex file exceeds maximum that this program can cope with.\n");
    fprintf( stderr, "          (%d %d-byte words)\n\n", MAX_MEMORY_SIZE, BytesPerWord);
    exit( EXIT_FAILURE );
  }

  DataArray[index].nAddress = address;
  DataArray[index].byData   = data;

}

int print_hex_word (int data) {

  if (BytesPerWord == 2) 
   printf ("%04X", data);
  else if (BytesPerWord == 4) 
   printf ("%08X", data);
  else
   printf ("%02X", data);

}

int print_formated_hex_data (int index, int with_assign, char* memory_name ) {

  unsigned int address, data;

  address = DataArray[index].nAddress;
  data = DataArray[index].byData;

  //   
  //   by default the format is for $readmemh
  //     @<HEX_ADDR> <HEX_HATA>
  //   
  //   the alternative format is raw verilog
  //     assign <MEM_NAME>[ <ADDR> ] = <n>'h<HEX_HATA>;
  //
  if ( with_assign )
    printf( "  assign %s[ %d ] = %d'h", memory_name, address, BytesPerWord*8 );
  else
    printf( "@%04X ", address);

  print_hex_word (data);

  if ( with_assign ) printf( ";" );

  printf( "\n" );

}


int print_usage_and_exit() {

  fprintf( stderr, "\n");
  fprintf( stderr, "Intel HEX File to Verilog memory file for ARM\n");
  fprintf( stderr, "Usage: hex2v_arm <infile> <bytes_per_word> [<rom_name_for_assign>]\n");
  fprintf( stderr, "\n");
  
  exit( EXIT_FAILURE );
}


int main (int argc, char *argv[])
{
   int  i;
   int  output_as_assign=0;
   int  contiguous=0;
   int which_byte=0;
   unsigned int  word_data=0;
   unsigned int  num_words_stored=0;
   unsigned int data=0;
   unsigned int bytes_fetched;
   unsigned int  start_address, word_start_address, address, ndata_bytes, record_type;

   char IntelHexLine[MAX_LINE_LENGTH];
   FILE *IntelHexFile;


   if ((argc != 3) && (argc != 4)) {
      print_usage_and_exit();
   }
   
   if (argc == 4) output_as_assign=1;


   // Open Intel HEX file for read
   IntelHexFile=fopen(argv[1], "r");
   if (!IntelHexFile) {
     fprintf( stderr, "ERROR - Can't open input file %s.\n", argv[1]);
     exit( EXIT_FAILURE);
   }
   
   if ( strcmp( argv[2], "1" ) == 0 ) {
      BytesPerWord = 1;
   } else if ( strcmp( argv[2], "2" ) == 0 ) {
      BytesPerWord = 2;
   } else if ( strcmp( argv[2], "4" ) == 0  ) {
      BytesPerWord = 4;
   } else {
     fprintf( stderr, "ERROR - expected bytes per word == 1, 2 or 4.\n", argv[1]);
     exit( EXIT_FAILURE);
   }

   // Read in the HEX file
   //
   // The Intel format is byte orientated with a 16-bit address
   // ARM supports either 16- or 32-bit instructions and 32-bit addresses
   //
   // 32-bit addresses are supported in the Intel format via the '04' record type
   // this 'extended linear address record' is not currently supportedby this code
   //
   num_words_stored = 0;
   bytes_fetched = 0;
   while (!feof(IntelHexFile)) {
      // Get one Intel HEX line
      fgets (IntelHexLine, MAX_LINE_LENGTH, IntelHexFile);
      if (strlen(IntelHexLine) >= 10) {
         // This is the ARM, with its 16-bit instruction words and 32-bit data words.
         // We're normally interested in these 16/32-bit words rather than the bytes.
         // We will read 2, 4 or 8 hex digits at a time and then store the data.
         //
         sscanf (&IntelHexLine[1], "%2x%4x%2x", &ndata_bytes, &start_address, &record_type);
         if ( ( ndata_bytes > 0 ) && record_type == DATA_RECORD ) {

            // printf ("%d  - %d  (%d)\n", start_address, start_address+ndata_bytes-1, record_type);

            // Suck up data bytes starting at 9th byte.
            i = 9;

            // Words.. not bytes..
            
            if ( address == start_address ) {
              contiguous = 1;
              // printf( "Contiguous Address:%d\n", address );
            } else if (( start_address % BytesPerWord ) != 0) {
              fprintf( stderr, "ERROR - start address (0x%x) is not aligned to %d-byte boundary\n", start_address, BytesPerWord);
              exit( EXIT_FAILURE);
            }

            // if (( ndata_bytes % BytesPerWord ) != 0) {
            //   fprintf( stderr, "WARNING - number of bytes (%d) is not a multiple of %d\n", ndata_bytes, BytesPerWord);
            // }
            
            if ( ( bytes_fetched != 0 ) && ! contiguous ) {
              // spare bytes need storing
              store_word( num_words_stored++, address/BytesPerWord, word_data );
	      word_data = 0;
              bytes_fetched = 0;
            }
            
            for (address = start_address; address < start_address + ndata_bytes; address++) {
              which_byte = address % BytesPerWord;
	      sscanf (&IntelHexLine[i], "%02x", &data);
              bytes_fetched++;
              // printf ("Address:%d Data %02x\n", address, data);
	      if ( MS_BYTE_FIRST )
		word_data = word_data  + ( data << (8*(BytesPerWord-which_byte-1)));
	      else
		word_data = word_data  + ( data << (8*which_byte));

              if ( bytes_fetched == BytesPerWord ) {

                store_word( num_words_stored++, address/BytesPerWord, word_data );
	        word_data = 0;
                bytes_fetched = 0;



              }
              i += 2;
              
            }
            // if ( bytes_fetched != 0 ) printf ("bytes remaining at end of line:  %d\n", bytes_fetched);
            
       } else if ( record_type == EXTENDED_LINEAR_ADDRESS_RECORD ) {
         fprintf( stderr, "ERROR - Extended address format records not supported by this program (%02x).\n", record_type);
         exit( EXIT_FAILURE);
       } else if (( record_type == EXTENDED_SEGMENT_ADDRESS_RECORD ) ||
                  ( record_type == START_SEGMENT_ADDRESS_RECORD )) {
         fprintf( stderr, "ERROR - Unxpected record type (%02x) relates to segment addresses not used by ARM.\n", record_type);
         exit( EXIT_FAILURE);
       } else if (( record_type != END_OF_FILE_RECORD ) &&
                  ( record_type != START_LINEAR_ADDRESS_RECORD )) {
         fprintf( stderr, "ERROR - Unxpected record type (%02x).\n", record_type);
         exit( EXIT_FAILURE);
       }
     }
   }
   fclose (IntelHexFile);

   // store spare bytes if necessary
   if ( bytes_fetched != 0 ) store_word( num_words_stored++, address/BytesPerWord, word_data );

   // Output the words one at a time

   for (i = 0; i < num_words_stored; i++) {
   
      print_formated_hex_data (i, output_as_assign, argv[3] );

   }

}

