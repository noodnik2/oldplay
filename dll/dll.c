/*

	dll.c


*/

static int dllstate= 0 ;

void _init()
{
	dllstate= 1 ;
	printf("_init called\n") ;
}

void _fini()
{
	dllstate= -1 ;
	printf("_fini called\n") ;
}

void dll_init()
{
	printf("dll initialization\n") ;
	dllstate= 2 ;
}

int dll_getstate()
{
	printf("dll state called: state=%d\n", dllstate) ;
	return(dllstate) ;
}

int dll_setstate(int state)
{
	int oldstate= dllstate ;

	dllstate= state ;
	printf("dll state set to %d (old state=%d)\n", state, oldstate) ;
	return(oldstate) ;
}

int dll_fini()
{
	dllstate= 3 ;
	printf("dll fini called\n") ;
}

/* end of dll.c */

