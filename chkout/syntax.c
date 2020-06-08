/*

	syntax					


*/

#include	<stdio.h>

#include	"compiler.h"

static char *syntax_msg[]= {
  "",
  "chkout - check out file(s) for exclusive use",
  "",
  "usage:",
  "\t(1) chkout -c [-u <user>] [-g <group> [-m <mode>] <dev> [<dev> ...]",
  "\t(2) chkout -r [<dev> ...]",
  "\t(3) chkout -l [ { all | avail | busy | inuse } ]",
  "\t(4) chkout -q <dev> [<dev>...]",
  "\t(5) chkout [-h]",
  "",
  "Usage (1) allows a user to check out a device or resource file <dev> for",
  "exclusive use.  If on behalf of another user, the -u option may be specified.",
  "The group of the resource will be changed to the current group associated",
  "with the user, or the group specified by the use of the -g option.  The mode",
  "of the resource will be set to 600 or the value specified by the use of the",
  "-m option.  More than one resource may be indicated on the same command.",
  "Usage (2) returns previously checked-out resources.  Usages (3) and (4) report",
  "the check-out status of resources.  Usage (5) prints this usage diagram.",
  ""
} ;
#define	N_MSGS	(sizeof(syntax_msg)/sizeof(syntax_msg[0])) 

void syntax()
{
	int i ;

	for (i= 0; i< N_MSGS; i++) {
		fprintf(stdout, "%s\n", syntax_msg[i]) ;
	}
}
