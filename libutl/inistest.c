#include	"compiler.h"

#include	"inis.h"

struct one {
	char c ;
	unsigned short s ;
	unsigned long l ;
} ;

struct two {
	unsigned short s ;
	char c ;
	char sz[31] ;
	unsigned long l ;
} ;

#undef	ISTRUCNAME
#define	ISTRUCNAME	struct one
struct istrucField ifaOne[]= {
	ISTRUCFIELD_INT("c", c),
	ISTRUCFIELD_INT("s", s),
	ISTRUCFIELD_INT("l", l),
} ;
#define	N_ONE	(sizeof(ifaOne) / sizeof(ifaOne[0])) 

#undef	ISTRUCNAME
#define	ISTRUCNAME	struct two
struct istrucField ifaTwo[]= {
	ISTRUCFIELD_INT("c", c),
	ISTRUCFIELD_INT("s", s),
	ISTRUCFIELD_INT("l", l),
	ISTRUCFIELD_STR("sz", sz),
} ;
#define	N_TWO	(sizeof(ifaTwo) / sizeof(ifaTwo[0])) 

void main()
{
	struct one o ;
	struct two t ;
	struct inisSection is[2] ;
	INISPARSE_RC prc ;

	memset(&o, '\0', sizeof(struct one)) ;
	memset(&t, '\0', sizeof(struct two)) ;

	is[0].pStruc= (void *) &o ;
	is[0].pszSectionName= "one" ;
	is[0].paFields= ifaOne ;
	is[0].nFields= N_ONE ;
 
	is[1].pStruc= (void *) &t ;
	is[1].pszSectionName= "two" ;
	is[1].paFields= ifaTwo ;
	is[1].nFields= N_TWO ;

	prc= inis_parseMultiple("inistest.ini", is, 2) ;

	printf("prc=%d\n", prc) ;
	if (prc != INISPARSERC_SUCCESS) {
		exit(1) ;
		/*NOTREACHED*/
	}

	printf("o.c='%u'\n", (unsigned) o.c) ;
	printf("o.s='%u'\n", (unsigned) o.s) ;
	printf("o.l='%lu'\n", (unsigned long) o.l) ;

	printf("t.c='%u'\n", (unsigned) t.c) ;
	printf("t.s='%u'\n", (unsigned) t.s) ;
	printf("t.l='%u'\n", (unsigned long) t.l) ;
	printf("t.sz='%s'\n", t.sz) ;

	exit(0) ;
}

/* end of inistest.c */
