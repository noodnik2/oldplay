#define	Revision       		21
#define	M_TRACE_MODULEID	"mclstbl"

/*

	mclstbl.cpp		qad-NewARC21 C++ Source: "M_ClsTbl" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/

#include	"mclstbl.h"
#include	"mthrow.h"
#include	<string.h>		

/*
	internal module data
*/

static M_Cls *ppClasses[M_CLSTBLMAX_CLASSES] ;

/*
	Class methods
*/

M_ClsTbl::~M_ClsTbl()
{
	// nothing to do
}

M_ClsTbl::M_ClsTbl()
:
	m_maxClasses(M_CLSTBLMAX_CLASSES),
	m_ppClasses(ppClasses),
	m_nClasses(0)
{
	// nothing to do
}

M_Cls *M_ClsTbl::of(const char *pszClsName) const
{
	for (int i= 0; i< m_nClasses; i++) {
		if (strcmp(pszClsName, m_ppClasses[i]->classname()) == 0) {
			return(m_ppClasses[i]) ;
		}
	}
	return((M_Cls *) 0) ;
}

void M_ClsTbl::add(M_Cls *pcls)
{
	if (m_nClasses>= m_maxClasses) {
		M_Throw::FatalError("oclstbl: too many classes") ;
		/*NOTREACHED*/
		return ;
	}
	m_ppClasses[m_nClasses++]= pcls ; 
}

M_ClsTbl *M_ClsTbl::pclstbl()
{
	static M_ClsTbl ct ;
	return(&ct) ;
}

// end of mclstbl.cpp
