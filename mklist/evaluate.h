/*	Revision:	10
*/

/*

	evaluate.h	 		C Header: Expression Evalaution
					Copyright (C) 1994, Contahal Ltd.

*/

#ifndef	EVALUATE_H_INC
#define	EVALUATE_H_INC

/*
	#includes
*/

/*
	#defines
*/
#define	EVALUATEE_SUCCESS		0

/*
	typedefs
*/
typedef int EVALUATEE ;
typedef int (*PFNRETUSRVARLONG)(int vid, long *plval) ;
typedef int (*PFNSETUSRVARLONG)(int vid, long lval) ;

/*
	function decls
*/

int evaluate_setpfngetusrvarlong(PFNRETUSRVARLONG usr_pfngetusrvarlong) ;
int evaluate_setpfnsetusrvarlong(PFNSETUSRVARLONG usr_pfnsetusrvarlong) ;
int evaluate_setlongstack(long *usr_longstack, int usr_longstack_size) ;
int evaluate_exstream_long(
	unsigned char *exstream_fp,
	int exstream_len,
	long *long_result_fp
) ;

/* #ifndef EVALUATE_H_INC */
#endif

/* end of evaluate.h */
