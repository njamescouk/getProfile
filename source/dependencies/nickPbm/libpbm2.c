/* libpbm2.c - pbm utility library part 2
**
** Copyright (C) 1988 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.

Heavily hacked NickJ June 2014

*/

#include <climits>
#include "math.h"
#include "fileio.h"
#include "pbm.h"
#include "PbmColour.h" 
#include "PBitMapRow.h" 
#include "PBitMap.h" 

static PbmBW 
getbit (FILE * const file) {
    char ch;

    do {
        ch = pm_getc( file );
    } while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

    if ( ch != '0' && ch != '1' )
        fprintf(stderr, "junk in file where bits should be" );
    
    /* white is 0, see pbm.h */
    return ( ch == '1' ) ? PBIT_BLACK : PBIT_WHITE;
}



void
pbm_readpbminitrest(FILE* file, int* colsP, int* rowsP)
{
    /* Read size. */
    *colsP = (int)pm_getuint( file );
    *rowsP = (int)pm_getuint( file );

    /* *colsP and *rowsP really should be unsigned int, but they come
       from the time before unsigned ints (or at least from a person
       trained in that tradition), so they are int.  We could simply
       consider negative numbers to mean values > INT_MAX/2 and much
       code would just automatically work.  But some code would fail
       miserably.  So we consider values that won't fit in an int to
       be unprocessable.
    */
    if (*colsP < 0)
        fprintf(stderr, "Number of columns in header is too large.");
    if (*rowsP < 0)
        fprintf(stderr, "Number of columns in header is too large.");
}



static void
validateComputableSize(unsigned int const cols,
                       unsigned int const rows) {
/*----------------------------------------------------------------------------
   Validate that the dimensions of the image are such that it can be
   processed in typical ways on this machine without worrying about
   overflows.  Note that in C, arithmetic is always modulus
   arithmetic, so if your values are too big, the result is not what
   you expect.  That failed expectation can be disastrous if you use
   it to allocate memory.

   A common operation is adding 1 or 2 to the highest row or
   column number in the image, so we make sure that's possible.
-----------------------------------------------------------------------------*/
    if (cols > INT_MAX - 2)
        fprintf(stderr,  "image width (%u) too large to be processed", cols);
    if (rows > INT_MAX - 2)
        fprintf(stderr,  "image height (%u) too large to be processed", rows);
}



void
pbm_readpbminit(FILE* const ifP, int*  const colsP, int*  const rowsP, int*  const formatP)
{
    /* Check magic number. */
    *formatP = pm_readmagicnumber(ifP);
    int fmtType = PBM_FORMAT_TYPE(*formatP);

    switch (fmtType)
    {
    case PBM_TYPE:
        pbm_readpbminitrest(ifP, colsP, rowsP);
        break;

    default:
        fprintf(stderr,  "bad magic number - not a pbm file" );
    }
    validateComputableSize(*colsP, *rowsP);
}

void pbm_readpbmrow(FILE* file, PBitMapRow& pr, int format)
{
    switch ( format )
    {
        {
        case PBM_FORMAT:
            int col;
            for (col = 0; col < pr.getWidth(); ++col) 
            {
                PbmBW pixel;
                pixel = getbit(file);
                PbmColour pb;
                pb.setBW(pixel);
                pr[col] = pb; 
            }
        }
        break;

        case RPBM_FORMAT: 
        {
            int numberOfBits = pr.getWidth();
            int numberOfBytes = (int)ceil(numberOfBits/8.0);
            unsigned char item;
            int byteNdx;
            for (byteNdx = 0; byteNdx < numberOfBytes; byteNdx++)
            {
                int bitNdx = byteNdx*8;
                int validBits = 8;
                if (byteNdx == numberOfBytes - 1)
                {
                    validBits = numberOfBits - bitNdx;
                }

                item = pm_getrawbyte(file);

                pickTheBitsOutOfThis(item, validBits, bitNdx, pr);
            }
        }
        break;
    }
}

void
pbm_readpbm(FILE* file, PBitMap *pBitMap)
{
    int format;
    int rows = 0,
        cols = 0;

    pbm_readpbminit( file, &cols, &rows, &format );

    if (PBM_FORMAT_TYPE(format) == PBM_TYPE)
    {
        pBitMap->setSize(cols, rows);

        int row;
        for (row = 0; row < pBitMap->getHeight(); ++row)
            pbm_readpbmrow(file, pBitMap->getRowRef(row), format);
    }
}

int 
pm_readmagicnumber(FILE * const ifP) {
    int ich1, ich2;

    ich1 = getc(ifP);
    ich2 = getc(ifP);
    if (ich1 == EOF || ich2 == EOF)
        fprintf(stderr, "Error reading magic number from Netpbm image stream.  "
                  "Most often, this "
                  "means your input file is empty." );

    return ich1 * 256 + ich2;
}

void pickTheBitsOutOfThis(unsigned char item, int validBits, int bitNdx, PBitMapRow& pr)
{
    /* tempted to use a struct of 8 single bit-fields here :-) */
    /* BUG we should ensure we're reading msb first */
    int i;
    for (i = 0; i < validBits; i++)
    {
        /* pick validBits bits off front of item, append them to pr */
        unsigned char bitPos = 7 - i;
        unsigned char shiftedItem = (item >> bitPos);
        PbmColour colour = ((shiftedItem & 1)==0)?PBIT_WHITE:PBIT_BLACK;
        int columnNumber = bitNdx + i;
        pr[columnNumber] = colour;
    }
}
