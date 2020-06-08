//	Revision:      		44

#ifndef		M_INI_H_INC
#define		M_INI_H_INC

#if 0
/*

	mini.h				qad-ARC21 Initialization Parameters
					Copyright (C) 1995, Contahal Ltd.


	This module defines C++ classes to access initialization files
	containing application run-time parameters.


	General:

		An .INI file consists of three types of entries:
	
			(1) comments
			(2) section-headers
			(3) entries within sections

		Accesses to .INI files are made in "sessions".  A
		cache entry for an .INI file exists while a session
		is active for the file.  The cache entry for each
		file consists of lists of accessed entries.

	Entry formats:

		All entries are stored in the cache prefixed by a
		common header structure.  The actual data for each
		entry depends on the entry type.

		Comment entries contain simple ASCIZ strings.  The
		interpretation of the strings is the actual comment
		data, exactly as it appears in the file.

		Section-headers contain ASCIZ strings containing the
		section-name.

		Entries within sections contain first a pointer to the
		section-header (defined previously) of the section to
		which the entry belongs, followed by an ASCIZ string
		containing the entry name, followed by another ASCIZ
		string containing the entry value.


	Logic:

		When a session is started, a cache entry for the .INI
		file is "checked-out" (if it already exists), or created
		(if it doesn't exist).  The general logic for accessing
		an entry in the file is given below, in pseudo-code:

		if (!(cache_ptr= get_cache(filename))) {
			cache_ptr= allocate_cache(filename) ;
		}
		if (!(entry_ptr= get_entry(cache_ptr, section, entry))) {
			if (!(section_ptr= get_section(cache_ptr, section))) {
				section_ptr= allocate_section(
					cache_ptr,
					section
				) ;
			}
			entry_ptr= allocate_entry(
				cache_ptr,
				section_ptr,
				entry
			) ;
		}

		(here we have the desired entry in the cache...)


		When an open session is closed, the cache entry
		is "released".  Releasing involves decrementing
		a usage-reference counter for the cache entry,
		and physically freeing the entry if the reference
		count becomes zero as a result of the decrement.

		In the process of physically freeing the cache entry,
		the "modified" flag of the cache entry is checked,
		and if set, then the all the entries found in the
		cache for which the "modified" bit are set are written
		to an updated version of the .INI file, which Eventually
		replaces the original.  The logic for writing out
		modified entries is the topic discussed in the next
		paragraph.

		To update an .INI file, first, a temporary output file
		is created, then the updated .INI file contents are
		written to the file, then the old file is renamed to
		.BAK (after first deleting the .BAK file that exists),
		then the new file is renamed to use the correct name.
		In order to write the updated version, the list of
		updated entries must be sorted by the 'start' field
		of the entries, and the old file is copied "as is"
		for all sections not appearing in the domain of the
		entries to be modified, and the new entries are
		written in place of old entries.
*/			
#endif

/*
	sub-#includes
*/

#include	"mstr.h"
#include	"mparysrt.h"

#include	<stdio.h>

/*
	#defines
*/

#define	M_INIFMAX_LINELENGTH	1024

#define	M_INIRC_SUCCESS		0
#define	M_INIRC_FAILURE		-1
#define	M_INIRC_NM_TIMPLEMENTED	0xE000

#define	M_INIID_NM_TFOUND	-1

#define	M_INIFAO_NONE		0		// no options
#define	M_INIFAO_BAK		1		// create .bak files

#define	M_INILFM_POS_EOF		((M_SULONG) -1)

/*
	typedefs
*/
typedef int M_IniRC ;
typedef int M_IniID ;
typedef int M_IniFAO ;

/*
	Class definitions
*/

/*
	Class M_IniFLOC			- .INI file item locations (base class)
*/

class M_IniFLOC : public M_Obj
{
	M_CLSDEFS() ;

	public:
		~M_IniFLOC() ;
		M_IniFLOC() ;
		M_IniFLOC(M_SULONG ulstart) ;
		void setFLOC(M_SULONG ulstart) ;
		M_IniFLOC(M_SULONG ulstart, M_SULONG ulnext) ;
		void setFLOC(M_SULONG ulstart, M_SULONG ulnext) ;
		M_SBOOL getFLOC(M_SULONG *pulstart, M_SULONG *pulnext) const ;
		M_SBOOL getFLOC() const ;
		int compare(M_PCO) const ;

	private:
		M_SULONG m_ulstart ;
		M_SULONG m_ulnext ;
		M_SBOOL m_bFound ;
} ;

/*
	Class M_IniSectionItem			- .INI file section items
*/

class M_IniSectionItem : public M_IniFLOC
{
	M_CLSDEFS() ;

	public:
		~M_IniSectionItem() ;
		M_IniSectionItem(const M_Str& ssection) ;
		M_IniSectionItem(const M_Str& ssection, const M_IniFLOC&) ;
		const M_Str& GetSectionName() const ;

	private:
		M_Str m_ssection ;		// section name string
} ;

/*
	Class M_IniEntryItem			- .INI file entries
*/

class M_IniEntryItem : public M_IniFLOC
{
	M_CLSDEFS() ;

	public:
		~M_IniEntryItem() ;
		M_IniEntryItem(M_IniID idsection, const M_Str& sentryname) ;
		M_IniEntryItem(
			M_IniID idsection,
			const M_Str& sentryname,
			const M_Str& sentryvalue,
			M_IniFLOC& floc
		) ;
		const M_Str& getEntryName() const ;
		M_SBOOL getEntryValue(M_Str&) const ;
		M_SBOOL putEntryValue(const M_Str&) ;
		M_IniID getIdSection() const ;
		M_SBOOL isChanged() const ;
		int compareByFLOC(M_PCO) const ;

	private:
		M_IniID m_idsection ;		// section id reference
		M_Str m_sentryname ;		// entry name string
		M_Str m_sentryvalue ;	// entry value string
		M_SBOOL m_bChanged ;		// entry was changed
		M_SBOOL m_bDefined ;		// entry has value
} ;

/*
	Class M_IniFile			- .INI file: cached entries
*/

class M_IniSectionIterator ;	// forward

class M_IniFile : public M_Obj
{
	M_CLSDEFS() ;

	friend M_IniSectionIterator ;

	public:
		~M_IniFile(void) ;
		M_IniFile(const M_Str& sfn) ;
		M_SBOOL getNextSectionName(
			M_Str &ssection,
			M_IniFLOC &floc
		) ;
		M_SBOOL getNextEntryName(
			M_Str &sentry,
			M_Str &sevalue,
			M_IniFLOC &floc
		) ;
		M_SBOOL getEntryValue(
			const M_Str& ssection,
			const M_Str& sentryname,
			M_Str& sentryvalue
		) ;
		M_SBOOL putEntryValue(
			const M_Str& ssection,
			const M_Str& sentryname,
			const M_Str& sentryvalue
		) ;
		const M_Str& getFn() const ;
		M_IniRC flush(M_IniFAO = M_INIFAO_NONE) ;
		M_SBOOL isChanged() const ;
		M_SWORD nChanges() const ;

	private:
		M_IniID findsectionid(const M_Str& ssection) const ;
		M_IniID findentryid(M_IniID idsec, const M_Str& sen) const ;
		M_IniID newsectionid(const M_Str& ssection) ;
		M_IniID cachesection(const M_Str &ssection, const M_IniFLOC &) ;
		M_IniID cachesection(const M_Str &ssection) ;

		static M_SBOOL issection(const char *fl, M_Str *pss) ;
		M_SBOOL readfilesection(M_IniFLOC *pfloc, const M_Str& ssection) ;
		static M_SBOOL isentry(
			const char *fl,
			M_Str *psen,
			M_Str *psev
		) ;
		M_SBOOL readfileentry(
			M_IniFLOC* pfloc,
			M_Str* psentryvalue,
			M_IniID idsection,
			const M_Str& sentryname
		) ;
		M_IniID newentryid(M_IniID idsection, const M_Str& sentryname) ;
		M_IniID getentryid(
			const M_Str& ssection,
			const M_Str& sentryname
		) ;
		M_IniRC mvfile(const char* pszFFn, const char* pszTFn) ;
		M_SBOOL mkfn(M_Str& sfn, const char* pszExt) ;
		M_IniRC copyfile(FILE *, M_SULONG, M_SULONG = M_INILFM_POS_EOF) ;
		M_IniRC writeentry(FILE *, M_IniEntryItem*) const ;
		void openfile() ;
		void closefile() ;
		void reset() ;

		M_Str m_sfn ;				// name of .INI file
		M_POArySrt m_IniSectionItemList ;	// cached sections
		M_POArySrt m_IniEntryItemList ;		// cached entries
		FILE *m_fp ;				// file pointer
} ;

/*
	class M_IniSectionIterator

	(perhaps this class should be split into a different module)
*/

class M_IniSectionIterator : public M_Obj
{
	M_CLSDEFS() ;

	public:
		~M_IniSectionIterator() ;
		M_IniSectionIterator(M_IniFile &) ;
		const M_Str &GetSectionName() ;
		void Rewind() ;
		M_SBOOL Next() ;
		M_SBOOL UnCache() ;
		M_SBOOL Cache() ;

	private:
		M_IniFLOC m_floc ;
		M_Str m_SectionName ;
		M_IniFile &m_IniFile ;
} ;

/*
	Class M_IniAccess		- *.INI files: general cached access

	User may use this class directly or derive from it.
*/

class M_IniAccess : public M_Obj
{
	M_CLSDEFS() ;

	public:
		virtual ~M_IniAccess() ;
		M_IniAccess(M_IniFAO fao= M_INIFAO_NONE) ;
		M_IniRC flush() ;
		void reset() ;
		M_SBOOL getEntryValue(
			const M_Str& sfn,
			const M_Str& ssection,
			const M_Str& sentryname,
			M_Str& sentryvalue
		) ;
		M_SBOOL putEntryValue(
			const M_Str& sfn,
			const M_Str& ssection,
			const M_Str& sentryname,
			const M_Str& sentryvalue
		) ;

	private:
		M_IniID findfileid(const M_Str& sfn) const ;
		M_IniID newfileid(const M_Str& sfn) ;
		M_IniID getfileid(const M_Str& sfn) ;

		M_IniFAO m_fao ;			// File access opts
		M_POArySrt m_IniFileList ;		// active .INI files
} ;

/*
	methods implemented in-line
*/

/*
	class M_IniSectionItem
*/
inline const M_Str &M_IniSectionItem::GetSectionName() const {
	return(m_ssection) ;
}

/*
	class M_IniEntryItem
*/
inline M_IniID M_IniEntryItem::getIdSection() const { return(m_idsection) ; }
inline M_SBOOL M_IniEntryItem::isChanged() const { return(m_bChanged) ; }

/*
	class M_IniFile
*/
inline const M_Str &M_IniFile::getFn() const { 	return(m_sfn) ;	}
inline M_SBOOL M_IniFile::isChanged() const {
	return((M_SBOOL) (nChanges() != 0)) ;
}

// #ifndef M_INI_H_INC
#endif

// end of mini.h
