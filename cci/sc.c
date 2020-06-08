#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

// pg. 91, #1.6 (String Compression)

struct RunCount {
	int c;	// character
	int n;	// count
};

static void flushRunCount(struct RunCount * const prc, int c) {
	if (c == 0) {
		return;
	}
	if (prc->n > 2) {
		printf("%d%c", prc->n, prc->c);
	} else {
		for (int i = 0; i < prc->n; i++) putchar(prc->c);
	}
	prc->c = c;
	prc->n = 1;
}
	

void main(const int argc, const char * const argv[]) {
	
	if (argc != 2) {
		printf("give a single argument\n");
		exit(1);
	}

	const char *s = argv[1];
	
	struct RunCount rc; memset(&rc, '\0', sizeof(rc));
	for (const char *p = s; *p; p++) {
		if (rc.c != 0) {
			if (rc.c == *p) {
				rc.n++;
				continue;
			}
			flushRunCount(&rc, *p);
			continue;
		}
		rc.c = *p;
		rc.n++;
	}
	if (rc.c != 0) {
		putchar(rc.c);
	}

	flushRunCount(&rc, 0);
	printf("\n");
}
