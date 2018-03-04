/* pbm.h - header file for libpbm portable bitmap library

Heavily hacked NickJ June 2016
*/

#ifndef _PBM_H_
#define _PBM_H_

#include <stdio.h>


typedef unsigned char bit;
#define PBM_WHITE 0
#define PBM_BLACK 1

class PBitMap;
class PBitMapRow;

/* Magic constants. */

#define PBM_MAGIC1 'P'
#define PBM_MAGIC2 '1'
#define RPBM_MAGIC2 '4'
#define PBM_FORMAT (PBM_MAGIC1 * 256 + PBM_MAGIC2)
#define RPBM_FORMAT (PBM_MAGIC1 * 256 + RPBM_MAGIC2)
#define PBM_TYPE PBM_FORMAT


/* Macro for turning a format number into a type number. */

#define PBM_FORMAT_TYPE(f) \
  ((f) == PBM_FORMAT || (f) == RPBM_FORMAT ? PBM_TYPE : -1)



/* Declarations of routines. */

void
pbm_init(int *   const argcP,
         char ** const argv);
void pbm_readpbminitrest ( FILE* file, int* colsP, int* rowsP );

void
pbm_nextimage(FILE *file, int * const eofP);

void pbm_readpbm(FILE* file, PBitMap *pBitMap);
void pbm_readpbminit(FILE* file, int* colsP, int* rowsP, int* formatP);
void pbm_readpbmrow(FILE* file, PBitMapRow& pr, int format);

void pbm_writepbm(FILE* const fileP, PBitMap *pBitMap);
void pbm_writepbminit(FILE* const fileP, int const cols, int const rows);
void pbm_writepbmrow(FILE * const fileP, PBitMapRow r);
void writePbmRowPlain(FILE * const fileP, PBitMapRow r);

int 
pm_readmagicnumber(FILE * const ifP);

void pickTheBitsOutOfThis(unsigned char item, int validBits, int bitNdx, PBitMapRow& pr);

#endif /*_PBM_H_*/

