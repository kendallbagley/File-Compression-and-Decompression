/**
  * @file bitbuffer.h
  * @author Kendall Bagley (kabagley)
  * This header file protoytpes and defines all that is necessary to create, manipulate, and
  * reference an implementation of a bit buffer which stores a resizeable array that works to
  * compress and decompress files via the use of codes.
 */

#ifndef _BITBUFFER_H_
#define _BITBUFFER_H_

#include "bitbuffer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** The number needed to shift between the low order bit and the high order bit in a byte */
#define SHIFT 7

/** The starting size of the resizeable array used in BitBuffer */
#define START_SIZE 4

/** Number of bits in a byte */
#define BYTE_SIZE 8

/** Array index for the codes file in the command line arguments */
#define CODES_FILE 1

/** Array index for the input file in the command line arguments */
#define INPUT_FILE 2

/** Array index for the output file in the command line arguments */
#define OUTPUT_FILE 3

/** Size of the size field that occupies the first section of bits in a bit buffer */
#define SIZE_FIELD 32

/** The number of expected arguments from the command line */
#define EXP_ARGS 4

/** Representation of a resizable array of bytes, with functions to
    make it easy to access individual bits. */
typedef struct {
  /** Resizable array of bytes stored in this buffer. */
  unsigned char *data;

  /** Number of bits currently stored in the data array (may not
      always be a multiple of 8). */
  int bitCount;

  /** Capacity of the data array, measured in bytes. */
  int cap;
} BitBuffer;

/** Returns a dynamically allocated new bit buffer that has been initialized to empty.
  * @return a pointer to a newly created instance of BitBuffer. */
BitBuffer *makeBitBuffer();

/** Frees any resources used by the given bit buffer
  * @param buffer the instance of bit buffer that must be freed. */
void freeBitBuffer( BitBuffer *buffer );

/** Returns the number of bits currently stored in the given bit buffer
  * @param buffer the instance of bit that we are returning the number of bits of
  * @return the interger value of the number of bits being used by the given bit buffer */
int getBitCount( BitBuffer const *buffer );

/** Returns the bit value of either 0 or 1 at the given index value of the bit buffer
  * @param buffer the instance of bit buffer being search
  * @param idx the index value of the bit being requested
  * @return a bool that is either 0 or 1 to match the value of the bit requested */
bool getBit( BitBuffer const *buffer, int idx );

/** Sets the value of the bit at the given index within the bit buffer provided to either 0 or 1
  * based on the value included in the parameters.
  * @param buffer the instance of bit buffer that needs to be updated
  * @param idx the index of the bit in bit buffer that needs to be updated
  * @param value the value of either 0 or 1 that the bit will be updated to */
void setBit( BitBuffer const *buffer, int idx, bool value );

/** Adds a bit to the end of the bit buffer to the value indicated, and it dynamically resizes the
  * bit buffer's data array if it ever runs out of room
  * @param buffer the instance of bit buffer that is being added to
  * @param the value that the bit being added will be set to */
void appendBit( BitBuffer *buffer, bool value );

/** Takes a file that is storing a printed out bit buffer data array and inputs it into a new
  * instance of a bit buffer that is then returned
  * @param filename the file that is storing the bit buffer that must be read in
  * @return a pointer to a newly created bit buffer containing the contents of the file provided */
BitBuffer *loadBitBuffer( char const *filename );

/** Saves the contents of the bit buffer to an output file that has the name included
  * @param buffer the instance of bit buffer that is being printed out to the file
  * @param filename the name of the binary output file that is being printed to */
void saveBitBuffer( BitBuffer *buffer, char const *filename );

#endif
