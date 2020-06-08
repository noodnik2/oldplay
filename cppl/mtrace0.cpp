#define	Revision		7

/*

	otrace0.cpp		qad-NewARC21 C++ Source: "M_Trace" utility
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/

#include	"mtrace.h"

#ifndef	M_NOTRACE

M_TraceMgr *M_TraceMgr::GetGlobalObject()
{
	static M_TraceMgr otm(
		"otrace.log",
		A21LF_REOPEN,		// A21LF_NONE,
		M_TF_ALL,
		15
	) ;
	return(&otm) ;
}

// #ifndef M_NOTRACE
#endif

// end of otrace0.cpp
