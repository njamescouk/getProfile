/* fileio.c - routines to read elements from Netpbm image files
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

#include <stdio.h>
#include <limits.h>

#include "fileio.h"

char
pm_getc(FILE * const fileP) {
    int ich;
    char ch;

    ich = getc(fileP);
    if (ich == EOF)
        fprintf(stderr, "EOF / read error reading a byte");
    ch = (char) ich;
    
    if (ch == '#') {
        do {
            ich = getc(fileP);
            if (ich == EOF)
                fprintf(stderr, "EOF / read error reading a byte");
            ch = (char) ich;
        } while (ch != '\n' && ch != '\r');
    }
    return ch;
}



unsigned int
pm_getuint(FILE * const ifP) {
/*----------------------------------------------------------------------------
   Read an unsigned integer in ASCII decimal from the file stream
   represented by 'ifP' and return its value.

   If there is nothing at the current position in the file stream that
   can be interpreted as an unsigned integer, issue an error message
   to stderr and abort the program.

   If the number at the current position in the file stream is too
   great to be represented by an 'int' (Yes, I said 'int', not
   'unsigned int'), issue an error message to stderr and abort the
   program.
-----------------------------------------------------------------------------*/
    char ch;
    unsigned int i;

    do {
        ch = pm_getc(ifP);
    } while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');

    if (ch < '0' || ch > '9')
        fprintf(stderr, "junk in file where an unsigned integer should be");

    i = 0;
    do {
        unsigned int const digitVal = ch - '0';

        if (i > INT_MAX/10)
            fprintf(stderr, "ASCII decimal integer in file is "
                     "too large to be processed.  ");
        
        i *= 10;

        if (i > INT_MAX - digitVal)
            fprintf(stderr, "ASCII decimal integer in file is "
                     "too large to be processed.  ");

        i += digitVal;

        ch = pm_getc(ifP);
    } while (ch >= '0' && ch <= '9');

    return i;
}





