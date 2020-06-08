/*

   mkdstr.h				Make Data String Header File
					Copyright (C) 1989, by ==C=R=C==	


*/

#define		MAXA_SIZE	4096		/* max string data area  */
#define		MAXA_DIM	32		/* max number of strings */
#define		MAXN_ARRAY	4		/* max number of arrays  */

struct mkdopt {
	int debug ;
	int help ;
} ;

struct mkddta {
	int ofh ;
	unsigned short magic ;
	unsigned short num ;
	unsigned short dim[MAXN_ARRAY] ;
	unsigned char *data[MAXN_ARRAY][MAXA_DIM] ;
	unsigned char *heap, *free ;
} ;
