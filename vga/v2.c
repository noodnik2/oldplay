#include	<stdio.h>
#include	<stdlib.h>
#include	<vga.h>
#include	<vgakeyboard.h>

static int bOk= 1 ;

static void keyboard_callback(int scancode, int press)
{
	printf("scan=0x%02X press=%d\n", scancode, press) ;
	if (scancode == 0x01) bOk= 0 ;
	if (scancode == 0x20) bOk= 0 ;
	if (scancode == 0x40) bOk= 0 ;
}

static void testit()
{
#if 0
	keyboard_seteventhandler(keyboard_callback) ; 
#endif
	while(bOk) {
		keyboard_waitforupdate() ;
		if (keyboard_keypressed(SCANCODE_ESCAPE)) bOk= 0 ;
	} ;
#if 0
	keyboard_setdefaulteventhandler() ; 
#endif
}

int main()
{
	if (vga_init()) {
		printf("can't init svgalib\n") ;
		exit(1) ;
	}
	if (keyboard_init()) {
		printf("can't init keyboard\n") ;
		exit(2) ;
	}
	testit() ;
	keyboard_close() ;
	exit(0) ;
}

