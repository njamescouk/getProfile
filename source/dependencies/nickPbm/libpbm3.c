/* libpbm3.c - pbm utility library part 3
**
** Copyright (C) 1988 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.

Heavily hacked NickJ June 2016

*/
#include <assert.h>

#include "pbm.h"
#include "PBitMapRow.h"
#include "PBitMap.h"

#define HAVE_MMX_SSE 0

void
pbm_writepbminit(FILE * const fileP, 
                 int    const cols, 
                 int    const rows) {

        fprintf(fileP, "%c%c\n%d %d\n", PBM_MAGIC1, PBM_MAGIC2, cols, rows);
}

void
writePbmRowPlain(FILE * const fileP, PBitMapRow r) {
    
    int col, charcount;

    charcount = 0;
    for (col = 0; col < r.getWidth(); ++col) {
        if (charcount >= 70) {
            putc('\n', fileP);
            charcount = 0;
        }
        PbmColour px = r[col];
        px.write(fileP);
        ++charcount;
    }
    putc('\n', fileP);
}

void
pbm_writepbmrow(FILE * const fileP, PBitMapRow r) {

    writePbmRowPlain(fileP, r);
}

void
pbm_writepbm(FILE * const fileP, PBitMap *pBitMap) {
    
    pbm_writepbminit(fileP, pBitMap->getWidth(), pBitMap->getHeight());
    
    int row;
    for (row = 0; row < pBitMap->getHeight(); ++row)
    {
        PBitMapRow r = pBitMap->getRow(row);
        pbm_writepbmrow(fileP, r);
    }
}
