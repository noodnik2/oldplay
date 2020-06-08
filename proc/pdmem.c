#include	<stdio.h>
#include	<fcntl.h>

#include	<sys/types.h>
#include	<sys/signal.h>
#include	<sys/fault.h>
#include	<sys/syscall.h>
#include	<sys/procfs.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"pd.h"

extern struct opts o ;

static void pdmem_blocks() ;
static void pdmemblock_detail() ;

extern char *malloc() ;

void pdmem(pdap)
pdd_args *pdap ;
{
	char *p ;	
	int npm ;

	if (ioctl(pdap->pidfh, PIOCNMAP, &npm)< 0) {
		pdfnerror(pdap, "can't PIONCMAP") ;
		return ;
	}

	if (npm<= 0) p= (char *)NULL ;
	else {
		if ((p= malloc((npm+1) * sizeof(prmap_t))) == (char *)NULL) {
			pdfnerror(pdap, "can't malloc CMAP") ;
			return ;	
		}
		if (ioctl(pdap->pidfh, PIOCMAP, (prmap_t *)p)< 0) {
			pdfnerror(pdap, "can't PIOCMAP") ;
			free(p) ;
			return ;
		}
	}

	print_add("mem") ;
	pdmem_blocks((prmap_t *)p, npm) ;

	if (p != (char *)NULL) free(p) ;	
}

static void pdmem_blocks(pm_p, npm)
prmap_t *pm_p ;
int npm ;
{
	long size ;
	int i ;

	size= 0L ;
	for (i= 0; i< npm; i++) {
		if (o.detail) pdmemblock_detail(&pm_p[i]) ;
		size+= (long) pm_p[i].pr_size ;
	}
	pdpr_longdec("total size", size) ;
}

struct {
	long flag ;
	char *name ;
} prm_flagnames[]= {
	{ MA_READ, 	"read"		},
	{ MA_WRITE,	"write"		},
	{ MA_EXEC,	"exec"		},
	{ MA_SHARED,	"shared"	},
	{ MA_BREAK,	"break"		},
	{ MA_STACK,	"stack"		},
} ;
#define	N_PRM_FLAGNAMES	(sizeof(prm_flagnames)/sizeof(prm_flagnames[0]))

static void pdmemblock_detail(pm_p)
prmap_t *pm_p ;
{
	int i ;
	char dmsg[1024] ;

	sprintf(
		dmsg,
		"\tA=0x%X S=0x%X O=0x%X F=",
		(unsigned) pm_p[i].pr_vaddr,
		(unsigned) pm_p[i].pr_size,
		(unsigned) pm_p[i].pr_off
	) ;
	print_add(dmsg) ;

	for (i= 0; i< N_PRM_FLAGNAMES; i++) {
		if (pm_p->pr_mflags & prm_flagnames[i].flag) {
			print_add(prm_flagnames[i].name) ;
			print_add(" ") ;
		}
	}
	print_add("\n") ;
}
