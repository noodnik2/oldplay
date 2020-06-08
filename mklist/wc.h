/*	Revision:		1
*/

/*

	wc.h

*/


/*
*/

#define	WC_RC			int
#define	WCRC_SUCCESS		0
#define	WCRC_OVERFLOW		16

#define	WC_NIB(c)		((c) & 0x0f)
#define	WC_AHEXNIB(c)		((WC_NIB(c)>9)? WC_NIB(c)+'A'-10: WC_NIB(c)+'0')


/*

*/

WC_RC wcCode(
	const char *pcszInputU,
	char *pszOutputU,
	int iMaxOutputU
) ;

/* end of wc.h */
