/*
	GDL interface to filling a segment of a pie chart.

	header file describing common structures.

	Note:  Angles are measured from 12:00 from 0 to 32767.
		phi1 and phi2 must both be positive or zero, and
		phi2>=phi1

	Copyright (c) 1985 by C.R.C.
*/

#include "window.h"
extern double double0, double2, double3, doubleh, doublepi ;
extern double double2p ;
extern struct window gd2wd ;
extern int gdlfile;
extern chau gdlomode, gdloloc;

struct angle {
	double  phi ;                   /* angle in radians     */
	double  sin ;                   /* sin of angle         */
	double  cos ;                   /* cos of phi           */
	double  sincos ;                /* sin/cos of phi       */
	int     scdef ;                 /* sin/cos defined      */
	int     quadrant ;              /* quadrant of angle    */
	int     upper ;                 /* 1 if Q1 or Q4        */
	int     y, x ;                  /* end point of line    */
} ;



struct arcdat {
	struct  angle phi1 ;
	struct  angle phi2 ;
	double  radius2 ;               /* radius squared       */
	int     radius ;                /* radius of circle     */
	int     yce, xce ;              /* center points        */
	int     y0,y1 ;                 /* y range              */
	int     x0,x1 ;                 /* x range              */
	int     casenum ;               /* case number          */
	int     amin, amax ;            /* arc min, max         */
	int     l1, l2 ;                /* x|y on line          */
	int     min, max ;              /* first range          */
	int     disjoint ;              /* flag for disjoint    */
	int     min2, max2 ;            /* second range         */
	int     xsk, ysk, xof ;         /* shading parms        */
} ;

#define UNDEFINED       32000
#define MAXANGLE        32767

#define MIN(x,y)        (x>y ? y : x)
#define MAX(x,y)        (x<y ? y : x)

double stcos(), stsin(), stsqrt() ;
