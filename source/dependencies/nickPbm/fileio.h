#ifndef _NETPBM_FILEIO_H_
#define _NETPBM_FILEIO_H_

/*
Heavily hacked NickJ June 2016
*/

#include <stdio.h>

char
pm_getc(FILE * const file);

unsigned char
pm_getrawbyte(FILE * const file);

unsigned int
pm_getuint(FILE * const file);

#endif
