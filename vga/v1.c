#include	<stdio.h>
#include	<stdlib.h>
#include	<vga.h>
#include	<vgagl.h>

static void testit(int mode)
{
	int i, j, gmode ;
	char *gmem ;

	vga_setmode(mode) ;
	gl_setcontextvga(mode) ;

	gmode= vga_getcurrentmode() ;
	gmem= vga_getgraphmem() ;

	gl_setfont(8, 8, gl_font8x8) ;
	gl_setwritemode(FONT_COMPRESSED + WRITEMODE_OVERWRITE) ;
	gl_setfontcolors(0, vga_white()) ;

	for (i= 0; i< 100; i++) {
		for (j= 0; j< 100; j++) {
			gl_printf(
				80 + i, 80 + j,
				"Hello, #{%d,%d}, world\n",
				i, j
			) ;
		}
	}

	sleep(3) ;

	vga_setmode(TEXT) ;

	printf("gmode=%d\n", gmode) ;
	printf("gmem=0x%lX\n", (long) gmem) ;
}

int main()
{
	if (vga_init() != 0) {
		fprintf(stderr, "can't initialize svgalib\n") ;
		exit(1) ;
	}

	testit(G1024x768x256) ;

	exit(0) ;
}

