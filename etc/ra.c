#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/acct.h>

char afile[]= "/var/adm/pacct" ;

void main()
{
	int fh ;
	
	if ((fh= open(afile, O_RDONLY))< 0) {
		error("can't open accounting file") ;
		exit(1) ;
	}
	
	rdafile(fh) ;

	close(fh) ;
	exit(0) ;
}

static rdafile(fh)
int fh ;
{
	struct acct a ;
	int l ;

	stat_init() ;
	while(
		(l= read(fh, (char *)&a, sizeof(struct acct)))
		== sizeof(struct acct)
	) {
		stat_add(&a) ;
		
	}
	if (l != 0) {
		error("warning: premature EOF") ;
	}
	stat_fini() ;
}

static stat_init()
{
	printf("dictionary {\n") ;
	printf("\tstr10\tcmd\t\"command name\"\n") ;
	printf("\tint\tstat\t\"exit status\"\n") ;
	printf("\tlong\ttty\t\"controlling terminal\"\n") ;
	printf("\tint\tuid\t\"user id\"\n") ;
	printf("\tint\tgid\t\"group id\"\n") ;
	printf("\tlong\tbtime\t\"beginning time\"\n") ;
	printf("\tdouble\tutime\t\"user time\"\n") ;
	printf("\tdouble\tstime\t\"system time\"\n") ;
	printf("\tdouble\tetime\t\"elapsed time\"\n") ;
	printf("\tdouble\tchio\t\"chars transferred\"\n") ;
	printf("\tdouble\tblkio\t\"blocks transferred\"\n") ;
	printf("\tdouble\tmem\t\"memory usage\"\n") ;
	printf("}\n") ;
}

static stat_add(ap)
struct acct *ap ;
{
	char name[sizeof(ap->ac_comm)+1] ;
	char ios[25] ;
	char rws[25] ;
	char mem[25] ;
	char utime[25] ;
	char stime[25] ;
	char etime[25] ;

	memcpy(name, ap->ac_comm, sizeof(ap->ac_comm)) ;
	name[sizeof(ap->ac_comm)]= '\0' ;
	comp2str(ap->ac_io, ios) ;
	comp2str(ap->ac_rw, rws) ;
	comp2str(ap->ac_mem, mem) ;
	comp2str(ap->ac_utime, utime) ;
	comp2str(ap->ac_stime, stime) ;
	comp2str(ap->ac_etime, etime) ;
	printf(
		"\"%s\" %d %ld %d %d %ld %s %s %s %s %s %s\n",
		name,
		(int)ap->ac_stat,
		(long)ap->ac_tty,	
		(int)ap->ac_uid,	
		(int)ap->ac_gid,	
		(long)ap->ac_btime,
		utime,
		stime,
		etime,
		ios,
		rws,
		mem
	) ;
}

static stat_fini()
{
}

static error(msg)
char *msg ;
{
	fprintf(stderr, "ra: %s\n", msg) ;
}

static comp2str(c, s)
comp_t c ;
char *s ;
{
	unsigned m, e ;

	m= e= (unsigned) c ;

#if 0
	m&= 0xFFF8 ;	m>>= 3 ;
	e&= 0x0007 ;
#else
	m&= 0x1FFF ;
	e&= 0xE000 ;	e>>= 13 ;
#endif

	if (e == 0) {
		sprintf(s, "0.%u", m) ;
	}
	else {
		sprintf(s, "0.%ue%u", m, e) ;
	}
}
