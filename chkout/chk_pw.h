/*

	chk_pw.h		Group and User Password File Access

*/


#define		CUN_SIZE	L_cuserid	/* max size of user name */
#define		CGN_SIZE	8		/* max size of group name */
#define		CGA_MAXAUTHS	16		/* max # authorizations */

typedef struct {
	int uid ;			/* current uid for user */
	int gid ;			/* current gid for user */
} cpwpent ;

typedef struct {
	int gid ;			/* corresponding gid */
} cpwgent ;

typedef struct {
	int nauths ;
	char uname[CGN_SIZE+1][CGA_MAXAUTHS] ;
} cpwgauth ;

char *chk_pw_cuname() ;

cpwpent *chk_pw_pebyuname() ;
char *chk_pw_pebyuid() ;

cpwgent *chk_pw_gebygname() ;
char *chk_pw_gebygid() ;

cpwgauth *chk_pw_gauthbygid() ;
cpwgauth *chk_pw_gauthbygname() ;

