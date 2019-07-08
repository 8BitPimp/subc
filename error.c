/*
 *	NMH's Simple C Compiler, 2011,2012
 *	Error handling
 */

#include "defs.h"
#include "data.h"
#include "decl.h"

static void cleanup(void) {
	if (!O_testonly && NULL != Basefile) {
		remove(newfilename(Basefile, 's'));
		remove(newfilename(Basefile, 'o'));
	}
}

void error(char *s, char *a) {
	if (Syntoken) return;
	if (!Errors) cleanup();
	fprintf(stderr, "error: %s: %d: ", File, Line);
	fprintf(stderr, s, a);
	fprintf(stderr, "\n");
	if (++Errors > 10) {
		Errors = 0;
		fatal("too many errors");
	}
}

void fatal(char *s) {
	error(s, NULL);
	error("fatal error, stop", NULL);
	exit(EXIT_FAILURE);
}

void cerror(char *s, int c) {
	char	buf[32];

	if (isprint(c))
		sprintf(buf, "'%c' (\\x%x)", c, c);
	else
		sprintf(buf, "\\x%x", c);
	error(s, buf);
}

int synch(int syn) {
	int	t;

	t = scan();
	while (t != syn) {
		if (EOF == t)
			fatal("found EOF in error recovery");
		t = next();
	}
	Syntoken = syn;
	return t;
}
