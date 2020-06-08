#include	<stdio.h>
#include	"wrap.h"

void main()
{
	char *fn= "wrtest2.ini" ;
	char *section= "section" ;
	char *entry= "entry" ;
	char buffer[1024] ;

	IniInit() ;
	IniRead(fn, section, entry, buffer, sizeof(buffer)) ;
	printf("entry='%s'\n", buffer) ;
	IniWrite(fn, section, entry, "new entry") ;
	IniFini() ;
}
