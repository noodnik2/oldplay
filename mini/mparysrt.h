//	Revision:      		16

#ifndef	M_PARYSRT_H_INC
#define	M_PARYSRT_H_INC

/*

	mparysrt.h

*/

/*
	sub-#includes
*/
#include	"mpary.h"

/*
	Class declarations
*/

class M_POArySrt : public M_POAry
{
	M_CLSDEFS() ;

	public:
		~M_POArySrt() ;
		M_POArySrt() ;
		M_POArySrt(M_PO po) ;
		M_POArySrt(const M_PO *m_poa) ;
		M_POArySrt(const M_PO *m_poa, int len) ;
		M_POArySrt(const M_POArySrt &oa) ;

		void sort() ;
		void sort(M_METHODCMP omc) ;

		void add(const M_PO *m_poa, int len) ;
		int addentry(M_PO po) ;

		M_SBOOL isSorted() const ;
		M_METHODCMP getSortMethod() const ;

		int search(M_PCO pco) ;
	
	private:
		M_METHODCMP m_omc ;

} ;

inline void M_POArySrt::sort() { sort((M_METHODCMP) &M_Obj::compare) ; }
inline M_METHODCMP M_POArySrt::getSortMethod() const { return(m_omc) ; }

inline M_SBOOL M_POArySrt::isSorted() const {
	return((M_SBOOL) (getSortMethod() != (M_METHODCMP) 0)) ;
}

// #ifndef M_PARYSRT_H_INC
#endif

// end of mparysrt.h
