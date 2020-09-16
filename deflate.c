/**
  * @file deflate.c
  * @author Kendall Bagley (kabagley)
  * This program can take an input file and compress it down using a code system that represents
  * each of the 256 ascii character codes in a much smaller way. It converts bytes from the input
  * into variable length codes that get stored bit by bit into a bit buffer and printed out to a
  * binary file.
  */

#include "bitbuffer.h"
#include "codes.h"

/** This is the program starting point for deflate. It checks for the appropriate amount of
  * command line arguments, loads the code file, creates a new bit buffer aloting room for the
  * size field, then inputs the variable length codes into the bit buffer based off of the bytes
  * read in from the input file, and updates the size field to the number of bytes read in.
  * @param argc number of command line arguments
  * @param argv list of command line arguments containing the codes, input, and output files
  * @return program exit status */
int main( int argc, char *argv[] )
{
  //check that the expected amount of command line arguments were provided
  if ( argc != EXP_ARGS ) {
    fprintf( stderr, "usage: deflate <code-file> <infile> <outfile>\n" );
    return EXIT_FAILURE;
  }

  loadCodes( argv[ CODES_FILE ] );

  BitBuffer *buffer = makeBitBuffer();

  //alot enough room for the size field
  for ( int i = 0; i < SIZE_FIELD; i++ )
    appendBit( buffer, 0 );

  //open the input file and check for errors
  FILE *input = fopen( argv[ INPUT_FILE ], "rb" );
  if ( !input ) {
    perror( argv[ INPUT_FILE] );
    exit( EXIT_FAILURE );
  }

  unsigned int byte;
  //track the number of bytes read in to later update the size field
  unsigned int count = 0;

  //continuously read in bytes from the input until the end of file is reached
  while ( ( byte = fgetc( input ) ) != EOF ) {
    count++;
    //get the code that matches the byte found
    char const *c_byte = getCode( (unsigned char ) byte );

    //append each bit of the appropriately matched code to the bit buffer
    for ( int i = 0; i < strlen( c_byte ); i++ )
      appendBit( buffer, c_byte[ i ] == '0' ? 0 : 1 );
  }

  fclose( input );

  //update the size field with the number of bytes inputted
  for ( int i = 1; i <= SIZE_FIELD; i++ )
    setBit( buffer, ( i - 1 ), ( count & ( 1 << ( SIZE_FIELD - i ) ) ) >> ( SIZE_FIELD - i ) );

  saveBitBuffer( buffer, argv[ OUTPUT_FILE ] );
  freeBitBuffer( buffer );
  freeCodes();
  return EXIT_SUCCESS;
}
