#include	<stdio.h>
#include	"mini.h"

void main()
{
	M_IniFile f("test.db") ;
	M_Str section, entry, value ;
	M_IniFLOC floc ;
	
	while(f.getNextSectionName(section, floc)) {
		printf("[%s]\n", (const char*) section) ;
		while(f.getNextEntryName(entry, value, floc)) {
			printf("%s=%s\n", (const char*) entry, (const char*) value) ;
		}
	}
}

