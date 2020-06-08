/*
	chk_err.c			Print error message to error file.


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

void chk_err(msg)
char *msg ;
{
	clerror(msg) ;
}
