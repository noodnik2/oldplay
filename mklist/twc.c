#include	"wc.h"

void main(int argc, char *argv[])
{
	char buf[128] ;
	WC_RC wcrc ;

	wcrc= wcCode(argv[1], buf, sizeof(buf)) ;
	printf("R(%d); s='%s'\n", (int) wcrc, buf) ;
}
