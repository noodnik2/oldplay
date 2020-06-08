#include	<stdio.h>
#include	<ctype.h>
#include	"mini.h"


/*
*/

static int isNumeric(const M_Str&) ;
static void fixupStr(M_Str& s) ;


/*

*/

void main()
{
	M_IniFile f("midi.db") ;
	M_Str section, entry, value ;
	M_IniFLOC floc ;
	int maxlenSection, maxlenEntry, maxlenValue ;

	maxlenSection= maxlenEntry= maxlenValue= 0 ;
	while(f.getNextSectionName(section, floc)) {
		fixupStr(section) ;
		while(f.getNextEntryName(entry, value, floc)) {
			fixupStr(entry) ;
			fixupStr(value) ;
			printf(
				(
					isNumeric(value)
				?	"%s\t%s\t%s\tNULL\n"
				:	"%s\t%s\tNULL\t%s\n"
				),
				(const char*) section,
				(const char*) entry,
				(const char*) value
			) ;
			if (maxlenSection< section.length()) {
				maxlenSection= section.length() ;
			}
			if (maxlenEntry< entry.length()) {
				maxlenEntry= entry.length() ;
			}
			if (maxlenValue< value.length()) {
				maxlenValue= value.length() ;
			}
		}
	}

	fprintf(stderr, "maxlenSection: %d\n", maxlenSection) ;
	fprintf(stderr, "maxlenEntry: %d\n", maxlenEntry) ;
	fprintf(stderr, "maxlenValue: %d\n", maxlenValue) ;
}


/*
*/

static void fixupStr(M_Str& s)
{
	int l= s.length() ;
	if (l<= 0) return ;

	char* p= new char[l+1] ;
	s.gets(p) ;

	for (int i= 0; i< l; i++) {
		if (p[i] == '"') {
			p[i]= '\'' ;
		}
	}

	s= p ;
	delete[] p ;
}

static int isNumeric(const M_Str& s)
{
	int i, l ;
	const char* p ;

	l= s.length() ;
	if (l == 0) return(0) ;

	p= (const char*) s ;
	for (i= 0; i< l; i++) {
		if (!p[i] || !isdigit(p[i])) break ;
	}
	return((i< l)? 0: 1) ;
}


/* */
