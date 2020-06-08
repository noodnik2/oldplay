/*

	gistok.h

*/


/*
	#defines
*/

#define	GISTOKCH_EOF		(-1)


/*
	structs, types
*/

struct gistok_istream {
	int (*getc)(void *) ;
	void (*ungetc)(void *, int) ;
	void *pUser ;
} ;

typedef struct gistok_istream gistok_istream_t ;


/*
	entry points
*/

int gettok(char *tok, int toksize, gistok_istream_t *) ;


/* end of gistok.h */
