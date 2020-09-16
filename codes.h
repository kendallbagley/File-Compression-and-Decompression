/**
  * @file codes.h
  * @author Kendall Bagley (kabagley)
  * This header prototypes the functions used in the codes implementation that allow different
  * code files to be inputed and then used to compress and decompress files by giving access to
  * get and lookup those inputed codes. It also defines the constant variables that determine the
  * allowable size of the codes and potential error codes to report.
 */

#ifndef _CODES_H_
#define _CODES_H_

#include "codes.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/** The total number of codes that must be loaded. Each occupying its own row in the codes two
  * dimensional array. */
#define CODE_ROWS 256

/** Number of bits in a byte.  */
#define BBITS 8

/** Maximum number of bits in a code. */
#define CODE_MAX 24

/** Return value from lookupCode() indicating that the given
  * string is the prefix of a code (so, it needs more bits
  * to match a code exactly. */
#define CODE_PREFIX -1

/** Return value from lookupCode() indicating that the given
  * string doesn't match (and isn't a prefix of) any code.  This
  * should only happen in case of an error. */
#define CODE_INVALID -2

/** Loads the given list of codes supplied by filename into a two dimensional array while checking
  * to make sure only 0's and 1's are being inputed, there are exactly 256 codes, each code is no
  * longer than 24 characters, and no code is a prefix of another.
  * @param filename File containing the list of codes that must be read in. */
void loadCodes( char const *filename );

/** If there was any memory being used by the codes implementation, this this would free it, but
  * my implementation does not use heap memory. */
void freeCodes();

/** Retrieves the code as a string of the character provided.
  * @param byte Character value being represented by ASCII code.
  * @return a string that contains a sequence of '0' and '1' that represents the character value
  * parameter in our compressed code */
char const *getCode( unsigned char byte );

/** Given a string of '0' and '1' characters representing string of bits,
  * report whether this sequence of bits matches the code for any
  * byte.
  * @param bits String containing a sequence of '0' and '1'.
  * @return the byte value for a matching code, or values CODE_PREFIX
  * or CODE_INVALID described above. */
int lookupCode( char const *bits );

#endif
