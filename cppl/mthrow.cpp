#define	Revision       		10

/*

	othrow.cpp		qad-NewARC21 C++ Source: Class "M_Throw"
				Copyright (C) 1995, Contahal Ltd.


*/

/*
	#includes
*/

#include	"mthrow.h"
#include	<stdio.h>
#include	<stdlib.h>

void M_Throw::BeepMessage(int)
{
	fprintf(stderr, "%c", 9) ;
}

void M_Throw::DisplayMessage(const char *pszMessage, const char *pszTitle)
{
	fprintf(stderr, "%s\n%s\n", pszTitle, pszMessage) ;
}

void M_Throw::FatalError(const char *pszErrorText, const char *pszTitle)
{
	DisplayMessage(pszErrorText, pszTitle) ;
	::exit(1) ;
}

int M_Throw::YesNoQuestion(const char *pszQuestion, const char *pszTitle)
{
  	char szA[15] ;
	int bA= -1 ;
	while(bA< 0) {
		DisplayMessage(pszQuestion, pszTitle) ;
		gets(szA) ;
		if ((szA[0] == 'Y') || (szA[0] == 'y')) bA= 1 ;
		if ((szA[0] == 'N') || (szA[0] == 'n')) bA= 0 ;
	}
	return(bA) ;
}

// end of othrow.cpp
