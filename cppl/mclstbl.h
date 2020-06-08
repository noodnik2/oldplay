//	Revision:		12

#ifndef	M_CLSTBL_H_INC
#define	M_CLSTBL_H_INC

/*

	mclstbl.h		qad-NewARC21 C++ Header: "M_ClsTbl" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	sub-#includes
*/
#include	"mcls.h"

/*
	macro #definitions
*/
#define	M_CLSTBLMAX_CLASSES		256
#define	M_CLSTBLGLOBAL_PTR		M_ClsTbl::pclstbl()

/*
	Class declarations
*/

class M_ClsTbl
{
	public:
		~M_ClsTbl() ;
		M_ClsTbl() ;
		void add(M_Cls *) ;		// adds M_Cls to table
		M_Cls *of(const char *) const ;	// find M_Cls by name
		static M_ClsTbl *pclstbl() ;	// get ptr to global clstbl

	private:
		int m_nClasses ;
		int m_maxClasses ;
		M_Cls **m_ppClasses ;
} ;

// #ifndef M_CLSTBL_H_INC
#endif

// end of mclstbl.h
