/*      Revision:       1
*/


/*
        types
*/
typedef struct tagTSSTR {
        int len ;
        char *psz ;
} TSSTR ;

/*
	function prototypes
*/

int tsub(
        TSSTR *psOldWord,
        TSSTR *psNewWord,
        TSSTR *psLine,
        int *ip,
        int maxlen
) ;

/* end of tsub.c */
