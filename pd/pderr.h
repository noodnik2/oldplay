/*
	Note on PD return codes:

	These are the return codes stored for the QUERY RETURN CODE 
	command.  
	GO_ERRST is special.  It records a PD_ABORT but forces a passage
	through the ERROR STATE. 

	Copyright (c) 1990 by C.R.C. 
*/

#define PD_OKAY		0
#define PD_ABORT	2
#define PD_BADARG	3
#define PD_NOWIN	4
#define PD_EXISTS	5
#define PD_NOTFND	7
#define PD_ERR		8

#define GO_ERRST	900
