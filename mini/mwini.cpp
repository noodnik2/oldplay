/*

	mwini.cpp

	C wrapper for C++ "initialization file" functions.

*/

#include	<stdio.h>

#include	"mini.h"
#include	"mwini.h"
#include	"mthrow.h"


/*
	internal #defines
*/

#define	IA	iaf()


/*
	internal function declarations
*/

static M_IniAccess& iaf() ;


/*
	entry points
*/

extern "C" void IniInit()
{
}

extern "C" void IniFlush()
{
	IA.flush() ;
	/* what about return code here? */
}

extern "C" void IniFini()
{
	IniFlush() ;
}

extern "C" void IniRead(
	char *fn,
	char *section,
	char *entry,
	char *output,
	int iMaxLen
) {
	M_SBOOL b ;
	M_Str sOut ;

	b= IA.getEntryValue(
		M_Str(fn),
		M_Str(section), 
		M_Str(entry), 
		sOut
	) ;
	if (!b) {
		output[0]= '\0' ;
		return ;
	}
	strcpy(output, (const char *) sOut) ;
}

extern "C" void IniWrite(
	char *fn,
	char *section,
	char *entry,
	char *input
) {
	M_SBOOL b ;

	b= IA.putEntryValue(
		M_Str(fn),
		M_Str(section),
		M_Str(entry),
		M_Str(input)
	) ;
}


/*
	"throw" entry point.
	(this was written before ANSI C++ had "throw" command; should
	probably convert to real "throw", now that ANSI C++ has this)
*/

void M_Throw::FatalError(const char *pszTitle, const char *pszText)
{
	fprintf(stderr, "fatal error: %s: %s\n", pszTitle, pszText) ;
	::exit(1) ;
}


/*
	internal functions
*/

static M_IniAccess& iaf()
{
	static M_IniAccess ia ;
	return(ia) ;
}

/* end of mwini.cpp */
