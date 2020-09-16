/**
  * @file codes.c
  * @author Kendall Bagley (kabagley)
  * This file implements all functions that work on the codes file and allow other code to interact
  * with its information. It inputs the codes from the provided file while also checking that the
  * inputted codes are both within the size bounds allowed and function in the way expected i.e.
  * there are no codes that are prefixes of another.
  */

#include "codes.h"

/** A global variable that contains all the code values indexed by their ASCII code */
static char codes[ CODE_ROWS ][ CODE_MAX + 1 ];

/** Outputs the error information whenever a codes file does not meet required expectations which
  * include containing 256 codes that are not prefixes of each other and that are never longer
  * than 24 characters
  * #param filename File of codes that do not meet the code requirments. */
static void codesError( char const *filename )
{
  fprintf( stderr, "Invalid code file: %s\n", filename );
  exit( EXIT_FAILURE );
}

/** Checks that the newly input code is not a prefix of any of the previously inputted codes
  * @param row The row number that the newly inputed code is located at
  * @return a boolean value that is true if the new code is not a prefix of any others and false
  * if the new code is a prefix of another code and is therefore not allowable. */
static bool checkPrefix( int row )
{
  //iterates through all the previously inputted codes
  for ( int i = 0; i < row; i++ ) {
    //only compares up to the last digit of the smallest code
    int n = ( strlen(codes[row]) < strlen(codes[i]) ) ? strlen(codes[row]) : strlen(codes[i]);
    if ( strncmp( codes[ row ], codes[ i ], n ) == 0 )
      return false;
  }

  return true;
}

void loadCodes( char const *filename )
{
  //open input file and checks that it opened successfully
  FILE *fp = fopen( filename, "r" );
  if ( !fp ) {
    perror ( filename );
    exit( EXIT_FAILURE );
  }

  //values that keep track of where to input the next individual bit value
  int bit;
  int row = 0;
  int col = 0;

  //iterates through each char value in the file and puts it into the correct location in the
  //two dimensional codes array
  while ( ( bit = fgetc( fp ) ) != EOF ) {
    //only allows 0's and 1's and new line characters and checks that we're not exceeding the
    //allowed number of codes
    if ( ( (char) bit != '0' && (char) bit != '1' && (char) bit != '\n' ) || row == CODE_ROWS )
      codesError( filename );

    //replaces every new line with a null terminator for the string
    if ( ( (char) bit ) == '\n' ) {
      codes[ row ][ col ] = '\0';

      //checks that the newly inputed code is not a prefix of any others
      if ( !checkPrefix( row ) )
        codesError( filename );

      //update array values
      row++;
      col = 0;
    } else {
      //saves new character in two dimensional array as long as it's within the allowed 24
      //character length
      if ( col <= CODE_MAX )
        codes[ row ][ col++ ] = ( char ) bit;
      else
        codesError( filename );
    }
  }

  //checks that no more than 256 codes were saved
  if ( row != CODE_ROWS )
    codesError( filename );

  //close the file now that we're done
  fclose( fp );
}

void freeCodes()
{
  //did not use any heap memory
}

char const *getCode( unsigned char byte )
{
  //return the code string at the index for the ascii code character provided
  return codes[ byte ];
}

int lookupCode( char const *bits )
{
  //cycles through all the codes
  for ( int i = 0; i < CODE_ROWS; i++ ) {
    //returns the index if it's an exact match and a -1 if it's a prefix
    if ( strcmp( bits, codes[i] ) == 0 )
      return i;
    else if ( strncmp( bits, codes[i], strlen( bits ) ) == 0 )
      return CODE_PREFIX;
  }

  //returns an error code if there was no perfect or prefix match, meaning bits was invalid
  return CODE_INVALID;
}
