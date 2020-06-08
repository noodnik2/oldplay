#include	<stdio.h>
#include	<string.h>

// pg. 91, #1.5

// is s1 same as s2 once a character is inserted at the begining of it?
static int isInsertion(const char *s1, const char *s2) {
	if (*s1 == 0 || *s2 == 0) return 0;
	return (s1[1] == *s2);
}

static int cc0(const char *s1, const char *s2) {

	int r = 0;
	
	char *p1;
	char *p2;
	// ensure that p1 points to the shorter string
	if (strlen(s1) > strlen(s2)) {
		p1 = s1;
		p2 = s2;
	} else {
		p1 = s2;
		p2 = s1;
	}

	while(*p2 != 0) {
		if (*p1 != *p2) {
			if (isInsertion(p1, p2)) {
				p1++;
				r++;
				continue;
			}
			if (isInsertion(p2, p1)) {
				p2++;
				r++;
				continue;
			}
			r++;
		}
		p1++;
		p2++;
	}

	printf("cc(%s, %s) => %s\n", s1, s2, r == 1 ? "true" : "false");
	return r;
}

static int cc(const char *s1, const char *s2) {
	cc0(s1, s2);
	cc0(s2, s1);
}

void main() {

	cc("pale", "ple");
	cc("pales", "pale");
	cc("pale", "bale");
	cc("pale", "bake");

}


