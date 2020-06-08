//	Revision:      		9

#ifndef	M_PARYORD_H_INC
#define	M_PARYORD_H_INC

/*

	mparyord.h	 	qad-NewARC21 C++ Header: "M_POAryOrd" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	sub-#includes
*/
#include	"mpary.h"

/*
	#defines
*/
#define	M_POAODMC (M_METHODCMP)&M_Obj::compare	// default compare method

/*
	Class declarations
*/

class M_POAryOrd : public M_POAry
{
	M_CLSDEFS() ;

	public:
		~M_POAryOrd() ;
		M_POAryOrd() ;
		M_POAryOrd(M_METHODCMP omc) ;
		M_POAryOrd(M_PO po) ;
		M_POAryOrd(M_METHODCMP omc, M_PO po) ;
		M_POAryOrd(const M_PO *m_poa) ;
		M_POAryOrd(M_METHODCMP omc, const M_PO *m_poa) ;
		M_POAryOrd(const M_PO *m_poa, int len) ;
		M_POAryOrd(M_METHODCMP omc, const M_PO *m_poa, int len) ;
		M_POAryOrd(const M_POAryOrd &oa) ;
		M_POAryOrd(M_METHODCMP omc, const M_POAry &oa) ;

		int addentry(M_PO po) ;
		void add(const M_PO *m_poa, int len) ;

		M_SBOOL isSorted() const ;
		M_METHODCMP getSortMethod() const ;

		int search(M_PCO pco) ;

	private:
		int m_i ;		// current pointer index
		M_METHODCMP m_omc ;	// current object sort order
} ;

/*
	methods implemented inline
*/

inline M_METHODCMP M_POAryOrd::getSortMethod() const { return(m_omc) ; }
inline M_SBOOL M_POAryOrd::isSorted() const { return(M_CTRUE) ; }

// #ifndef M_PARYORD_H_INC
#endif

// end of mparyord.h
