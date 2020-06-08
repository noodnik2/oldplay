//	Revision:      		5

/*

	ostrlst.cpp			qad-ARC21 C++/String List Class
					Copyright (C) 1995, Contahal Ltd.


*/

#include	"compiler.h"
#include	"mstrlst.h"
#include	<ctype.h>

void M_StrLst::Clear()
{
	m_poas.resetAndFreeContents() ;
}

M_SINT M_StrLst::addstr(M_Str* pstr)
{
	return(m_poas.addentry(pstr)) ;
}

void M_StrLst::addwords(const char *pwords)
{
	int l ;

	while((*pwords) != '\0') {
		for (l= 0; pwords[l] != '\0'; l++) {
			if (!isspace(pwords[l])) break ;
		}
		pwords+= l ;
		for (l= 0; pwords[l] != '\0'; l++) {
			if (isspace(pwords[l])) break ;
		}
		if (l> 0) addstr(new M_Str(pwords, l)) ;
		pwords+= l ;
	}
}

M_SBOOL M_StrLst::getstr(int index, M_Str& str) const
{
	if ((index< 0) || (index>= Length())) return(M_CFALSE) ;
	str= *((M_Str *) m_poas[index]) ;
	return(M_CTRUE) ;
}

// end of ostrlst.cpp
