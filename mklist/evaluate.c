#define	MSRC_Revision		24

/*

	evaluate.c:			Evaluate expression stream
					Copyright (C) 1994-1996, Marty Ross

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"subexpr.h"
#include	"evaluate.h"

static long *longstack ;
static int longstack_size ;
static int longstack_ptr ;
static PFNRETUSRVARLONG pfngetusrvarlong ;
static PFNSETUSRVARLONG pfnsetusrvarlong ;

static int evalerr(char *msg) ;
static int pushlong(long lval) ;
static int poplong(long *lval_p) ;
static int copylong(long *lval_p) ;

int evaluate_setlongstack(long *usr_longstack, int usr_longstack_size)
{
	longstack= usr_longstack ;
	longstack_size= usr_longstack_size ;
	pfngetusrvarlong= (PFNRETUSRVARLONG) 0 ;
	pfnsetusrvarlong= (PFNSETUSRVARLONG) 0 ;
	return(0) ;
}

int evaluate_setpfngetusrvarlong(PFNRETUSRVARLONG usr_pfngetusrvarlong)
{
	pfngetusrvarlong= usr_pfngetusrvarlong ;
	return(0) ;
}

int evaluate_setpfnsetusrvarlong(PFNSETUSRVARLONG usr_pfnsetusrvarlong)
{
	pfnsetusrvarlong= usr_pfnsetusrvarlong ;
	return(0) ;
}

int evaluate_exstream_long(
	unsigned char *exstream_fp,
	int exstream_len,
	long *long_result_fp
) {
	int i, op ;
	long r, arg1, arg2, arg3 ;
	int vid ;

	i= 0;
	longstack_ptr= 0 ;
	while(i< exstream_len) {

		op= exstream_fp[i++] ;

		if ((op>= EOPTR_STARTBOP) && (op<= EOPTR_ENDBOP)) {
			if (poplong(&arg2)) return(1) ;
			if (poplong(&arg1)) return(1) ;
			switch(op) {

				case EOPT_TESTEQ: r= (arg1 == arg2) ;	break ;
				case EOPT_TESTNE: r= (arg1 != arg2) ;	break ;
				case EOPT_TESTLT: r= (arg1 <  arg2) ;	break ;
				case EOPT_TESTGT: r= (arg1 >  arg2) ;	break ;
				case EOPT_TESTLE: r= (arg1 <= arg2) ;	break ;
				case EOPT_TESTGE: r= (arg1 >= arg2) ;	break ;
				case EOPT_LOGAND: r= (arg1 && arg2) ;	break ;
				case EOPT_LOGOR:  r= (arg1 || arg2) ;	break ;
				case EOPT_LOGXOR:
					r= ((arg1 != 0) ^ (arg2 != 0)) ;
					break ;
				case EOPT_BINAND: r= (arg1 &  arg2) ;	break ;
				case EOPT_BINOR:  r= (arg1 |  arg2) ;	break ;
				case EOPT_BINXOR: r= (arg1 ^  arg2) ;	break ;
				case EOPT_MUL:	  r= (arg1 *  arg2) ;	break ;
				case EOPT_DIV:
					if (arg2 == 0L) {
						return(	evalerr(
							"division by zero"
						) ) ;
					}
					r= (arg1 /  arg2) ;
					break ;
				case EOPT_MOD:
					if (arg2 == 0L) {
						return(	evalerr(
							"division by zero"
						) ) ;
					}
					r= (arg1 %  arg2) ;
					break ;
				case EOPT_ADD:	  r= (arg1 +  arg2) ;	break ;
				case EOPT_SUB:	  r= (arg1 -  arg2) ;	break ;
				case EOPT_SHL:	  r= (arg1 << arg2) ;	break ;
				case EOPT_SHR:	  r= (arg1 >> arg2) ;	break ;

				default:
					return(evalerr("unknown binary op")) ;
					break ;

			}
			if (pushlong(r)) return(1) ;
		}

		else if ((op>= EOPTR_STARTUOP) && (op<= EOPTR_ENDUOP)) {
			if (poplong(&arg1)) return(1) ;
			switch(op) {
				case EOPT_UNEG: r= -arg1 ;		break ;
				case EOPT_UPOS: r= arg1 ;		break ;
				case EOPT_UNOT: r= !arg1 ;		break ;
				case EOPT_UCPL: r= ~arg1 ;		break ;
				default:
					return(evalerr("unknown unary op")) ;
					break ;
			}
			if (pushlong(r)) return(1) ;
		}

		else if ((op>= EOPTR_START3OP) && (op<= EOPTR_END3OP)) {
			if (poplong(&arg3)) return(1) ;
			if (poplong(&arg2)) return(1) ;
			if (poplong(&arg1)) return(1) ;
			switch(op) {
				case EOPT_3COND: r= arg1? arg2: arg3 ;	break ;
				default:
					return(evalerr("unknown tertiary op")) ;
					break ;
			}
			if (pushlong(r)) return(1) ;
		}

		else if (op == EOPT_PUSHLIT) {
			arg1= (long) (
				*(
					(long *)
				      	(exstream_fp+i)
			      	)
			) ;
			i+= sizeof(unsigned long) ;
			if (pushlong(arg1)) return(1) ;
		}

		else if (op == EOPT_PUSHVAR) {
			vid= (int) (			/* var id # */
				*(
					(short *)
				      	(exstream_fp+i)
			      	)
			) ;
			i+= sizeof(unsigned short) ;
			if (pfngetusrvarlong == (PFNRETUSRVARLONG) 0) {
				return(evalerr("can't evaluate variable")) ;
			}
			
			if (!pfngetusrvarlong(vid, &arg1)) {
				return(evalerr("variable value error")) ;
			}

			if (pushlong(arg1)) return(1) ;
		}

		else if (op == EOPT_SETVAR) {
			vid= (int) (			/* var id # */
				*(
					(short *)
				      	(exstream_fp+i)
			      	)
			) ;
			i+= sizeof(unsigned short) ;
			if (copylong(&arg1)) return(1) ;
			if (pfnsetusrvarlong == (PFNSETUSRVARLONG) 0) {
				return(
					evalerr(
						"can't set variable"
						" (is '=' allowed here?)"
					)
				) ;
			}
			if (!pfnsetusrvarlong(vid, arg1)) {
				return(evalerr("variable set error")) ;
			}
		}

		else {
			return(evalerr("unknown op")) ;
		}

	}

	if (longstack_ptr != 1) return(evalerr("result error")) ;

	if (poplong(&r)) return(1) ;

	(*long_result_fp)= r ;

	return(0) ;
}

/*
	static internal subroutines
*/

static int evalerr(char *msg)
{
	fprintf(stderr, "eval: %s\n", msg) ;
	return(1) ;
}

static int pushlong(long lval)
{
	if (longstack_ptr == longstack_size) {
		return(evalerr("longstack overflow")) ;
	}
	longstack[longstack_ptr++]= lval ;
	return(0) ;
}

static int poplong(long *lval_p)
{
	if (longstack_ptr == 0) {
		return(evalerr("longstack underflow")) ;
	}
	(*lval_p)= longstack[--longstack_ptr] ;
	return(0) ;
}

static int copylong(long *lval_p)
{
	if (longstack_ptr == 0) {
		return(evalerr("longstack underflow")) ;
	}
	(*lval_p)= longstack[longstack_ptr-1] ;
	return(0) ;
}

/* end of evaluate.c */
