#include	<stdio.h>
#include	<stdlib.h>
#include	<vga.h>

static void testit()
{
	int c ;

	printf("press <ESC> to quit\n") ;
	while(/*CONSTCOND*/1) {
		c= vga_getch() ;
		printf("c=0x%02X\n", c) ;
		if (c == 0x1b) break ;
	}
}

int main()
{
	if (vga_init()) {
		printf("can't init svgalib\n") ;
		exit(1) ;
	}
	vga_setmode(G320x200x256) ;
	testit() ;
	vga_setmode(TEXT) ;
	exit(0) ;
}

