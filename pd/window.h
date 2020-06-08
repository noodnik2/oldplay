/*
	window header:  Describes dimensions of current window
*/

#define		NSTPENS		9
#define		NSTCOLORS	9

struct window {
	int     uy,ux ;                 /* user defined metric  */
	int     cy,cx ;                 /* user character size  */
	int	figy, figx;		/* user figure size     */
	long    my,by ;                 /* mapping for y        */
	long    mx,bx ;                 /* mapping for x        */
	chau style ;                    /* suppress style       */
	chau tmode ;                    /* mode for text dsply  */
	chau pen ;                      /* current pen number   */
	chau y32 ; 			/* y32/32 x/y aspect 	*/
	chau ppl, ppc ;			/* pixels/character:y,x */
	chau npen;			/* size of carousel     */
	chau junk;			/* next chau field      */
	chau *cmap;			/* Location of colormap */
} ;

extern double dble32k ;

#if 0
/*
These macros convert between the x and y metrics for the current screen

-- Marty
*/

#define         ydeflate(yxval)         (( ((int) gd2wd.y32)*(yxval))/32)
#define         yinflate(xyval)         (( (xyval)*32)/((int) gd2wd.y32))
#endif
