//	Revision:      		7

#ifndef		M_STRLST_H_INC
#define		M_STRLST_H_INC

/*

	mstrlst.h		qad-NewARC21 C++ Header: "M_StrLst" class
				Copyright (C) 1995, Contahal Ltd.


*/

/*
	sub-#includes
*/

#include	"mstr.h"
#include	"mpary.h"

/*
	class definitions
*/

class M_StrLst : public M_Obj
{
	public:
		~M_StrLst() ;
		M_StrLst() ;
		M_StrLst(const char *plist) ;
		M_StrLst(const M_StrLst&) ;		  // copy operator
		M_StrLst& operator= (const M_StrLst&) ; // assignment operator
	
		int Length() const ;
		M_SBOOL getstr(int index, M_Str& str) const ;
		void addwords(const char *) ;
		M_SINT addstr(M_Str *) ;
		M_SINT Add(const char *) ;
		void Clear() ;

		const M_Str& operator[](int index) const ;

	private:
		M_POAry m_poas ;
} ;

/*
	methods implemented in-line
*/

inline M_StrLst::~M_StrLst() { Clear() ; }
inline M_StrLst::M_StrLst() {}
inline M_StrLst::M_StrLst(const char *plist) { addwords(plist) ; }
inline M_SINT M_StrLst::Add(const char *pszStr) {
	return(addstr(new M_Str(pszStr))) ;
}
inline int M_StrLst::Length() const { return(m_poas.length()) ; }
inline const M_Str& M_StrLst::operator[](int index) const {
	return(*((M_Str *) m_poas[index])) ;
}

// #ifndef M_STRLST_H_INC
#endif

// end of mstrlst.h
