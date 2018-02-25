// pclYaccfns.h
#ifndef have_pclYaccfns
#define have_pclYaccfns

extern int pcllineno;
#include <stdio.h>

/* C++ wants prototypes, you 'just know' the declarations */
int pcllex();
int pclparse();
void pclerror (char *s);

void pclScanString(const char *s);

extern FILE *pclin, *pclout;
extern int pcldebug;
extern char *pcltext;

// #define YYDEBUG 1
#endif // have_pclYaccfns
