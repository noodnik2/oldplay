/* 

	void pdmdcolor(i)		Machine-Dependent Set Video Color
	int i ;				Copyright (C) 1990, by ==C=R=C==

	Coding:		0	yellow
			1	green
			2	blue
			3	red
			4	white
			5	colors off	(default state)

	It is known that 0<=i<=5.  Please switch the console to the 
	indicated color if possible.

	Also sets "normal" attribute and "black" background colors.

*/

#include	"compiler.h" 
#include	"pd.h"
#include	"setdisp.h"

void pdmdcolor(i)
int i ; 
{
#if 0
	setdisp(i) ;
#endif
}
