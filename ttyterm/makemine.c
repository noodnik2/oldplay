#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"

char *getenv() ;

void main()
{
	int rc ;
	char *terminal ;

	if ((terminal= getenv("TERMTTY")) == NULL) {
		fprintf(stderr, "no terminal\n") ;
		exit(1) ;
	}

	if (rc= makemine(terminal)) {
		fprintf(stderr, "can't make mine: rc= %d\n", rc) ;
		exit(1) ;
	}

	fprintf(stderr, "terminal mine\n") ;
}

static int makemine(terminal)
char *terminal ;
{
	int rc ;
	uid_t my_uid ;
	gid_t my_gid ;

	my_uid= getuid() ;
	my_gid= getgid() ;

	if (rc= chown(terminal, my_uid, my_gid)) {
		fprintf(stderr, "can't chown, rc=%d\n", rc) ;
		return(rc) ;
	}

	if (rc= chmod(terminal, (mode_t) (S_IRUSR|S_IWUSR))) {
		fprintf(stderr, "can't chmod, rc= %d\n", rc) ;
		return(rc) ;
	}

	return(0) ;
}
