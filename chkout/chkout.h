/*
	chkout.h


*/

#define		FN_SIZE		72
#define		BFN_SIZE	14
#define		BASE_FN		"chkout"

#define		DEFAULT_MSPEC	"600"

#define		UNAME_SIZE	8		/* size of user name */
#define		GNAME_SIZE	8		/* size of group name */
#define		MSPEC_SIZE	16		/* size of mode spec */
#define		DNAME_SIZE	64		/* size of device name */
#define		GLIST_SIZE	64		/* size of grp acc list */
#define		TIME_SIZE	35		/* size of timestamp */

#define		TFN_SIZE	FN_SIZE		/* temporary filename size */
#define		TFN_TEMPLATE	"updXXXXXX"	/* temporary filename format */
#define		LFN_FORMAT	"L%s"

#define		ROOT_UID	0		/* on must systems */

#define		CHK_DIR		"/usr/local/chkout"
#define		DIR_SIZE	64

#define		RFN_FORMAT	"%s.res"	/* name of resource file */
#define		RFN_ID		"resource"	/* reg file "id" */
#define		RFN_SIZE	FN_SIZE		/* reg fname size */
#define		RFNE_MAX	1000		/* max # entries */
#define		RF_RECSIZE	256		/* max size of record */
#define		RF_IDSIZE	10		/* size of ID string */

#define		CFN_FORMAT	"%s.status"	/* name of chkout file */
#define		CFN_ID		"status"	/* chk file "id" */
#define		CFN_SIZE	FN_SIZE		/* chk file size */
#define		CFNE_MAX	1000		/* max # entries */
#define		CF_RECSIZE	256		/* max size of record */
#define		CF_IDSIZE	10		/* size of ID string */

#define		COC_UNKNOWN		0	/* no command specified */
#define		COC_CHKOUT		1	/* chkout devices command */
#define		COC_RETURN		2	/* return devices command */
#define		COC_QUERY		3	/* query devices command */
#define		COC_LIST		4	/* list devices command */
#define		COC_USAGE		5	/* report usage diagram */

#define		CHK_ERR_BADARG		12	/* invalid argument error */
#define		CHK_ERR			100	/* internal error code base */

/*

*/

typedef struct {
	int n ;
	char **list ;
} co_nlist ;

/*
	co_cmd contains the specifics of the command
*/
typedef struct {
	int cmd ;				/* command to be performed */
	char mspec[MSPEC_SIZE+1] ;		/* mode specification */
	char by_uname[UNAME_SIZE+1] ;		/* user issuing command */
	char by_gname[GNAME_SIZE+1] ;		/* group of user issuing cmd */
	char for_uname[UNAME_SIZE+1] ;		/* user command on behalf of */
	char for_gname[GNAME_SIZE+1] ;		/* group command on behalf of */
	char cmd_time[TIME_SIZE+1] ;		/* command invocation time */
	int by_uid ;				/* uid(by_uname) */
	int by_gid ;				/* gid(by_gname) */
	int for_uid ;				/* uid(for_uname) */
	int for_gid ;				/* gid(for_gname) */
	int mode ;				/* value(mode specification) */
	char **dnlist ;				/* device list */
	int ndevs ;				/* number of devices in list */
} co_cmd ;

/*
	co_parms contains the run-time variables of the chkout command.
	(not fully qualified) 
*/

typedef struct {
	char basefile[BFN_SIZE+1] ;
	char temp_basefile[TFN_SIZE+1] ;
	char regfile[RFN_SIZE+1] ;
	char chkfile[CFN_SIZE+1] ;
	char lock_regfile[RFN_SIZE+1] ;
	char lock_chkfile[CFN_SIZE+1] ;
	char temp_regfile[RFN_SIZE+1] ;
	char temp_chkfile[CFN_SIZE+1] ;
	char regid[RF_IDSIZE+1] ;
	char chkid[CF_IDSIZE+1] ;
	char chkdir[DIR_SIZE+1] ;
	int root_uid ;
	int debug ;				/* debug flag */
} co_parms ;

/*
	co_deve contains resource status information.
*/

typedef struct {
	int uid ;				/* resource owner: user */
	int gid ;				/* resource owner: group */
	unsigned mode ;				/* resource mode: chmod */
} co_deve ;

/*
	co_updf contains information used to update a file.
	(fully qualified)
*/

typedef struct {
	char 	  regfn[FN_SIZE+1] ;
	char      chkfn[FN_SIZE+1] ;
	char lock_regfn[FN_SIZE+1] ;
	char lock_chkfn[FN_SIZE+1] ;
	char temp_regfn[FN_SIZE+1] ;
	char temp_chkfn[FN_SIZE+1] ;
	co_deve *rst_deplist ;
	char **rst_dnlist ;
	int rst_dnlist_len ;
} co_updf ;

/*
	co_rlist_entry holds information from an entry of the resource file.
	co_rlist holds the entire resource file.
*/

typedef struct {
	char dname[DNAME_SIZE+1] ;		/* device   */
	char uname[UNAME_SIZE+1] ;		/* user id  */
	char gname[GNAME_SIZE+1] ;		/* group id */
	char mspec[MSPEC_SIZE+1] ; 		/* modespec */
	char glist[GLIST_SIZE+1] ;		/* grp list */
	co_deve de ;				/* details  */
} co_rlist_entry ;

typedef struct {
	char **dnlist ;				/* device list */
	co_rlist_entry *rlep ;			/* reg entries */
	int nentries ;				/* # reg ent's */
	int maxentries ;			/* max # r.e's */
} co_rlist ;

/*
	co_clist_entry holds information from an entry of the checkout file
	co_clist holds the entire checkout file.
*/

typedef struct {
	char dname[DNAME_SIZE+1] ;		/* device */
	char uname[UNAME_SIZE+1] ;		/* user that checked it out */
	char timeout[TIME_SIZE+1] ;		/* time device checked out */
	int to_uid ;				/* user to whom checked out */
	co_deve de ;				/* details  */
	short dnf ;				/* device not found */
} co_clist_entry ;

typedef struct {
	char **dnlist ;				/* device list */
	co_clist_entry *clep ;			/* chkout entries */
	int nentries ;				/* # chkout entries */
	int maxentries ;			/* max # chkout entries */
} co_clist ;
 
