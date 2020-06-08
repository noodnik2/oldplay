#include	"compiler.h"

char *strim(cmd)
char *cmd;
{
	register int i;

	while(cmd[0]==' ') cmd++;
	for (i= strlen(cmd); i> 0; i--) {
		if (cmd[i-1]!=' ') break;
	}
	cmd[i]= '\0';
	return(cmd);
}
