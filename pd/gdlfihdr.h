/*
	gdlfihdr.h                      GDL Fileheader

	Copyright (c) 1987 by C.R.C.
*/

struct gdlfihdr {
	chau      ch1 ;                 /* 1st char id = 129    */
	chau      ch2 ;                 /* 2nd char id = 132    */
	chau      release ;             /* Current release is 1 */
	chau      border ;              /* 1 -> HILO, 0 -> LOHI */
} ;

#define GDLCH1 129
#define GDLCH2 132
#define GDLRELEASE 1
