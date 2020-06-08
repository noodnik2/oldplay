#include	<dirent.h>
#include	<fnmatch.h>

int main()
{
	DIR *pd ;
	struct dirent *pde ;

	for (
		pd= opendir(".") ;
		pde= readdir(pd) ;
	) {
		if (
			!fnmatch(
				"*.c",
				pde->d_name,
				FNM_NOESCAPE | FNM_PATHNAME | FNM_PERIOD
			)
		) {
			printf("f='%s'\n", pde->d_name) ;
		}
		
	}
	closedir(pd) ;
}
