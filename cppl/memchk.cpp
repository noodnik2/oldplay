// Revision:	       		14

/*

	memchk.cpp	qad-NewARC21/C++ Source: overloaded new and delete
			Copyright (C) 1995, Contahal Ltd.
			Written by Amir, updated by Marty

	Used to check for memory leaks

*/

// #include	"memchk.h"

/*
	stuff that belongs in memchk.h
*/

/*
	sub-#includes
*/

#include <stdio.h>
#include <process.h>
#include <malloc.h>

/*
	#defines
*/

#define SIGNATURE	50398585L
#define MAXMEM		100000L
#define FILENAME	"memchk.log"
#define	ABORTFP		stderr

#if 0
#define	FFLUSH(x)	fflush(x)	// can remove if don't want
#else
#define	FFLUSH(x)
#endif

/*
	utility class declarations
*/

class newInfo
{
	public:
		~newInfo() ;
		newInfo(size_t) ;
		size_t Size() ;
		void *pUser() ;
		void *operator new(size_t, size_t) ;
		void operator delete(void *) ;
		newInfo *&pNext() ;
		newInfo *&pPrev() ;
		void UnLink() ;
		static newInfo *pUser2Info(void *) ;

	private:
		long m_signature;
		size_t m_size;
		newInfo *m_pNext ;
		newInfo *m_pPrev ;
} ;

class infoList
{
	public:
		~infoList() ;
		infoList() ;
		void Add(newInfo *p) ;
		void Remove(newInfo *p) ;
		void Dump(FILE *fp) ;

	private:
		newInfo *m_pHead ;
		newInfo *m_pTail ;
		unsigned m_count ;
} ;

class logFile
{
	public:
		~logFile() ;
		logFile() ;
		long Total() const ;
		void Grow(size_t) ;
		void Shrink(size_t) ;
		FILE *fp() const ;
		infoList &List() ;

	private:
		FILE *m_fp;
		long m_accumulative;
		infoList m_list ;
} ;

/*
	methods implemented in-line
*/

inline newInfo::~newInfo() { m_signature= 0 ; }
inline newInfo::newInfo(size_t user)
:	m_signature(SIGNATURE),
	m_size(user),
	m_pNext(0),
	m_pPrev(0)
{}
inline void *newInfo::pUser() { return(this + 1) ; }
inline size_t newInfo::Size() { return(m_size) ; }
inline void newInfo::operator delete(void *pUser) { free(pUser) ; }
inline void *newInfo::operator new(size_t info, size_t user) {
	return malloc(info + user) ;
}
inline newInfo *newInfo::pUser2Info(void *pUser) {
	newInfo *pInfo = ((newInfo *) pUser) - 1;
	if (pInfo->m_signature != SIGNATURE) return(0) ;
	return(pInfo) ;
}
inline void newInfo::UnLink() {
	if (m_pPrev) m_pPrev->m_pNext= m_pNext ;
	if (m_pNext) m_pNext->m_pPrev= m_pPrev ;
}
inline newInfo *&newInfo::pNext() { return(m_pNext) ; }
inline newInfo *&newInfo::pPrev() { return(m_pPrev) ; }

inline infoList::~infoList() {}
inline infoList::infoList() : m_pHead(0), m_pTail(0), m_count(0) {}

inline void logFile::Grow(size_t amount) { m_accumulative+= amount ; }
inline void logFile::Shrink(size_t amount) { m_accumulative-= amount ; }
inline long logFile::Total() const { return(m_accumulative) ; }
inline FILE *logFile::fp() const { return(m_fp) ; }
inline infoList &logFile::List() { return(m_list) ; }

/*
	end of stuff that belongs in memchk.h
*/

/*
	forward references
*/

static void stopRun() ;
static void dumpUser(FILE *fp, void *p, size_t) ;
static void logRec(FILE *, const char *, newInfo *) ;

/*
	utility class implementations
*/

void infoList::Add(newInfo *pInfo)
{
	if (!m_pHead) m_pHead= pInfo ;
	pInfo->pPrev()= m_pTail ;
	if (m_pTail) m_pTail->pNext()= pInfo ;
	m_pTail= pInfo ;
	m_count++ ;
}

void infoList::Remove(newInfo *pInfo)
{
	pInfo->UnLink() ;
	if (pInfo == m_pHead) m_pHead= pInfo->pNext() ;
	if (pInfo == m_pTail) m_pTail= pInfo->pPrev() ;
	m_count-- ;
} ;

void infoList::Dump(FILE *fp)
{
	fprintf(fp, "%d items remaining in list:\n", m_count) ;
	for (newInfo *pInfo= m_pHead; pInfo; pInfo= pInfo->pNext()) {
		dumpUser(fp, pInfo->pUser(), pInfo->Size()) ;
	}
}

logFile::~logFile()
{
	fprintf(m_fp, "accumulative=%ld\n", m_accumulative) ;
	m_list.Dump(m_fp) ;
	fclose(m_fp) ;
}

logFile::logFile() : m_accumulative(0)
{
	m_fp = fopen(FILENAME, "w");
	if (!m_fp) {
		fprintf(ABORTFP, "Can't open output file %s\n", FILENAME) ;
		stopRun() ;
		/*NOTREACHED*/
	}
}

/*
	log file access
*/

static logFile &file()
{
	static logFile f ;
	return(f) ;
}

/*
	overloaded global 'new' and 'delete' operators
*/

void *operator new(size_t size)
{
	FILE *fp = file().fp() ;
	file().Grow(size) ;
	if (file().Total() > MAXMEM) {
		fprintf(fp, "new: attempt to allocate more than\n") ;
		fprintf(fp, "new: allowed maximum of %ld.\n", MAXMEM) ;
		FFLUSH(fp) ;
		stopRun() ;
		/*NOTREACHED*/
	}
	newInfo *pInfo= new(size) newInfo(size) ;  // operator newInfo::new
	if (!pInfo) {
		fprintf(fp, "new: failed; size=%lu\n", (long) size) ;
		FFLUSH(fp) ;
		stopRun() ;
		/*NOTREACHED*/
	}
	logRec(fp, "new", pInfo) ;
	file().List().Add(pInfo) ;
	return(pInfo->pUser()) ;
}

void operator delete(void *pUser)
{
	if (!pUser) return ;
	FILE *fp = file().fp() ;
	newInfo *pInfo= newInfo::pUser2Info(pUser) ;
	if (!pInfo) {
		fprintf(fp, "delete: %lX -- invalid ptr\n", (long) pUser) ;
		FFLUSH(fp) ;
		stopRun() ;
		/*NOTREACHED*/
	}
	logRec(fp, "del", pInfo) ;
	file().Shrink(pInfo->Size()) ;
	file().List().Remove(pInfo) ;
	delete pInfo ;			// operator newInfo::delete
}

/*
	utility functions
*/

static void stopRun()
{
	fprintf(ABORTFP, "newdelet: program aborted due to error\n") ;
	fprintf(ABORTFP, "newdelet: see log file: %s\n", FILENAME) ;
	exit(1);
}

static void logRec(FILE *fp, const char *pszId, newInfo *pInfo)
{
	fprintf(
		fp,
		"%lX\t%lu\t%s\t%lu\n",
		(long) pInfo->pUser(),
		(unsigned long) pInfo->Size(),
		(const char *) pszId,
		(unsigned long) file().Total()
	) ;
	FFLUSH(fp) ;
}

static void dumpUser(FILE *fp, void *p, size_t size)
{
	unsigned char *pc= (unsigned char *) p ;
	unsigned long lsize= (unsigned long) size ;
	fprintf(fp, "%lX(%lu): '", (long) p, (unsigned long) size) ;
	for (unsigned long i= 0; i< lsize; i++) {
		int c= pc[i] ;
		fprintf(fp, ((c>= ' ') && (c<= 126))? "%c": "\\%02X", c) ;
	}
	fprintf(fp, "'\n") ;
}

// end of memchk.cpp
