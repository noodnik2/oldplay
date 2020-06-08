/*

*/

#include	<stdio.h>
#include	<ctype.h>

void main()
{
	FILE *ebcfp, *ascfp;
	int e, a, print;

	if ((ebcfp= fopen("ebc.map", "r")) == (FILE *)NULL) {
		fprintf(stderr, "can't open ebcdic map: ebc.map\n");
		exit(1);
	}
	if ((ascfp= fopen("asc.map", "r")) == (FILE *)NULL) {
		fprintf(stderr, "can't open ascii map: ascii.map\n");
		exit(2);
	}
	for (;;) {
		if ((e= getc(ebcfp))==EOF) break;
		if ((a= getc(ascfp))==EOF) break;
		print= (isprint(a) && !isspace(a));
		if (print) printf("%d %c\n", e, (char)a);
		else		printf("%d %02d\n", e, a);
	}
	fclose(ascfp);
	fclose(ebcfp);
}	
