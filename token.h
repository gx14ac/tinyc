#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Token Types
enum {
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_ASTAR,
	TOKEN_SLASH,
	TOKEN_INTLIT // interger literal. この場合は構造体tokenのintvalueに値が入る
};

// Token structure
struct token {
	int token;
	int intvalue;
};
