#include <stdio.h>
#include <tcl/tcl.h>

int
RandomCmd(ClientData clientData, Tcl_Interp *interp,
		int argc, char *argv[]);

main(int argc, char *argv[])
{
	Tcl_Interp *interp;
	int error;
	char *trace;

	interp = Tcl_CreateInterp();
	Tcl_Init(interp);
	Tcl_CreateCommand(interp, "xrandom", RandomCmd,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	error = Tcl_EvalFile(interp, argv[1]);
	if (error != TCL_OK) {
		fprintf(stderr, "%s: %s\n", argv[1],
				interp->result);
		trace = Tcl_GetVar(interp, "errorInfo",
			 	TCL_GLOBAL_ONLY);
		if (trace != NULL) {
			fprintf(stderr, "*** TCL TRACE ***\n");
			fprintf(stderr, "%s\n", trace);
		}
		exit(1);
	} else {
		printf("%s\n", interp->result);
		exit(0);
	}
}
int
RandomCmd(ClientData clientData, Tcl_Interp *interp,
		int argc, char *argv[])
{
	int rand, error;
	int limit = 0;
	if (argc > 2) {
		Tcl_SetResult(interp, "Usage: xrandom ?range?", TCL_STATIC);
		return TCL_ERROR;
	}
	if (argc == 2) {
	    error = Tcl_GetInt(interp, argv[1], &limit);
	    if (error != TCL_OK) {
		return error;
	    }
	}
	rand = random();
	if (limit != 0) {
		rand = rand % limit;
	}
	sprintf(interp->result, "%d", rand);
	return TCL_OK;
}
