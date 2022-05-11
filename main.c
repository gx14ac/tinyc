#include "token.h"
#define extern_
#include "data.h"
#undef extern_
#include "decl.h"
#include <errno.h>

// initialize global variables
static void init() {
	Line = 1;
	Putback = '\n';
}

static void usage(char *prog) {
	fprintf(stderr, "Usage: %s infile\n", prog);
	exit(1);
}

void main(int argc, char *argv[]) {
	struct ASTnode *n;

	if (argc != 2) {
		usage(argv[0]);
	}

	init();

  	// Open up the input file
  	if ((Infile = fopen(argv[1], "r")) == NULL) {
  	  fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
  	  exit(1);
  	}
  	
  	// Create the output file
  	if ((Outfile = fopen("out.s", "w")) == NULL) {
  	  fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
  	  exit(1);
  	}
  	
  	scan(&Token);			// Get the first token from the input
  	n = binexpr(0);		// Parse the expression in the file
  	printf("%d\n", interpretAST(n));	// Calculate the final result
  	generateCode(n);
  	
  	fclose(Outfile);
  	exit(0);
}
