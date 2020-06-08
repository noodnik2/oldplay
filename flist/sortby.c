/*

	sortby				Sort the list on a speicified key
					Copyright (C) 1990, by ==C=R=C==

		The key is a routine which returns 0 or 1 for
		each line in the file list; 1 indicates that a
		switch should be made with this observation and
		the next according to the desired sort order.

		The routine is a simple bubble sort.

*/

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;
extern struct fl_opt flo ;
extern struct fl_scn fls ;

void sortby(rtn, rv)
int (*rtn)() ;
int rv ;
{
	int d, i, n, it ;
	struct fl_line **flp0, *flp, *tfp1 ;

	do {
		flp0= &(FL_LIST) ;

		for (
			i= it= 0, n= TOTFILE-1, flp= *flp0 ;
			(i<n) ;
			i++, flp= flp->next
		) {

			d= (*rtn)(flp, flp->next) ;
			if (rv) d= -d ;
			if (d> 0) {		/* swap if item2 < item1 */
				tfp1= flp->next ;
				(*flp0)= tfp1 ;
				flp->next= tfp1->next;
				tfp1->next= flp ;
				flp= tfp1 ;
				it++;
			}
			flp0= &(flp->next) ;
		}
	} while(it != 0) ;
}

/*
	s_name, s_ext, s_size, s_date - sort by name, ext, size or date

	returns 0 if item i1 matches item i2, or returns -1 if item i1 is
	logically before item i2, or 1 if item i1 is logically after item
	i2, according to the designated (ascending) sort order.

*/

int s_name(flp1, flp2)
struct fl_line *flp1, *flp2;
{
	return(strcmp(flp1->line, flp2->line));
}

int s_date(flp1, flp2)
struct fl_line *flp1, *flp2;
{
	return(	(flp1->stat.date)==(flp2->stat.date)? 0:
	    	(flp1->stat.date)< (flp2->stat.date)? -1: 1 ) ;
}

int s_size(flp1, flp2)
struct fl_line *flp1, *flp2;
{
	return(	(flp1->stat.size)==(flp2->stat.size)? 0:
	    	(flp1->stat.size)< (flp2->stat.size)? -1: 1 ) ;
}

int s_ext(flp1, flp2)
struct fl_line *flp1, *flp2;
{
	return(strcmp(fnext(flp1->line), fnext(flp2->line))) ;
}

