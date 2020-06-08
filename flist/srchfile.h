/*

	srchfile.h:			Find file across PATH env variable
					Copyright (C) 1992, Contahal Ltd.

*/

#define		FT_EXEC		0

#if	OPSYS==OPUNIX
#define		PATH_DELIM	':'
#define		PATH_SEP	'/'
#endif

#if	OPSYS==OPMSDOS
#define		PATH_DELIM	';'
#define		PATH_SEP	'\\'
#endif

