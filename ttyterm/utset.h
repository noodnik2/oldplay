
/*

	utset.h


*/

#include	<sys/types.h>
#include	<sys/stat.h>

#if OPSYSLVL==OPLBSD42
#include	<sgtty.h>
#endif

#if OPSYSLVL==OPLSYSV
#include	<termio.h>
#endif

#if OPSYSLVL==OPLBSD42
typedef	struct sgttyb TERMCHAR ;
#endif

#if OPSYSLVL==OPLSYSV
typedef	struct termio TERMCHAR ;
#endif

struct utset {
	TERMCHAR inp_s ;
	TERMCHAR out_s ;
} ;

