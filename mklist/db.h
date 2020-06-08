/*	Revision:		11
*/

#ifndef	DB_H_INC
#define	DB_H_INC

/*

	db.h

*/


/*
	#definitions
*/

#define		DB_RC			int
#define		DBRS_RC			int
#define		DB_BOOL			int

#define		DBRC_SUCCESS		0
#define		DBRC_FAILURE		1
#define		DBRC_NOFILE		2

#define		DBRSRC_SUCCESS		0
#define		DBRSRC_EOF		1
#define		DBRSRC_OVERFLOW		2
#define		DBRSRC_NOTSECTION	3

#define		DBBOOL_FALSE		0
#define		DBBOOL_TRUE		(!DBBOOL_FALSE)

#define		DBMAX_NCOLS		24	/* # of "variables"	*/
#define		DBMAX_DBNAMELEN		255	/* name of "database"	*/
#define		DBMAX_ROWNAMELEN	255	/* name of "db entry"	*/
#define		DBMAX_COLNAMELEN	32	/* name of "db var"	*/
#define		DBMAX_COLVALUELEN	250	/* value of "db var"	*/

#define		DBMAX0_RECORDLEN	1024	/* length of raw db rec	*/


/*
	structures, types
*/

struct db_Col {
	char szName[DBMAX_COLNAMELEN+1] ;
	char szValue[DBMAX_COLVALUELEN+1] ;
} ;

struct db_Row {
	char szName[DBMAX_ROWNAMELEN+1] ;
	int nCols ;
	struct db_Col colArray[DBMAX_NCOLS] ;
} ;

typedef DB_BOOL (*dbFnProcessRow_t)(struct db_Row *, DBRS_RC) ;

struct db_Process {
	char szDBName[DBMAX_DBNAMELEN+1] ;
	dbFnProcessRow_t fnProcessRow ;
} ;

struct db0_SecSpec {
	char *pName ;
	int iNameLen ;
} ;

struct db0_ColSpec {
	char *pName ;
	char *pValue ;
	int iNameLen ;
	int iValueLen ;
} ;


/*
	API entry points
*/

DB_RC dbRead(struct db_Process *pdbp) ;


/* #ifndef DB_H_INC */
#endif

/* end of db.h */
