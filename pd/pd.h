#define		PD_ECM		'\003'

#ifndef	PD_ROOT

extern	int	pdecm ;			/* PD input: ecm character   	*/
extern	int	pdbrkc ;      		/* PD output: break character 	*/

extern	int	pddbugflg ;    		/* debug on/off flag	   	*/
extern	int	pdoutbuf ;     		/* output buffering enabled	*/
extern	int	pdmaphi ;      		/* maphi output flag	   	*/

extern	int	pdrc ;	       		/* global return code	   	*/
extern	int	pdgison ;      		/* whether in graphics mode	*/
extern	int	pd_ints ;		/* number of BREAKs received    */

extern	char	pdversion[] ; 
extern	int	pdvermaj ;     		/* major version number	   	*/
extern	int	pdvermin ;     		/* minor version number	   	*/
			       	
extern	int	pdborder ; 		/* incoming BYTEORDER	   	*/
#endif

#ifdef	PD_ROOT

	int	pdecm= PD_ECM ;		/* ecm character	   	*/
#if 0
	int	pdbrkc= '\003' ;	/* assume ^C is interrupt     	*/
#endif
	int     pdbrkc= -1 ;            /* Break = break                */
	int	pddbugflg= 0 ; 		/* debug on/off flag		*/
	int	pdoutbuf= 0 ;  		/* output buffering enabled	*/
	int	pdmaphi= 0 ;   		/* maphi output flag		*/

	int	pdrc ;      		/* global return code	   	*/
	int	pdgison ;   		/* whether in graphics mode	*/
	int	pd_ints ;		/* number of BREAKs received    */

	char	pdversion[]= "1.00" ;
	int	pdvermaj= 1 ;  		/* major version number		*/
	int	pdvermin= 0 ;  		/* minor version number		*/

	int	pdborder= BYTEORDER ;	/* incoming BYTEORDER		*/

#endif

#define PD_MAXLINELEN	80
