//	Revision:		62

#ifndef	M_STR_H_INC
#define	M_STR_H_INC

/*

	mstr.h			qad-NewARC21 C++ Header: "M_Str" class
				Copyright (C) 1995, Contahal Ltd.

*/

/*
	sub-#includes
*/
#include	"mobj.h"
#include	<memory.h>

/*
	#definitions
*/
#define	M_STRMAXLEN_PRINTS		255
#define	M_STRCHAR_SPACE			32
#define	M_STRDATA_PNIL			M_StrData_Nil()
#define	M_STRFMT_LONG			"%ld"
#define	M_STRFMT_ULONG			"%lu"
#define	M_STRFMT_DOUBLE			"%lf"


/*
	Class declarations
*/

/*
	Class "M_StrData"
*/

class M_StrData
{
	public:
		~M_StrData() ;
		M_StrData() ;
		int length() const ;
		int allocated() const ;
		const char *gets() const ;
		static M_StrData *alloc() ;
		static M_StrData *alloc(short unsigned nalloc) ;
		void freeSelf() ;
		int load(const char *psz, int len) ;
		int load(const M_StrData *pd) ;
		int append(const char *psz, int len) ;
		int append(const M_StrData *pd) ;
		void toUpper() ;
		void toLower() ;
		M_SBOOL isEmpty() const ;

	private:
		M_StrData(short unsigned) ;
		void *operator new(size_t, short unsigned) ;

		unsigned short m_nlen ;	   // actual length of string
		unsigned short m_nalloc ;  // # of allocated bytes
		char m_sz[1] ;		   // string starts here, ends w/'\0'
} ;

extern M_StrData *M_STRDATA_PNIL ;
					      
inline M_StrData::~M_StrData() {}
inline M_StrData *M_StrData::alloc() { return(M_STRDATA_PNIL) ; }
inline int M_StrData::length() const { return(m_nlen) ; }
inline int M_StrData::allocated() const { return(m_nalloc) ; }
inline const char *M_StrData::gets() const { return(&m_sz[0]) ; }

/*
	Class "M_Str"
*/

class M_Str : public M_Obj
{
	M_CLSDEFS() ;

	public:

		~M_Str() ;
		M_Str() ;
		M_Str(long l, const char *pcszFmt= M_STRFMT_LONG) ;
		M_Str(unsigned long ul, const char *pcszFmt= M_STRFMT_ULONG) ;
		M_Str(double d, const char *pcszFmt= M_STRFMT_DOUBLE) ;
		M_Str(const char *psz) ;
		M_Str(const char *psz, int len) ;
		M_Str(const M_Str &os) ;

				      	// base class method replacements
		void reset() ;
		int compare(M_PCO pco) const ;

					// replaced operators
		int operator == (const char *psz) const ;
		int operator != (const char *psz) const ;
		int operator >  (const char *psz) const ;
		int operator <  (const char *psz) const ;
		int operator >= (const char *psz) const ;
		int operator <= (const char *psz) const ;
		
					// new class methods
		void shrink() ;
		void expand(int n) ;
		void realloc(int n) ;
		char charAt(int idx) const ;
		M_SBOOL isEmpty() const ;
		int length() const ;
		int allocated() const ;
		void toUpper() ;		// uppercase in-place
		void toLower() ;		// lowercase in-place

		M_SBOOL charIn(char c) const ;
		int compareCStr(const char *psz) const ;

		const char *gets() const ;   	// return ptr to string
		char *gets(char *psz) const ;	// copy string into *psz
		char *gets(char *psz, int maxlen) const ;

		int geti() const ;
		unsigned int getui() const ;
		long getl() const ;
		unsigned long getul() const ;
		double getd() const ;
	
		int putd(double d, const char *pcszFmt= M_STRFMT_DOUBLE) ;
		int putl(long l, const char *pcszFmt= M_STRFMT_LONG) ;
		int putul(
			unsigned long l,
			const char *pcszFmt= M_STRFMT_ULONG
		) ;

		int puts(const char *psz) ;
		int puts(const M_Str *pos) ;

		int prints(const char *pszFormat, ...) ;
		int prints_append(const char *pszFormat, ...) ;

		int append(const char *psz, int len) ;
		int append(const char *psz) ;
		int append(const M_Str *pos) ;

						// new operators
		int operator !() const ;
		char operator[](int idx) const ;

		M_Str &operator+= (const M_Str &os) ;
		M_Str &operator+= (const char *psz) ;

		M_Str& operator= (const M_Str &os) ;
		M_Str& operator= (const char *psz) ;

		operator const char *() const ;
		operator int() const ;
		operator unsigned int() const ;
		operator long() const ;
		operator unsigned long() const ;
		operator double() const ;

		M_Obj &Clone() const ;			// new: 11.6.95

	private:
		int construct(const char *psz) ;
		int construct(const char *psz, int len) ;
		int reconstruct(const char *psz) ;
		int reconstruct(const char *psz, int len) ;
		void M_Str::reload(const char *psz) ;

		M_StrData *m_pd ;
} ;

inline M_Str::~M_Str() { M_Str::reset() ; }
inline M_Str::M_Str() : m_pd(M_STRDATA_PNIL) {}
inline M_Str::M_Str(const char *psz) { construct(psz) ; }
inline M_Str::M_Str(const char *psz, int len) { construct(psz, len) ; }
inline M_Str::M_Str(const M_Str &os) { construct(os.gets(), os.length()) ; }

inline M_Str::M_Str(long l, const char *pcszFmt)
	: m_pd(M_STRDATA_PNIL)
{
	putl(l, pcszFmt) ;
}

inline M_Str::M_Str(unsigned long ul, const char *pcszFmt)
	: m_pd(M_STRDATA_PNIL)
{
	putul(ul, pcszFmt) ;
}

inline M_Str::M_Str(double d, const char *pcszFmt)
	: m_pd(M_STRDATA_PNIL)
{
	putd(d, pcszFmt) ;
}

inline void M_Str::shrink() { realloc(m_pd->length()) ; }
inline void M_Str::expand(int n) { realloc(m_pd->allocated() + n) ; }

inline void M_Str::toUpper() { m_pd->toUpper() ; }
inline void M_Str::toLower() { m_pd->toLower() ; }

inline char M_Str::charAt(int idx) const {
	if ((idx< 0) || (idx>= length())) return(0) ;
	return((gets())[idx]) ;
}

inline M_SBOOL M_Str::isEmpty() const { return(m_pd->isEmpty()) ; }
inline const char *M_Str::gets() const { return(m_pd->gets()) ; }

inline int M_Str::puts(const char *psz) { return(reconstruct(psz)) ; }

inline int M_Str::putd(double d, const char *pszFmt) {
	return(prints(pszFmt, d)) ;
}

inline int M_Str::putl(long l, const char *pszFmt) {
	return(prints(pszFmt, l)) ;
}

inline int M_Str::putul(unsigned long ul, const char *pszFmt) {
	return(prints(pszFmt, ul)) ;
}
	
inline int M_Str::allocated() const { return(m_pd->allocated()) ; }
inline int M_Str::length() const { return(m_pd->length()) ; }

inline int M_Str::append(const M_Str *pos) {
	return(append(pos->gets(), pos->length())) ;
}

inline int M_Str::operator == (const char *psz) const {
	return(compareCStr(psz) == 0) ;
}
	
inline int M_Str::operator != (const char *psz) const {
	return(compareCStr(psz) != 0) ;
}

inline int M_Str::operator >  (const char *psz) const {
	return(compareCStr(psz) > 0) ;
}

inline int M_Str::operator <  (const char *psz) const {
	return(compareCStr(psz) < 0) ;
}

inline int M_Str::operator >= (const char *psz) const {
	return(compareCStr(psz) >= 0) ;
}

inline int M_Str::operator <= (const char *psz) const {
	return(compareCStr(psz) <= 0) ;
}

inline char M_Str::operator[](int idx) const { return(charAt(idx)) ; }

inline M_Str &M_Str::operator+= (const M_Str &os) {
	append(&os) ;
	return(*this) ;
}

inline M_Str &M_Str::operator+= (const char *psz) {
	append(psz) ;
	return(*this) ;
}

inline M_Str& M_Str::operator= (const M_Str &os) {
	if (&os != this) reconstruct(os.gets(), os.length()) ;
	return(*this) ;
}

inline M_Str& M_Str::operator= (const char *psz) {
	reconstruct(psz) ;
	return(*this) ;
}

inline int M_Str::operator !() const { return(isEmpty()) ; }
inline M_Str::operator int() const { return(geti()) ; }
inline M_Str::operator unsigned int() const { return(getui()) ; }
inline M_Str::operator long() const { return(getl()) ; }
inline M_Str::operator unsigned long() const { return(getul()) ; }
inline M_Str::operator double() const { return(getd()) ; }
inline M_Str::operator const char *() const { return(gets()) ; }

/*
	global function declarations
*/
M_Str substr(const M_Str& os1, int offset, int len= 0) ;

/*
	global inline function definitions
*/

inline M_Str toUpper(const M_Str& os1) {
	M_Str os(os1) ;
	os.toUpper() ;
	return(os) ;
}

inline M_Str toLower(const M_Str& os1) {
	M_Str os(os1) ;
	os.toUpper() ;
	return(os) ;
}

inline M_Str concat(const M_Str &os1, const M_Str &os2) {
	M_Str os(os1) ;
	os.append(os2) ;
	return(os) ;
}

inline M_Str concat(const char *psz, const M_Str &os2) {
	M_Str os(psz) ;
	os.append(os2) ;
	return(os) ;
}

inline M_Str concat(const M_Str &os1, const char *psz) {
	M_Str os(os1) ;
	os.append(psz) ;
	return(os) ;
}

inline M_Str operator+ (const M_Str &os1, const M_Str &os2) {
	return(concat(os1, os2)) ;
}

inline M_Str operator+ (const char *psz, const M_Str &os2) {
	return(concat(psz, os2)) ;
}

inline M_Str operator+ (const M_Str &os1, const char *psz) {
	return(concat(os1, psz)) ;
}

// #ifndef M_STR_H_INC
#endif

// end of mstr.h
