/*	Revision:	12
*/
/*

	subexpr.h			C Header: Sub-Expression Compiler
					Copyright (C) 1994, Marty Ross

*/

/*
	Expression Operand Types
*/

#define		EOPTR_STARTUOP		10	/* start of unary ops	*/
#define		EOPT_UNEG		11
#define		EOPT_UPOS		12
#define		EOPT_UNOT		13
#define		EOPT_UCPL		14
#define		EOPT_UADDR		15
#define		EOPT_UDEREF		16
#define		EOPTR_ENDUOP		17	/* end of unary ops */

#define		EOPTR_STARTBOP		20	/* start of binary ops	*/
#define		EOPT_TESTEQ		20
#define		EOPT_TESTNE		21
#define		EOPT_TESTLT		22
#define		EOPT_TESTGT		23
#define		EOPT_TESTLE		24
#define		EOPT_TESTGE		25
#define		EOPT_LOGAND		30
#define		EOPT_LOGOR		31
#define		EOPT_LOGXOR		33
#define		EOPT_BINAND		40
#define		EOPT_BINOR		41
#define		EOPT_BINXOR		42
#define		EOPT_MUL		50
#define		EOPT_DIV		51
#define		EOPT_MOD		52
#define		EOPT_ADD		60
#define		EOPT_SUB		61
#define		EOPT_SHL		70
#define		EOPT_SHR		71
#define		EOPTR_ENDBOP		79	/* end of binary ops */

#define		EOPTR_START3OP		80	/* start tertiary ops	*/
#define		EOPT_3COND		80	/* conditional operator	*/
#define		EOPTR_END3OP		89	/* start tertiary ops	*/


#define		EOPT_PUSHLIT		100	/* push literal value */
#define		EOPT_PUSHVAR		101	/* push value of variable */

#define		EOPT_SETVAR		110	/* set value of var */

/*
	structures, typedefs
*/
typedef struct {
	char **tok ;
	int len ;
} subexpr_t ;

typedef int (*FNUSRFINDVAR)(char *vname, int *pvid) ;
typedef int (*FNUSRMAKELONG)(char *s, long *plval) ;

/*
	function declarations
*/
void subexpr_setexstream(unsigned char *usr_exstream, int usr_exstream_len) ;
int subexpr(subexpr_t subexp) ;
int subexpr_exstream_len(void) ;

void subexpr_setmakelong(FNUSRMAKELONG usr_pfnmakelong) ;
void subexpr_setfoundvar(FNUSRFINDVAR usr_foundvar) ;
int subexpr_exstreamlen(void) ;

int dump_exstream(unsigned char *exstream_fp, int exstream_len) ;
int subexpr_setdebug(int level) ;

/* end of subexpr.h */
