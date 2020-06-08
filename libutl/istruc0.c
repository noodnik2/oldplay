#define	MSRC_Revision		2
#define	MSRC_DEBUG		0

/*

	istruc0.c		Structure Initialization: Init/Fini
				Copyright (C) 1996, Pointofsale Ltd.

*/


/*
	#includes
*/
#include	<stdio.h>
#include	<ctype.h>

#include	"istruc.h"

/*
	internal function prototypes
*/

/*
	public global data
*/
struct istrucField *istruc0_paFields= 0 ;
int istruc0_nFields= 0 ;


/*
	entry points
*/

ISTRUC_BOOL istruc_parseInit(
	struct istrucField *paFields_User,
	int nFields_User
) {
	int i ;

	/*
		check for invalid calling parms
	*/
	if ((paFields_User == 0) && (nFields_User != 0)) {
		return(ISTRUCBOOL_FALSE) ;	
	}

	/*
		Modify the user's "*paFields" structure,
	*/
	if (paFields_User) {
		for (i= 0; i< nFields_User; i++) {
			paFields_User[i].iFieldNameLen= strlen(
				paFields_User[i].pszFieldName
			) ;
		}
	}

	/*
		Save pointers to it.
	*/
	istruc0_paFields= paFields_User ;
	istruc0_nFields= nFields_User ;

	return(ISTRUCBOOL_TRUE) ;
}

void istruc_parseFini()
{
	istruc0_paFields= 0 ;
	istruc0_nFields= 0 ;
}

/* end of istruc0.c */
