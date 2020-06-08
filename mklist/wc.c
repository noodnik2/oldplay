#include	<sys/types.h>
#include	"wc.h"

/*
	NOTE:	This code assumes ASCII!
*/

WC_RC wcCode(
	const char *pcszInputU,
	char *pszOutputU,
	int iMaxOutputU
) {
	int iOutSize ;
	unsigned char uc ;
	const unsigned char *pcuszInput ;
	unsigned char *puszOutput ;
	const unsigned char *puszOutputFence ;

	for (
		pcuszInput= (const unsigned char *) pcszInputU,
		puszOutput= (unsigned char *) pszOutputU,
		puszOutputFence= (const unsigned char *) (
				pszOutputU + iMaxOutputU
		) ;
		(uc= pcuszInput[0]) != '\0' ;
		pcuszInput++, puszOutput+= iOutSize
	) {

		switch(uc) {
			case ',':
			case ';':
			case '"':
			case '\'':
			case '&':
			case '=':
			case '(':
			case ')':
			case '<':
			case '>':
			case '?':
			case '#':
			case '{':
			case '}':
			case '%':
			case ' ':
				iOutSize= 3 ;
				break ;
			default:
				iOutSize= 1 ;
				break ;
		}

		if ((puszOutput + iOutSize)>= puszOutputFence) {
			puszOutput[0]= '\0' ;
			return(WCRC_OVERFLOW) ;
		}

		if (iOutSize == 3) {
			puszOutput[0]= '%' ;
			puszOutput[1]= WC_AHEXNIB(uc >> 4) ;
			puszOutput[2]= WC_AHEXNIB(uc) ;
			continue ;
		}

		puszOutput[0]= uc ;
	}

	puszOutput[0]= '\0' ;
	return(WCRC_SUCCESS) ;
}

/* end of wc.c */
