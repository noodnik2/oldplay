/*

	ebcdic.h			EBCDIC translation facility header
					Copyright (C) 1989, by ==C=R=C==

	Defines:	TABLE_SIZE	256
			TABLER		(table-read macro)
			TABLEW		(table-write macro)
			A_MISSING	(missing EBCDIC value)

			ebc2asc(e)	(conversion macro)
			ebcistxt(e)	(indicator macro)
			ebcisok(e)	(indicator macro)


	If macro TABLE_OWNER is NOT defined, then the external
	declarations are also made (so the macros can use them).


	(char) ebcatca[TABLE_SIZE]	the translation table data

		
*/

#define	TABLE_SIZE	256
#define	TABLER(i)	(ebcatca[i])
#define	TABLEW(i,v)	(ebcatca[i]= (chau)v)
#define	A_MISSING	0

#define	ebc2asc(e)	((ebcatca[e]==A_MISSING)? (e): ebcatca[e])
#define	ebcistxt(e)	((ebcatca[e]==A_MISSING)? 0: 1)
#define	ebcisok(e)	((e>= 0) && (e< TABLE_SIZE))

#ifndef	TABLE_OWNER
extern chau ebcatca[] ;
#endif
