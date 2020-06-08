/*


   rddstr.h				Read Data String Header File
					Copyright (C) 1989, by ==C=R=C==	


*/

#define		N_AMAX		4	/* max number of string arrays */
#define		N_SMAX		32	/* max number of strings per array */

static struct rdstrdta {
	int ifh ;
	short unsigned magic, border ;
	short unsigned num, spos, swap, nopen ;
	short unsigned dim[N_AMAX] ;
	short unsigned data[(N_AMAX*N_SMAX)+1] ;
} ;
