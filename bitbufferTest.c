#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitbuffer.h"

int main()
{
  // Make an empty bit buffer.
  BitBuffer *buffer = makeBitBuffer();

  // Should start out empty.
  assert( getBitCount( buffer ) == 0 );

  // Try out the free function (valgrind could help to test this.
  freeBitBuffer( buffer );

  /////////////////////////////////////////////////////

  // Make another bit buffer.
  buffer = makeBitBuffer();

  // Add some bits to it.
  appendBit( buffer, 0 );
  appendBit( buffer, 1 );

  // Should now contain two bits, a zero and a one.
  assert( getBitCount( buffer ) == 2 );
  assert( getBit( buffer, 0 ) == 0 );
  assert( getBit( buffer, 1 ) == 1 );

  // Try adding some more bits.
  appendBit( buffer, 1 );
  appendBit( buffer, 0 );
  appendBit( buffer, 0 );
  appendBit( buffer, 1 );

  // Check the bits we just added.
  assert( getBitCount( buffer ) == 6 );
  assert( getBit( buffer, 2 ) == 1 );
  assert( getBit( buffer, 3 ) == 0 );
  assert( getBit( buffer, 4 ) == 0 );
  assert( getBit( buffer, 5 ) == 1 );
  
  // Append enough bits to extend into the next byte.
  appendBit( buffer, 0 );
  appendBit( buffer, 1 );
  appendBit( buffer, 1 );
  appendBit( buffer, 0 );

  // Check the last four bits
  assert( getBitCount( buffer ) == 10 );
  assert( getBit( buffer, 6 ) == 0 );
  assert( getBit( buffer, 7 ) == 1 );
  assert( getBit( buffer, 8 ) == 1 );
  assert( getBit( buffer, 9 ) == 0 );
  
  freeBitBuffer( buffer );

  /////////////////////////////////////////////////////

  // Try out the setBit() function.
  
  buffer = makeBitBuffer();

  // Add 8 zero bits and 8 1 bits.
  for ( int i = 0; i < 16; i++ )
    appendBit( buffer, i < 8 ? 0 : 1 );

  // Make sure the buffer looks right.
  assert( getBitCount( buffer ) == 16 );
  for ( int i = 0; i < 16; i++ )
    assert( getBit( buffer, i ) == ( i < 8 ? 0 : 1 ) );

  // Change the middle 8 bits. to 01010101
  for ( int i = 4; i < 12; i++ )
    setBit( buffer, i, i % 2 );
  
  // Make sure these middle bits are correct.
  for ( int i = 4; i < 12; i++ )
    assert( getBit( buffer, i ) == i % 2 );
  
  // Make sure the other bits didn't change.
  for ( int i = 0; i < 16; i++ )
    if ( i < 4 || i >= 12 )
      assert( getBit( buffer, i ) == ( i < 8 ? 0 : 1 ) );

  freeBitBuffer( buffer );
  
  /////////////////////////////////////////////////////

  // A sequence of bits to use for some of the remaining tests (Here,
  // we're just using a string to make it easy to get to each bit.  This
  // isn't how the BitBuffer is supposed to store bits.)
  char bits[] = "01101001" "01011010" "10001100" "10010101"
    "01010001" "01111111" "10111000";

  // Make bit buffer and append enough bits to force it to grow its data
  // array.
  buffer = makeBitBuffer();
  for ( int i = 0; bits[ i ]; i++ )
    appendBit( buffer, bits[ i ] == '0' ? 0 : 1 );

  // Check the number of bits in the buffer, and check each bit.
  assert( getBitCount( buffer ) == strlen( bits ) );
  for ( int i = 0; bits[ i ]; i++ ) {
    if ( bits[ i ] == '0' )
      assert( getBit( buffer, i ) == 0 );
    else
      assert( getBit( buffer, i ) == 1 );
  }

  // Try saving the contents of the buffer to a file.
  saveBitBuffer( buffer, "output.bin" );

  // Make sure the contents of the file look right.
  FILE *fp = fopen( "output.bin", "rb" );

  // Make sure we could open the file (did it got created).
  assert( fp );

  // Try to read up to 100 bytes from the file.
  unsigned char data[ 100 ];

  // Make sure the file is the right size (7 bytes)
  //printf( "%d", fread( data, sizeof( unsigned char ), 100, fp ) );
  assert( fread( data, sizeof( unsigned char ), 100, fp ) == 7 );
  fclose( fp );

  // Make sure the contents of the file are right.
  // This is the same sequence of bits as the bits array, but here it's
  // actually in binary rather than as a string of 0 and 1 characters.
  unsigned char rawBits[] = { 0x69, 0x5A, 0x8C, 0x95, 0x51, 0x7F, 0xB8 };
  for ( int i = 0; i < sizeof( rawBits ); i++ )
    assert( rawBits[ i ] == data[ i ] );

  freeBitBuffer( buffer );
//printf("made it" );

  /////////////////////////////////////////////////////

  // Try out loading a bit buffer from a file.
  buffer = loadBitBuffer( "output.bin" );

  // Check the number of bits in the buffer, and check each bit.
  assert( getBitCount( buffer ) == strlen( bits ) );
  for ( int i = 0; bits[ i ]; i++ ) {
    if ( bits[ i ] == '0' )
      assert( getBit( buffer, i ) == 0 );
    else
      assert( getBit( buffer, i ) == 1 );
  }

  freeBitBuffer( buffer );
  
  /////////////////////////////////////////////////////

  // If a bit buffer isn't a multiple of 8 bits, make sure it gets padded with
  // zero bits when we write it.

  // Make a buffer where the last byte only has one bit used.
  buffer = makeBitBuffer();
  for ( int i = 0; i < 49; i++ )
    appendBit( buffer, bits[ i ] == '0' ? 0 : 1 );

  // Last byte should be padded with zeros when we write it out.
  saveBitBuffer( buffer, "output.bin" );

  freeBitBuffer( buffer );

  // Read the contents of the file.
  assert( fp = fopen( "output.bin", "rb" ) );
  assert( fread( data, sizeof( unsigned char ), 100, fp ) == 7 );
  fclose( fp );
  
  // Check the contents of the file (mostly just checking the padding)
  unsigned char rawBitsPadded[] = { 0x69, 0x5A, 0x8C, 0x95, 0x51, 0x7F, 0x80 };
  for ( int i = 0; i < sizeof( rawBits ); i++ )
    assert( rawBitsPadded[ i ] == data[ i ] );

  return EXIT_SUCCESS;
}
