//	Revision:      		13
/*

	wrtest.cpp			qad-ARC21 Initialization Parameters
					Copyright (C) 1995, Contahal Ltd.


	See "iniacc.h" for comments.
			
*/

#include	"wrtest.h"
#include	"mini.h"

#include	<stdlib.h>
#include	<iostream.h>

typedef struct tagTESTIV {
	char *pszIniFile ;
	char *pszIniSection ;
	char *pszIniEntry ;
	char *pszNewValue ;
} WRTESTIV ;

static WRTESTIV wrtestiv[]= {
	{ "wrtest.ini",	"one", 	"first entry", "WRTEST:ONE:1st"		},
	{ "wrtest.ini",	"one", 	"second entry", "WRTEST:ONE:2nd"	},
	{ "win.ini",	"Windows",	"Beep", "BeepNo"		},
	{ "wrtest.ini",	"two", 	"first entry", "WRTEST:TWO:1st"		},
	{ "win.ini",	"intl",	"sCountry", "Zimbabwe"			},
	{ "new.ini",	"junk",	"noentry", 0				},
	{ "wrtest.ini",	"two",	"second entry", "WRTEST:TWO:2nd"	},
	{ "win.ini", "mci extensions", "wav", "wav,mid"			},
	{ "wrtest.ini",	"two",	"third entry", 0			},
	{ "wrtest.ini",	"new", "newfirst", "New Section, entry"		},
	{ "new.ini",	"test",	"first", "first entry in new file"	},
	{ "win.ini",	"sounds", "SystemDefault", "NoSound"		},
	{ "win.ini",	"intl",	"iCurrency", "123"			},
	{ "new.ini",	"test",	"another", "another entry in new file"	},
} ;
#define	NWRTESTIV	(sizeof(wrtestiv) / sizeof(wrtestiv[0]))

void main()
{
	M_IniAccess ia ;
	M_Str s ;
	M_SBOOL b ;
	int i ;

	cout << "Read:\n" ;
	for (i= 0; i< NWRTESTIV; i++) {
		s= "*** Undefined ***" ;
		b= ia.getEntryValue(
			wrtestiv[i].pszIniFile,
			wrtestiv[i].pszIniSection,
			wrtestiv[i].pszIniEntry,
			s
		) ;
		cout
			<< i
			<< ".) "
			<< wrtestiv[i].pszIniFile
			<< ":"
			<< wrtestiv[i].pszIniSection
			<< "."
			<< wrtestiv[i].pszIniEntry
			<< " ==> [" << b << "] '"
			<< (const char *) s
			<< "'\n"
		;
	}

	cout << "Write:\n" ;
	for (i= 0; i< NWRTESTIV; i++) {
		if (wrtestiv[i].pszNewValue) {
			b= ia.putEntryValue(
				wrtestiv[i].pszIniFile,
				wrtestiv[i].pszIniSection,
				wrtestiv[i].pszIniEntry,
				s= wrtestiv[i].pszNewValue
			) ;
			cout
				<< i
				<< ".) "
				<< wrtestiv[i].pszIniFile
				<< ":"
				<< wrtestiv[i].pszIniSection
				<< "."
				<< wrtestiv[i].pszIniEntry
				<< " ==> [" << b << "] '"
				<< (const char *) s
				<< "'\n"
			;
		}
	}

	cout << "Read Again:\n" ;
	for (i= 0; i< NWRTESTIV; i++) {
		s= "*** Still Undefined ***" ;
		b= ia.getEntryValue(
			wrtestiv[i].pszIniFile,
			wrtestiv[i].pszIniSection,
			wrtestiv[i].pszIniEntry,
			s
		) ;
		cout
			<< i
			<< ".) "
			<< wrtestiv[i].pszIniFile
			<< ":"
			<< wrtestiv[i].pszIniSection
			<< "."
			<< wrtestiv[i].pszIniEntry
			<< " ==> [" << b << "] '"
			<< (const char *) s
			<< "'\n"
		;
	}

}

#include	"mthrow.h"

void M_Throw::FatalError(const char *pszTitle, const char *pszText)
{
	fprintf(stderr, "fatal error: %s: %s\n", pszTitle, pszText) ;
	::exit(1) ;
}

// end of wrtest.cpp
