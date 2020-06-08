/*
	(void) pdmddebug() 

	machine dependent debug information 

	Use pdwrstty() to present lines of information.
	You must not present a blank ("\n") line.
	Be sure your last line ends with a newline character. 

*/ 

#include	"compiler.h"
#include	"pd.h"

#define		CAT(msg,v)	strcat(msgbuf, msg);	 	\
				sprintf(msgbuf+strlen(msgbuf), " %d\n", v)

void pdmddebug() 
{ 
	char msgbuf[1025] ;
	char ttlbuf[81] ;

	strcpy(msgbuf, "Status:\n") ;
	CAT("pdecm", pdecm) ;
	CAT("pdrc", pdrc) ;
	CAT("pdoutbuf", pdoutbuf) ;
	CAT("pdgison", pdgison) ;
	CAT("pdmaphi", pdmaphi) ;
	CAT("pddbugflg", pddbugflg) ;
	CAT("pdvermaj", pdvermaj) ;
	CAT("pdvermin", pdvermin) ;
	CAT("pdborder", pdborder) ;

	sprintf(ttlbuf, " PD %s: ", pdversion) ;

#if 0
	idisplay(msgbuf, ttlbuf, 0) ;
#else
	pdwrstty(ttlbuf) ;
	pdwrstty(msgbuf) ;
#endif

}
