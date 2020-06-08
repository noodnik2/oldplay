#include	<stdio.h>
#include	<dirent.h>

void main()
{
	DIR *dh ;
	struct dirent *de_p ;

	if ((dh= opendir(".")) == (DIR *)NULL) {
		fprintf(stderr, "can't do it\n") ;
		exit(1) ;
	}
	while((de_p= readdir(dh)) != (struct dirent *)NULL) {
		printf("%s\n", de_p->d_name) ;

	}
	closedir(dh) ;
}
