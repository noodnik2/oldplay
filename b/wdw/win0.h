#define			MAXWINS	10

struct win0 {
	struct uccoord pos;
	struct uccoord size;
	struct uccoord color;		/* Color.x is foreground */
	struct uccoord cursor;
	unsigned char *lbuf;
	} ;
