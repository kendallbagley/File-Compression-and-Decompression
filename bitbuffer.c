/**
  * @file bitbuffer.c
  * @author Kendall Bagley (kabagley)
  * This implementation file executes all the functions that can create, manipulate, and access an
  * implementation of a bit buffer. Where a bit buffer contains a resizeable array that works to
  * compress and decompress files via a code system. Additionally, the execution of this bit
  * buffer accomplishes this compression and decompression via bit manipulation.
  */

#include "bitbuffer.h"

BitBuffer *makeBitBuffer()
{
  //dynamically allocate a new instance of bit buffer
  BitBuffer *n = ( BitBuffer * )malloc( sizeof( BitBuffer ) );

  //dynamically allocate a resizable array to all zeros that will store all of our codes
  n->data = ( unsigned char * )calloc( START_SIZE, sizeof( unsigned char ) );
  n->bitCount = 0;
  n->cap = START_SIZE;

  return n;
}

void freeBitBuffer( BitBuffer *buffer )
{
  //frees all heap memory being used by an instance of bit buffer
  free( buffer->data );
  free( buffer );
}

int getBitCount( BitBuffer const *buffer )
{
  return buffer->bitCount;
}

bool getBit( BitBuffer const *buffer, int idx )
{
  //finds the index of the data array and the index of the bit needed at that location
  int d_idx = idx / BYTE_SIZE;
  int b_idx = idx % BYTE_SIZE;

  //clears everything but the bit needed and shifts it to the low order position
  return ( buffer->data[ d_idx ] & ( 1 << ( SHIFT - b_idx ) ) ) >> ( SHIFT - b_idx );
}

void setBit( BitBuffer const *buffer, int idx, bool value )
{
  //finds the index of the data array and the index of the bit needed at that location
  int d_idx = idx / BYTE_SIZE;
  int b_idx = idx % BYTE_SIZE;

  //updates the desired bit to the value provided as a parameter
  buffer->data[ d_idx ] = ( buffer->data[ d_idx ] & ~( 1 << ( SHIFT - b_idx ) ) )
                            | ( value << ( SHIFT - b_idx ) );

  return;
}

void appendBit( BitBuffer *buffer, bool value )
{
  //checks if the array size needs to be increased
  if ( buffer->bitCount >= buffer->cap * BYTE_SIZE ) {
    buffer->cap++;

    //dynamically allocates a new array to all zeros
    unsigned char *temp = ( unsigned char * )calloc( buffer->cap, sizeof( unsigned char ) );

    //copies the components of the previous array
    memcpy( temp, buffer->data, ( buffer->cap - 1 ) * sizeof( unsigned char ) );

    //frees the old array and sets the new array
    free( buffer->data );
    buffer->data = temp;
  }

  //adds the new bit to the end of the array
  setBit( buffer, buffer->bitCount, value );
  buffer->bitCount++;
  return;
}

BitBuffer *loadBitBuffer( char const *filename )
{
  //creates a new instance of bit buffer
  BitBuffer *n = makeBitBuffer();

  //opens the file with the bit buffer that needs to be read in
  FILE *fp = fopen( filename, "rb" );
  unsigned int current;

  //continues to read in each character until it reaches the end of file
  while ( ( current = fgetc( fp ) ) != EOF ) {

    //inputs each bit from the file into the newly create bit buffer
    for ( int i = 1; i <= BYTE_SIZE; i++ ) {
      int shift = BYTE_SIZE - i;
      appendBit( n, ( current & ( 1 << shift ) ) >> shift );
    }
  }

  fclose( fp );
  return n;
}

void saveBitBuffer( BitBuffer *buffer, char const *filename )
{
  //opens file that is being written to
  FILE *fp = fopen( filename, "wb" );
  size_t data_size;

  //determines the size of the array that will be written to the file
  if ( buffer->bitCount % BYTE_SIZE == 0 )
    data_size = buffer->bitCount / BYTE_SIZE;
  else
    data_size = ( buffer->bitCount / BYTE_SIZE ) + 1;

  //writes the data array to the file
  fwrite( buffer->data, sizeof( unsigned char ), data_size, fp );
  fclose( fp );
}
