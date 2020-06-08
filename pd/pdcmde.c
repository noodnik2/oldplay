/*
	pdcmde.c			Execute PD command

*/

#include	"compiler.h"
#include	"pd.h"
#include	"pderr.h"

void pdcolor() ;
void pdmddebug() ;
void pdmdwindir() ;
void pdmdcw() ; 

int pdcmdeb()
{
	pdoutbuf= 1 ; 
	return(PD_OKAY) ; 
}

int pdcmddb()
{
	pdflushbuf() ; 
	pdoutbuf = 0 ; 
	return(PD_OKAY) ; 
}

int pdcmddbg() 
{
	char buf[80] ; 

	sprintf(buf, "pd: ascm=1 maphi=%d border=%d\n",	pdmaphi, pdborder) ; 
	pdwrstty(buf) ; 
	pdmddebug() ; 
	return(PD_OKAY) ; 
}

int pdcmddcw()
{
	char name[80] ; 

	if (pdmdhaswin()) { 
		name[0]= '\0' ; 
		pdmdcw(name) ; 
		if (name[0]) pdwrstty(name) ; 
	}
	return(0) ;
}


int pdcmdwdir()
{
	int rc ; 
		
	if (pdmdhaswin()==0) return(pderror(3)) ; 
	pdmdwindir() ; 
	return(0) ; 
}

int pdcmdasp(s) 
char *s ; 
{
	char buf[200] ; 

	if (pdcmdaspu(s)) { 
		sprintf(buf, "cannot set pen '%s'", s) ; 
		pderrmsg(buf) ; 
		return(PD_BADARG) ; 
	}
	return(PD_OKAY) ; 
}

int pdcmdacmap(s)
char *s ; 
{
	switch(pdmdcmap(s)) { 
		case 0:		return(PD_OKAY) ; 
		case 1:		pderrmsg("CMAP irrelevant\n") ; break ; 
		case 2:		pderrinv(s) ; break ; 
		case 999:	break ; 
		default:	pderror(4) ; 
	}
	return(PD_BADARG) ; 
}
		
static int pdcmdaspu(s)
char *s ; 
{
	char *t ; 

	for (t= s; *t != ' '; t++) { 
		if (*t == '\0') return(PD_BADARG) ; 
	}
	*t= '\0' ;
	for(t++; *t==' '; t++) { 
		if (*t == '\0') return(PD_BADARG) ; 
	}
	return(pdmdsetpen(s,t)) ; 
}

int pdcmdawd(s)
char *s ; 
{
	int rc ; 
	char *t ;   

	if (pdmdhaswin()==0) return(pderror(3)) ; 

	for (t= s; *t != ' '; t++) { 
		if (*t == '\0') return(pderror(4)) ; 
	}
	*t= '\0' ;
	for(t++; *t == ' '; t++) { 
		if (*t == '\0') return(pderror(4)) ; 
	}
	rc= pdmdwindef(s,t) ; 
	switch(rc) { 
		case 0:		return(PD_OKAY) ; 
		case 1:		return(pderrex(t)) ; 
		case 2:		return(pderrinv(s)) ; 
		case 3:		return(pderrinv(t)) ; 
		case 999:	return(PD_BADARG) ; 
	}
	return(pderror(4)) ; 
}

int pdcmdawdr(s)
char *s ; 
{
	int rc ; 

	if (pdmdhaswin()==0) return(pderror(3)) ; 
	rc= pdmdwindrop(s) ; 
	switch(rc) { 
		case 0:		return(PD_OKAY) ; 
		case 1:		return(pderrnf("name(s)")) ; 
		case 2:		return(pderrinv("name(s)")) ; 
		case 999:	return(PD_BADARG) ; 
	}
	return(pderror(4)) ; 
}

int pdcmdauw(s)
char *s ; 
{ 
	int rc ; 

	if (pdmdhaswin()==0) return(pderror(3)) ;

	rc= pdmdwinuse(s) ; 
	switch(rc) { 
		case 0:		return(PD_OKAY) ; 
		case 1:		return(pderrnf(s)) ; 
		case 2:		return(pderrinv(s)) ; 
		case 999:	return(PD_BADARG) ; 
	}
	return(pderror(4)) ; 
}


#if 0
int pdcmdabo(s)
char *s ; 
{
	if (s[1] != '\0') return(PD_BADARG) ; 
	switch(*s) { 
		case '1':	pdborder= HILO ; break ; 
		case '2':	pdborder= LOHI ; break ; 
		default:	return(PD_BADARG) ; 
	}
	return(PD_OKAY) ; 
}
#endif

int pdcmdaee(s)
char *s ; 
{
	if (s[1] == '\0') { 
		switch(s[0]) { 
			case '0':	pdmaphi= 1 ; return(0) ; break ; 
			case '1':	pdmaphi= 0 ; return(0) ; break ; 
		}
	}
	pderrmsg("\nee: bad argument") ; 
	return(PD_BADARG) ; 
}

int pdcmdaclr(s)
char *s ; 
{
	if (s[1] == '\0') { 
		if (s[0]>='0' && s[0]<='5') { 
			pdcolor(s[0] - '0') ; 
			return(PD_OKAY) ; 
		}
	}
	pderrmsg("\nc: bad argument\n") ; 
	return(PD_BADARG) ; 
}

int pdcmdasv(s)
char *s ; 
{
	int major, minor ; 
	char buf[80] ; 

	if (sscanf(s, "%d.%d", &major, &minor)==2) { 
		if ((major == pdvermaj) && (minor<= pdvermin)) {
			return(PD_OKAY) ; 
		}
	}

	sprintf(buf, "RTM version number mismatch: %s", s) ; 
	pderrmsg(buf) ; 

	sprintf(buf,"PD internal version: %s\n", pdversion) ; 
	pderrmsg(buf) ; 

	pderrmsg("\nExit now!\n") ; 
	return(PD_OKAY) ; 
}

