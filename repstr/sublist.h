/*      Revision:       3
*/

/*
	sublist.h
*/

#include        "tsub.h"

#define SUBSTRMAX_LENGTH        256

typedef struct tag_SUBLISTITEM {
        TSSTR sFrom ;
        TSSTR sTo ;
} SUBLISTITEM ;

int getsublist(SUBLISTITEM *psli, int maxitems, char *pszFileName) ;
void freesublist(SUBLISTITEM *psli, int nitems) ;

/* end of sublist.h */
