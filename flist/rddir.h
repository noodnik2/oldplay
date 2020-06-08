#if 0
#include	<sys/dir.h>
#endif

typedef struct {
	int fh ;
} DIR ;

DIR *opendir() ;
struct direct *readdir() ;
void closedir() ;

