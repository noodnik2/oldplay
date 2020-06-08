#include	<unistd.h>
#include	<sys/mman.h>
#include	<stdio.h>
#include	<fcntl.h>
#include	<errno.h>

#pragma pack(1)
typedef struct _cell_tag {
	unsigned char value ;
	unsigned char attrib ;
} _cell_t ;
#pragma

typedef struct _scn_tag {
	int rows ;
	int cols ;
	_cell_t *pBuf ;
} _scn_t ;

static int clearscn(_scn_t *pScn)
{
	_cell_t cs, *pcs ; 
	int i, j ;

	cs.attrib= 0x70 ;
	cs.value= ' ' ;
	pcs= pScn->pBuf ;
	for (i= 0; i< pScn->rows; i++) {
		for (j= 0; j< pScn->cols; j++) {
			(*pcs++)= cs ;
		}
	}
	return(0) ;
}

static int refreshscn(int fd, _scn_t *pScn)
{
	size_t sw ;

	sw= (size_t) (pScn->rows * pScn->cols) * sizeof(_cell_t) ;
	if (write(fd, pScn->pBuf, sw)< sw) {
		return(-1) ;
	}
	return(0) ;
}

static int drawscn(_scn_t *pScn)
{
	int i, j, k ;
	_cell_t *pcs ; 
	char *p, *pText= "256 character codes:" ;

	pcs= pScn->pBuf + (pScn->cols * 10) + 10 ;
	for (p= pText; *p; p++) {
		pcs->value= (unsigned char) *p ;
		pcs++ ;
	}

	k= 0 ;
	for (i= 0; i< 4; i++) {
		pcs= pScn->pBuf + (pScn->cols * (12 + i)) + 10 ;
		for (j= 0; j< 64; j++) {
			pcs->value= (unsigned char) k++ ;
			pcs++ ;
		}
	}

	return(0) ;
}

static int doit(int fd, int rows, int cols)
{
	_scn_t *pScn ;
	size_t ssize ;

	ssize= sizeof(_scn_t) +	(rows * cols * sizeof(_cell_t)) ;
	pScn= (_scn_t *) malloc(ssize) ;
	if (!pScn) {
		fprintf(stderr, "can't allocate screen buffer\n") ;
		return(-1) ;
	}
	memset(pScn, '\0', ssize) ;
	pScn->rows= rows ;
	pScn->cols= cols ;
	pScn->pBuf= (_cell_t *) (pScn + 1) ;

	clearscn(pScn) ;
	drawscn(pScn) ;
	refreshscn(fd, pScn) ;

	sleep(10) ;

	free((void *) pScn) ;
	return(0) ;
}

int main()
{
	int i, fd ;
	unsigned char rows, cols ;

	fd= open("/dev/vcsa0", O_RDWR) ;
	if (fd< 0) {
		fprintf(stderr, "can't open virtual console\n") ;
		exit(1) ;
	}

	read(fd, &rows, 1) ;
	read(fd, &cols, 1) ;

	printf("rows=%d, cols=%d\n", (int) rows, (int) cols) ;

	doit(fd, rows, cols) ;

	close(fd) ;
	return(0) ;
}

