/*
	Machine dependent code .. windowing interface 

	This version for machines with no windows.

	flag= pdmdhaswin()
		returns 0 if no windows, 1 if windows.

	rc= pdmdwindef(name,stuff)
	char *name ; 
	char *stuff ; 
		please return:
			0.	done
			1.	name already exists
			2.	stuff invalid
			3.	name invalid
		      999.	other error, for which you present errmsg
		called only if pdmdhaswin()==1.

	rc= pdmdwindrop(namelist)
	char *namelist ; 
		a list of windows to drop.  Drop all or none.
		called only if pdmdhaswin()==1.
		please return:
			0.	done
			1.	a name does not exist
			2.	argument invalid
		      999.	other error, for which you present errmsg

	rc= pdmdwinuse(name)
	char *name ; 
		current window to use.  
		called only if pdmdhaswin()==1.
		please return:
			0.	done
			1.	name does not exist
			2.	name invalid
		      999.	other error, for which you present errmsg

	(void)  pdmdwindir() 
		please present directory of windows
		called only if pdmdhaswin()==1

	(void)	pdmdcw(s)
	char*s ; 
		place the name of the current window in s. 
		called only if pdmdhaswin()==1

*/

int pdmdhaswin()
{
	return(0) ; 
}

int pdmdwindef(name,stuff)
char *name ; 
char *stuff ; 
{
	return(2) ; 
}

int pdmdwindrop(name)
char *name ; 
{
	return(2) ; 
}

int pdmdwinuse(name)
char *name ; 
{
	return(2) ; 
}

void pdmdwindir()
{
	return ; 
}

void pdmdcw(s)
char *s ; 
{
	return ; 
}
