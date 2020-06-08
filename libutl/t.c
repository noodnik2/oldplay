#include	"compiler.h"
#include	"lclprtf.h"

void main(int argc, char *argv[])
{
	LCLPRTF_OPTS lOpts ;
	
	if (!lclprtf_parseopts(argv[1], &lOpts)) {
		clerror("couldn't parse") ;
		exit(1) ;
	}
	printf("lOpts=0x%X\n", lOpts) ;
}
