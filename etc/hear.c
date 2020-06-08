
#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/time.h>

char play_prog[]= "/usr/openwin/demo/online_intro/play" ;
char play_textpfx[]= "/usr/openwin/demo/online_intro/sounds/" ;
char play_textext[]= ".au" ;
char temp_file[]= "/tmp/iterm.log" ;

#define	SI_CUCKOO	0
#define	SI_DRIP		1
#define	SI_LAUGH	2
#define	SI_ROOSTER	3
#define	SI_SPACEMUSIC	4
#define	SI_TRAIN	5
char *sounds[]= {
	"cuckoo",
	"drip",
	"laugh",
	"rooster",
	"spacemusic",
	"train"
} ;
#define	N_SOUNDS	(sizeof(sounds)/sizeof(sounds[0]))

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char cmdline[1024] ;
	int sound_index, next_time, fh ;
	char *cmd_name, *cmd_arg ;

	cmd_name= argv[0] ;

	if (argc>= 2) {
		if ((fh= open(temp_file, O_RDONLY))>= 0) {
			close(fh) ;
			cmd_arg= argv[1] ;
			sprintf(
				cmdline,
				"%s %s%s%s",
				play_prog,
				play_textpfx,
				cmd_arg,
				play_textext
			) ;
		}
		else {
			sprintf(
				cmdline,
				"> %s",
				temp_file
			) ;
		}
		system(cmdline) ;
	}

	next_time= rn() + 1 ;;

	sound_index=
		(next_time< 20)? SI_DRIP:
		(next_time< 40)? SI_TRAIN:
		(next_time< 60)? SI_CUCKOO:
		(next_time< 80)? SI_ROOSTER:
		(next_time< 100)? SI_LAUGH:
		SI_SPACEMUSIC ;


	sprintf(
		cmdline,
		"echo %s %s | at now + %d minutes 2> /dev/null",
		cmd_name,
		sounds[sound_index],
		next_time
	) ;
	system(cmdline) ;
}

/*

	(int)n= rn() ;

				              NBITS_WIDE
	Returns "random" number from  0  to  2            - 1

*/

#define		NBITS_KEEP	7
#define		NBITS_WIDE	12
#define		NBITS_ROLL	4

static int rn()
{
	time_t t ;
	int i, nbr, seed ;

	time(&t) ;
	seed= (int) t & ((1<<NBITS_WIDE) - 1) ;
	seed+= (int) (t >> NBITS_WIDE) & ((1<<NBITS_WIDE) - 1) ;
	nbr= (int) t & ((1<<NBITS_ROLL) - 1) ;
	for (i= 0; i< nbr; i++) {
		seed= ((seed & 1)<< (NBITS_WIDE-1)) | (seed>>1) ;
	}
	return(seed & ((1<<NBITS_KEEP) - 1)) ;
}
