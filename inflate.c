/**
  * @file inflate.c
  * @author Kendall Bagley (kabagley)
  * This program decompresses a file that contains a printed out bit buffer full of codes that
  * represent ascii character codes. Essentially, this is able to take the unreadable compressed
  * file and return it to its original state.
  */

#include "bitbuffer.h"
#include "codes.h"

/** This is the program starting point for inflate. It loads the all the codes from the codes
  * file, loads the bit buffer that needs to be decompressed, and outputs the original input file
  * by searching for recognizable codes and printing out their corresponding ascii characters.
  * @param argc number of command line arguments
  * @param argv list of command line arguments including the codes, input, and output files
  * @return program exit status */
int main( int argc, char *argv[] )
{
  //checks for the expected number of command line arguments
  if ( argc != EXP_ARGS ) {
    fprintf( stderr, "usage: inflate <code-file> <infile> <outfile>\n" );
    return EXIT_FAILURE;
  }

  loadCodes( argv[ CODES_FILE ] );

  BitBuffer *buffer = loadBitBuffer( argv[ INPUT_FILE ] );
  unsigned int count = 0;

  //reads in the size field from the loaded in bit buffer
  for ( int i = 0; i < SIZE_FIELD; i++ )
    count = ( count << 1 ) | getBit( buffer, i );

  //opens the output file to write to
  FILE *output = fopen( argv[ OUTPUT_FILE ], "w" );

  //array values to track the codes found in the bit buffer
  int bitbuffercount = getBitCount( buffer );
  char bits[ CODE_MAX + 1 ] = { 0 };
  int bitscount = 0;
  int charcount = 0;

  //iterates through the bit buffer starting after the size field
  for ( int i = SIZE_FIELD; i < bitbuffercount && charcount < count; i++ ) {
    //bits holds the code being checked for a potential match and it continues to grow until a
    //match is found
    bits[ bitscount ] = getBit( buffer, i ) == 0 ? '0' : '1';
    int code = lookupCode( bits );

    //if a match is found, then it prints the ascii code to the output file, increases the number
    //of character codes found by one and resets bit, so it can hold the next code
    if ( code >= 0 ) {
      fprintf( output, "%c", (char) code );
      charcount++;
      bitscount = 0;
      for ( int j = 0; j <= CODE_MAX; j++ )
        bits[ j ] = 0;
    //if the code was invalid, it prints an error message to standard error
    } else if ( code == CODE_INVALID ) {
      fprintf( stderr, "Invalid input file: %s\n", argv[ INPUT_FILE ] );
      return EXIT_FAILURE;
    //if the code was a prefix, then it updates the position for bits, so it can keep searching
    } else if ( code == CODE_PREFIX ) {
      bitscount++;
    }
  }

  //checks that it found the expected number of codes, and prints an error if not
  if ( charcount < count ) {
    fprintf( stderr, "Invalid input file: %s\n", argv[ INPUT_FILE ] );
    return EXIT_FAILURE;
  }

  fclose( output );
  freeBitBuffer( buffer );
  freeCodes();
  return EXIT_SUCCESS;
}
