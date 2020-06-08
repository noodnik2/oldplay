#define	MSRC_Revision		1

/*

	border.c			Byte-order conversions
					Copyright (C) 1996, Pointofsale Ltd.


*/


#include	"compiler.h"
#include	"border.h"

void border_flips(short *ps)
{
	unsigned char ucx ;

	ucx= ((unsigned char *)ps)[0] ;
	((unsigned char *)ps)[0]= ((unsigned char *)ps)[1] ;
	((unsigned char *)ps)[1]= ucx ;
}

void border_flipl(long *pl)
{
	unsigned char ucx ;

	ucx= ((unsigned char *)pl)[0] ;
	((unsigned char *)pl)[0]= ((unsigned char *)pl)[3] ;
	((unsigned char *)pl)[3]= ucx ;
	ucx= ((unsigned char *)pl)[1] ;
	((unsigned char *)pl)[1]= ((unsigned char *)pl)[2] ;
	((unsigned char *)pl)[2]= ucx ;
}

/* end of border.c */
