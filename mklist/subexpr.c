/*	Revision:	126
*/

#ifndef		TEST_VER
#define		TEST_VER		0
#endif

/*

	subexpr.c			Sub-Expression Compiler
					Copyright (C) 1993-1996, Marty Ross

	Compiles sub-expressions using standard notation and
	operator precedence.  Examples:

	1.)	Compiles:	"5 == 1 + 2 + 3 * 4 / 5"
		Into:		5 1 2 + 3 4 * 5 / + ==

		(which, if executed, would leave '1' (TRUE) on the stack)


	2.)	Compiles:	"5 == ( 1 + 2 + 3 ) * 4 / 5"
		Into:		5 1 2 + 3 + 4 * 5 / ==

		(which, if executed, would leave '0' (FALSE) on the stack)

*/

#include	"compiler.h"

#include	<stdio.h>
#include	<string.h>

#include	"subexpr.h"

void subexpr_setexstream(unsigned char *usr_exstream, int usr_exstream_size) ;
void subexpr_setfoundvar(FNUSRFINDVAR pfnufv) ;
void subexpr_setmakelong(FNUSRMAKELONG fnusrmakelong) ;

int subexpr_exstreamlen() ;
int subexpr(subexpr_t exp) ;
static int assignexp(int i, subexpr_t exp) ;
static int makelong(char *s, long *lnum_p) ;
static int invexp(subexpr_t exp, char *why) ;
static void showexp(char *prefix, subexpr_t exp) ;

int dump_exstream(unsigned char *exstream_fp, int exstream_len) ;

static int addexstream_byte(unsigned char b) ;
static int addexstream_short(short s) ;
static int addexstream_long(long l) ;
static int addexstream_op(int op) ;

static int overflow_abort() ;
static void showop(char *s) ;
static int getunary_bintok(char *tok) ;
static int getbinop_bintok(char *tok) ;
static int subexpr0(subexpr_t exp) ;
static int subexpr00(subexpr_t exp) ;
static int subexpr000(subexpr_t exp) ;

static int debug_expr= 0 ;

#ifdef		TEST_VER
#if		TEST_VER

#define		EXSTREAM_SIZE		256

void main(argc, argv)
int argc ;
char *argv[] ;
{
	subexpr_t exp ;
	unsigned char exstream[EXSTREAM_SIZE] ;
	char **tok ;
	int len ;

	tok= argv + 1 ;
	len= argc - 1 ;

	if (len> 0) {
		if (strcmp(tok[0], "-z") == 0) {
			debug_expr= 1 ;
			tok++ ;
			len-- ;
		}
	}

	exp.tok= tok ;
	exp.len= len ;

	subexpr_setexstream(exstream, EXSTREAM_SIZE) ;
	subexpr(exp) ;

	dump_exstream(
		(unsigned char *)exstream,
		subexpr_exstreamlen()
	) ;
}

#endif
#endif

typedef struct {
	char *src_tok ;
	int bin_tok ;
} operator_t ;

static operator_t intop1_calc[]= {	/* unary calc */
	{ "-", EOPT_UNEG	},
	{ "+", EOPT_UPOS	},
	{ "!", EOPT_UNOT	},
	{ "~", EOPT_UCPL	},
	{ NULL			}
} ;

static operator_t intop2_logor[]= {
	{ "||", EOPT_LOGOR	},
	{ NULL			}
} ;

static operator_t intop2_logand[]= {
	{ "&&", EOPT_LOGAND	},
	{ NULL			}
} ;

static operator_t intop2_bitor[]= {
	{ "|", EOPT_BINOR	},
	{ NULL			}
} ;

static operator_t intop2_bitxor[]= {
	{ "^", EOPT_BINXOR	},
	{ NULL			}
} ;

static operator_t intop2_bitand[]= {
	{ "&", EOPT_BINAND	},
	{ NULL			}
} ;

static operator_t intop2_equal[]= {
	{ "==", EOPT_TESTEQ	},
	{ "!=", EOPT_TESTNE	},
	{ NULL			}
} ;

static operator_t intop2_relop[]= {
	{ "<", EOPT_TESTLT	},
	{ ">", EOPT_TESTGT	},
	{ "<=", EOPT_TESTLE	},
	{ ">=", EOPT_TESTGE	},
	{ NULL			}
} ;

static operator_t intop2_shift[]= {
	{ "<<", EOPT_SHL	},
	{ ">>", EOPT_SHR	},
	{ NULL			}
} ;

static operator_t intop2_additive[]= {
	{ "+", EOPT_ADD		},
	{ "-", EOPT_SUB		},
	{ NULL			}
} ;

static operator_t intop2_multip[]= {
	{ "*", EOPT_MUL		},
	{ "/", EOPT_DIV		},
	{ "%", EOPT_MOD 	},
	{ NULL			}
} ;

static operator_t *intop2_table[]= {	/* order of increasing precedence */
	intop2_logor,
	intop2_logand,
	intop2_bitor,
	intop2_bitxor,
	intop2_bitand,
	intop2_equal,
	intop2_relop,
	intop2_shift,
	intop2_additive,
	intop2_multip,
	NULL
} ;	

static operator_t intop3_conditional[]= {		/* ?: */
	{ NULL			}
} ;

static unsigned char *exstream ;
static int exstream_size ;
static int exstream_len ;
static FNUSRFINDVAR pfnfoundvar ;
static FNUSRMAKELONG pfnmakelong ;

void subexpr_setexstream(unsigned char *usr_exstream, int usr_exstream_size)
{
	exstream= usr_exstream ;
	exstream_size= usr_exstream_size ;
	pfnfoundvar= (FNUSRFINDVAR) 0 ;
	pfnmakelong= (FNUSRMAKELONG) 0 ;
}

void subexpr_setfoundvar(FNUSRFINDVAR usr_pfnfoundvar)
{
	pfnfoundvar= usr_pfnfoundvar ;
}

void subexpr_setmakelong(FNUSRMAKELONG usr_pfnmakelong)
{
	pfnmakelong= usr_pfnmakelong ;
}

int subexpr_exstreamlen()
{
	return(exstream_len) ;
}

int subexpr_setdebug(int level)
{
	debug_expr= level ;
	return(0) ;
}

int subexpr(subexpr_t exp)
{
	exstream_len= 0 ;
	if (debug_expr) showexp("exp: ", exp) ;
	return(subexpr0(exp)) ;
}

/*
	finds =		right to left associativity;
			left-to-right scan
*/
static int subexpr0(subexpr_t exp)
{
	int i, expr_nest ;
	char *exptok ;

	/*	pass looking for = */

	if (debug_expr) showexp("exp0: ", exp) ;

	expr_nest= 0 ;
	for (i= 0; i< exp.len-1; i++) {
		exptok= exp.tok[i] ;
		if (strcmp(exptok, ")") == 0) expr_nest++ ;
		if (strcmp(exptok, "(") == 0) expr_nest-- ;
		if (expr_nest != 0) continue ;
  		if (strcmp(exptok, "=") != 0) continue ;
		return(assignexp(i, exp)) ;
	}
	return(subexpr00(exp)) ;
}

/*
	finds ?:	right to left associativity;
			left-to-right scan
*/
int subexpr00(subexpr_t exp)
{
	subexpr_t subexp ;
	int i, j, bin_tok ;
	int expr_nest, condop_nest ;
	char *exptok ;

	/*	pass looking for ?: */

	if (debug_expr) showexp("exp00: ", exp) ;

	expr_nest= 0 ;
	condop_nest= 0 ;
	bin_tok= EOPT_3COND ;
	for (i= 0; i< exp.len-1; i++) {
		exptok= exp.tok[i] ;
		if (strcmp(exptok, ")") == 0) expr_nest++ ;
		if (strcmp(exptok, "(") == 0) expr_nest-- ;
		if (expr_nest != 0) continue ;
		/* find the corresponding '?' */
  		if (strcmp(exptok, "?") != 0) continue ;
		for (j= i+1; j< exp.len-1; j++) {
			exptok= exp.tok[j] ;
			if (strcmp(exptok, ")") == 0) expr_nest++ ;
			if (strcmp(exptok, "(") == 0) expr_nest-- ;
			if (expr_nest != 0) continue ;
	  		if (strcmp(exptok, "?") == 0) {
				condop_nest++ ;
				continue ;
			}
			if (strcmp(exptok, ":") == 0) {
				if (condop_nest == 0) {
					subexp.len= i ;
					subexp.tok= exp.tok ;
					if (subexpr0(subexp)) return(1) ;
					subexp.len= j - i - 1 ;
					subexp.tok= exp.tok + i + 1 ;
					if (subexpr0(subexp)) return(1) ;
					subexp.len= exp.len - j - 1 ;
					subexp.tok= exp.tok + j + 1 ;
					if (subexpr0(subexp)) return(1) ;
					if (addexstream_op(bin_tok)) return(1) ;
					return(0) ;
				}
				condop_nest-- ;
				continue ;
			}
		}
	}
	return(subexpr000(exp)) ;
}

/*
	"divide-and-conquer"
	
	NOTES:

	(1)	this routine heavily makes use of recursion;
		make sure there is plenty of stack space.

	(2)	processing of unary operators was recently
		added on a "trial-and-error" basis.  This
		last addition is definately not thought to
		be implemented correctly.

*/
static int subexpr000(subexpr_t exp)
{
	subexpr_t subexp ;
	operator_t *iotp ;
	char *exptok, *cmptok ;
	int i0, i, j, k, expr_nest, bin_tok ;
	int var_id ;
	long lnum ;

	if (debug_expr) showexp("exp000: ", exp) ;

	if (exp.len == 0) {			/* null expression */
		return(invexp(exp, "empty sub-expression")) ;
	}

	/*
		find lowest-priority operator, right-to-left
		(to allow logical left-to-right grouping)

		if found, split expression at operator, parse
		both left and right sub-expressions, output
		operator to execution stream, and return.
	*/

	expr_nest= 0 ;

	/* scan from lowest to highest precedence for operators */
	for (j= 0; (iotp= intop2_table[j]) != NULL; j++) {

		/* from right, find "unary expression", if one present */
		i0= exp.len - 1 ;
		if (strcmp(exp.tok[i0], ")") == 0) {
			for (; i0>= 0; i0--) {
				exptok= exp.tok[i0] ;	/* expression token */
					/* skip embedded expressions */
				if (strcmp(exptok, ")") == 0) expr_nest++ ;
				if (strcmp(exptok, "(") == 0) expr_nest-- ;
				if (expr_nest == 0) break ;
			}
			if (expr_nest != 0) break ;
		}
		if (i0<= 0) i0= exp.len - 1 ;	/* one big sub-expression */
		else {
			/*
				back up to first unary operator before
				expression indexed by i0
			*/
			i0-- ;
			if (getunary_bintok(exp.tok[i0])>= 0) {
				while(i0> 0) {
					if (getunary_bintok(exp.tok[i0-1])< 0) {
						break ;
					}
					i0-- ;
				}	
			}

			/*
				now, i0 points to first unary operator
				in series of unary operators, if any were
				found.  If i is zero, and it indexes a
				unary operator, then compile the unary
				operation.
			*/
			if (i0 == 0) {
				bin_tok= getunary_bintok(exp.tok[0]) ;
				if (bin_tok>= 0) {
					subexp.tok= exp.tok + 1 ;
					subexp.len= exp.len - 1 ;
					if (subexpr0(subexp)) return(1) ;
					if (addexstream_op(bin_tok)) return(1) ;
					return(0) ;
				}
			}
			else {
				/*
					if the token before i0 (the first
					unary operator in a possible sequence)
					is a binary operator, then back up
					to it.  THIS IS SLOW!
				*/
				if (getbinop_bintok(exp.tok[i0-1])>= 0) {
					i0-- ;
				}
			}
		}

		/* scan leftward across expression: find binary exp */
		for (i= i0; i>= 0; i--) {

			exptok= exp.tok[i] ;	/* expression token */

					/* skip embedded expressions */
			if (strcmp(exptok, ")") == 0) expr_nest++ ;
			if (strcmp(exptok, "(") == 0) expr_nest-- ;
			if (expr_nest != 0) continue ;	/* in subexpr */

			if (i == 0) break ;	/* not binary expr */

			/* scan for operators in current precedence class */
			for (k= 0; (cmptok= iotp[k].src_tok) != NULL; k++) {

				/* binary operator in current class? */
				if (strcmp(exptok, cmptok) == 0) {

					/* yes: split, parse subexpressions */
					bin_tok= iotp[k].bin_tok ;
					subexp.tok= exp.tok ;
					subexp.len= i ;
					if (subexpr0(subexp)) return(1) ;
					subexp.tok= exp.tok+i+1 ;
					subexp.len= exp.len-i-1 ;
					if (subexpr0(subexp)) return(1) ;

					/* do binary op on them */
					if (addexstream_op(bin_tok)) return(1) ;
					return(0) ;
				}
			}
		}
	}

				/* check for parenthesis imbalance */
	if (expr_nest != 0) {
		return(invexp(exp, "mismatched parentheses")) ;
	}

	/*
		handle forced sub-expressions
		(which MUST be surrounded by parentheses)
	*/
	if (strcmp(exp.tok[0], "(") == 0) {	/* forced sub-expression */
		if (strcmp(exp.tok[exp.len-1], ")") != 0) {
			return(invexp(exp, "missing ')'")) ;
		}
		subexp.tok= exp.tok + 1 ;
		subexp.len= exp.len - 2 ;
		return(subexpr0(subexp)) ;
	}

	if (exp.len != 1) {
		return(invexp(exp, "expecting single token")) ;
	}

	/*
		try to make a number out of the token
	*/
	if (makelong(exp.tok[0], &lnum) == 0) {
		if (addexstream_op(EOPT_PUSHLIT)) return(1) ;
		if (addexstream_long(lnum)) return(1) ;
		return(0) ;
	}

	/*
		if the user has defined a find-variable routine,
		then we try to look up the item as a variable
	*/
	if (pfnfoundvar != (FNUSRFINDVAR) 0) {
		if (pfnfoundvar(exp.tok[0], &var_id)) {
			if (addexstream_op(EOPT_PUSHVAR)) return(1) ;
			if (addexstream_short((short) var_id)) return(1) ;
			return(0) ;
		}
	}

	/*
		The token is unknown
	*/
	return(invexp(exp, "undefined")) ;
}	


/*
	static internal routines
*/

static int assignexp(int i, subexpr_t exp)
{
	subexpr_t subexp ;
	int var_id ;

	if (pfnfoundvar == (FNUSRFINDVAR) 0) {
		return(invexp(exp, "'=' not allowed")) ;
	}
	if (i != 1) {
		return(invexp(exp, "misplaced '='")) ;
	}
	if (!pfnfoundvar(exp.tok[0], &var_id)) {
		return(invexp(exp, "undefined lval")) ;
	}

	subexp.tok= exp.tok+i+1 ;
	subexp.len= exp.len-i-1 ;
	if (subexpr0(subexp)) return(1) ;
	if (addexstream_op(EOPT_SETVAR)) return(1) ;
	if (addexstream_short((short) var_id)) return(1) ;
	return(0) ;
}

static int makelong(char *s, long *lnum_p)
{
	if (pfnmakelong != (FNUSRMAKELONG) 0) {
		return(pfnmakelong(s, lnum_p)) ;
	}
	if (sscanf(s, "%ld", lnum_p) != 1) return(1) ;
	return(0) ;
}

static int invexp(subexpr_t exp, char *why)
{
	int i ;

	fprintf(stderr, "subexpr: %s: '", why) ;
	for (i= 0; i< exp.len; i++) {
		if (i> 0) fprintf(stderr, " ") ;
		fprintf(stderr, "%s", exp.tok[i]) ;
	}
	fprintf(stderr, "'\n") ;
	return(1) ;
}

static void showexp(char *prefix, subexpr_t exp)
{
	int i ;

	printf(prefix) ;
	for (i= 0; i< exp.len; i++) {
		printf(" %s", exp.tok[i]) ;
	}
	printf("\n") ;
}

/*

	exstream routines

*/

static int addexstream_op(int op)
{
	return(addexstream_byte((unsigned char) op)) ;
}

/* static */ int dump_exstream(unsigned char *exstream_fp, int exstream_len)
{
	int i, op ;

	i= 0;
	while(i< exstream_len) {

		op= exstream_fp[i++] ;

		switch(op) {
			
			case EOPT_UNEG:		showop("neg") ;		break ;
			case EOPT_UPOS:		showop("pos") ;		break ;
			case EOPT_UNOT:		showop("not") ;		break ;
			case EOPT_UADDR:	showop("addr-of") ;	break ;
			case EOPT_UCPL:		showop("cpl") ;		break ;
			case EOPT_UDEREF:	showop("deref") ;	break ;

			case EOPT_TESTEQ:	showop("==") ; 		break ;
			case EOPT_TESTNE:	showop("!=") ; 		break ;
			case EOPT_TESTLT:	showop("<") ;  		break ;
			case EOPT_TESTGT:	showop(">") ;  		break ;
			case EOPT_TESTLE:	showop("<=") ; 		break ;
			case EOPT_TESTGE:	showop(">=") ; 		break ;
			case EOPT_LOGAND:	showop("&&") ; 		break ;
			case EOPT_LOGOR:	showop("||") ; 		break ;
			case EOPT_LOGXOR:	showop("^^") ; 		break ;
			case EOPT_BINAND:	showop("&") ;  		break ;
			case EOPT_BINOR:	showop("|") ;  		break ;
			case EOPT_BINXOR:	showop("^") ;  		break ;
			case EOPT_MUL:		showop("*") ;  		break ;
			case EOPT_DIV:		showop("/") ;  		break ;
			case EOPT_MOD:		showop("%") ;  		break ;
			case EOPT_ADD:		showop("+") ;  		break ;
			case EOPT_SUB:		showop("-") ;  		break ;
			case EOPT_SHL:		showop("<<") ;  	break ;
			case EOPT_SHR:		showop(">>") ;  	break ;

			case EOPT_3COND:	showop("?:") ; 		break ;

			case EOPT_PUSHLIT:
				printf(
					"%ld ",
					(long) (*
						((unsigned long *)
							(exstream_fp+i)
						)
					)
				) ;
				i+= sizeof(unsigned long) ;
				break ;

			case EOPT_PUSHVAR:
				printf(
					"[var#%d] ",
					(int) (*
						((unsigned short *)
							(exstream_fp+i)
						)
					)
				) ;
				i+= sizeof(unsigned short) ;
				break ;

			case EOPT_SETVAR:
				printf(
					"[set var#%d]",
					(int) (*
						((unsigned short *)
							(exstream_fp+i)
						)
					)
				) ;
				i+= sizeof(unsigned short) ;
				break ;

			default:
				fprintf(stderr, "unknown operator: %d\n", op) ;
				return(1) ;
				break ;			
		}
	}

	printf("\n") ;
	return(0) ;
}

/*
	static internal routines
*/

#define	ADDSTREAM_MACRO(t, v)							\
	if ((exstream_len + sizeof(t))> exstream_size) {		\
		return(overflow_abort()) ;				\
	}								\
	*((t *) (exstream + exstream_len))= v ;				\
	exstream_len+= sizeof(t)

static int addexstream_byte(unsigned char b)
{
	ADDSTREAM_MACRO(unsigned char, b) ;
	return(0) ;
}

static int addexstream_short(short s)
{
	ADDSTREAM_MACRO(short, s) ;
	return(0) ;
}

static int addexstream_long(long l)
{
	ADDSTREAM_MACRO(long, l) ;
	return(0) ;
}

static int overflow_abort()
{
	fprintf(stderr, "exstream: execution stream overflow\n") ;
	return(1) ;
}

static void showop(char *s)
{
	printf("%s ", s) ;
}

static int getunary_bintok(char *tok)
{
	int i ;

	for (i= 0; intop1_calc[i].src_tok; i++) {
		if (strcmp(intop1_calc[i].src_tok, tok) == 0) {
			return(intop1_calc[i].bin_tok) ;
		}
	}
	return(-1) ;
}

static int getbinop_bintok(char *tok)
{
	int i, j ;
	operator_t *iotp ;

	for (j= 0; (iotp= intop2_table[j]) != NULL; j++) {
		for (i= 0; iotp[i].src_tok; i++) {
			if (strcmp(iotp[i].src_tok, tok) == 0) {
				return(iotp[i].bin_tok) ;
			}
		}
	}
	return(-1) ;
}

/* end of subexpr.c */
