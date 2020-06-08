#define	Revision       		39

/*

	ostr.cpp

*/

/*
	#includes
*/
#include	"mstr.h"

#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>

/*
	Class global declaration
*/
M_CLSDECLARE_DERIVEDWITHDEFCTOR(M_Str, M_Obj) ;

/*
	module "internal data"
*/

M_StrData *M_STRDATA_PNIL
{
	static M_StrData ostr_nil ;
	return(&ostr_nil) ;
}

/*
	Class methods
*/

/*
	Class:	M_StrData
*/
M_StrData::M_StrData()
:	m_nlen(0),
	m_nalloc(0)
{
	m_sz[0]= '\0' ;
}

M_StrData::M_StrData(short unsigned nalloc)
:	m_nlen(0),
	m_nalloc(nalloc)
{
	m_sz[0]= '\0' ;
}

void *M_StrData::operator new(size_t baseSize, short unsigned nalloc)
{
	return new char[baseSize + nalloc] ;
}

void M_StrData::freeSelf()
{
	if (this != M_STRDATA_PNIL) delete this ;
}

M_StrData *M_StrData::alloc(short unsigned nalloc)
{
	return(new(nalloc) M_StrData(nalloc)) ;
}

// append given string to M_StrData object: return # bytes appended
int M_StrData::append(const char *psz, int len)
{
	if ((m_nlen + len)> m_nalloc) len= m_nalloc - m_nlen ;
	if (len<= 0) return(0) ;

	int i ;
	char *pszend= &m_sz[m_nlen] ;
	for (i= 0; (i< len) && (psz[i]); i++) {
		pszend[i]= psz[i] ;
	}
	while(i< len) {
		pszend[i++]= M_STRCHAR_SPACE ;
	}
	m_nlen+= len ;
	m_sz[m_nlen]= '\0' ;
	return(len) ;
}

int M_StrData::load(const char *psz, int len)
{
	m_nlen= 0 ;
	return(append(psz, len)) ;
}

int M_StrData::load(const M_StrData *pd)
{
	return(load(pd->gets(), pd->length())) ;
}

int M_StrData::append(const M_StrData *pd)
{
	return(append(pd->gets(), pd->length())) ;
}

void M_StrData::toUpper()
{
	int l= (int) m_nlen ;
	for (int i= 0; i< l; i++) {
		m_sz[i]= toupper(m_sz[i]) ;
	}
}

void M_StrData::toLower()
{
	int l= (int) m_nlen ;
	for (int i= 0; i< l; i++) {
		m_sz[i]= tolower(m_sz[i]) ;
	}
}

M_SBOOL M_StrData::isEmpty() const
{
	int l= (int) m_nlen ;
	for (int i= 0; i< l; i++) {
		if (m_sz[i] != M_STRCHAR_SPACE) {
			return(M_CFALSE) ;
		}
	}
	return(M_CTRUE) ;
}

/*
	Class:	M_Str
*/

int M_Str::compare(M_PCO pco) const
{
	// NOTE: assumes object is M_Str or derived from M_Str
	return(compareCStr(((const M_Str *)pco)->gets())) ;
}

int M_Str::construct(const char *psz, int len)
{
	if (!psz || (len<= 0)) {
		m_pd= M_StrData::alloc() ;
	}
	else {
		m_pd= M_StrData::alloc(len) ;
		m_pd->load(psz, len) ;
	}
	return(length()) ;
}

int M_Str::construct(const char *psz)
{
	if (!psz) return(construct(0, 0)) ;
	return(construct(psz, strlen(psz))) ;
}

int M_Str::reconstruct(const char *psz, int len)
{
	if (!psz || (len<= 0)) {
		M_Str::reset() ;
	}
	else {
		if (len<= m_pd->length()) {
			m_pd->load(psz, len) ;
		}
		else {
			M_Str::reset() ;
			construct(psz, len) ;
		}
	}
	return(length()) ;
}

int M_Str::reconstruct(const char *psz)
{
	if (!psz) return(reconstruct(0, 0)) ;
	return(reconstruct(psz, strlen(psz))) ;
}

void M_Str::reset()
{
	m_pd->freeSelf() ;
	m_pd= M_StrData::alloc() ;
}

void M_Str::realloc(int n)
{
	if (n<= m_pd->length()) return ;		// won't hurt value
	M_StrData *pd= M_StrData::alloc(n) ;
	pd->load(m_pd) ;
	m_pd->freeSelf() ;
	m_pd= pd ;
}

char *M_Str::gets(char *psz) const
{
	strcpy(psz, m_pd->gets()) ;
	return(psz) ;
}

char *M_Str::gets(char *psz, int len) const
{
	strncpy(psz, m_pd->gets(), len) ;
	return(psz) ;
}

int M_Str::puts(const M_Str *pos)
{
	return(reconstruct(pos->gets(), pos->length())) ;
}

int M_Str::geti() const
{
	return(atoi(gets())) ;
}

unsigned int M_Str::getui() const
{
	return((unsigned int) atoi(gets())) ;
}

long M_Str::getl() const
{
	return(atol(gets())) ;
}

unsigned long M_Str::getul() const
{
	return((unsigned long) atol(gets())) ;
}

double M_Str::getd() const
{
	return(atof(gets())) ;
}

M_SBOOL M_Str::charIn(char c) const
{
	return((M_SBOOL) (strchr(gets(), c) != 0)) ;
}

int M_Str::compareCStr(const char *psz) const
{
	return(strcmp(m_pd->gets(), psz)) ;
}

int M_Str::append(const char *psz, int len)
{
	if (!psz || (len<= 0)) return(0) ;
	if ((length() + len)> allocated()) expand(len) ;
	return(m_pd->append(psz, len)) ;
}

int M_Str::append(const char *psz)
{
	if (!psz) return(append(0, 0)) ;
	return(append(psz, strlen(psz))) ;
}

int M_Str::prints(const char *pszFormat, ...)
{
	va_list parms ;
	va_start(parms, pszFormat) ;
	char *pbuf= new char[M_STRMAXLEN_PRINTS+1] ;
	vsprintf(pbuf, pszFormat, parms) ;
	int nlen= reconstruct(pbuf) ;
	delete [] pbuf ;
	return(nlen) ;
}

int M_Str::prints_append(const char *pszFormat, ...)
{
	va_list parms ;
	va_start(parms, pszFormat) ;
	char *pbuf= new char[M_STRMAXLEN_PRINTS+1] ;
	vsprintf(pbuf, pszFormat, parms) ;
	int nlen= append(pbuf) ;
	delete [] pbuf ;
	return(nlen) ;
}

M_Obj &M_Str::Clone() const
{
	M_Str *pNewStr= new M_Str(*this) ;
	return(*pNewStr) ;
}

/*
	global functions
*/
#if 1
M_Str substr(const M_Str &os1, int offset, int len)
{
	M_Str r ;
	int olen= os1.length() ;
	if (offset< olen) {
		if ((len == 0) || (len> (olen - offset))) {
			len= olen - offset ;
		}
		r= M_Str(os1.gets() + offset, len) ;
	}
	return(r) ;
}
#else
M_Str substr(const M_Str &os1, int offset, int len)
{
	int olen= os1.length() ;
	if (offset>= olen) return(M_Str()) ;
	if ((len == 0) || (len> (olen - offset))) {
		len= olen - offset ;
	}
	return(M_Str(os1.gets() + offset, len)) ;
}
#endif

// end of ostr.cpp
