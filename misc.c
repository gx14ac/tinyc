#include "token.h"
#include "data.h"
#include "decl.h"

void match(int t, char *what) {
    if (Token.token == t) {
        scan(&Token);
    } else {
        printf("%s expected on line %d\n", what, Line);
        exit(1);
    }
}

void semi(void) {
    match(TOKEN_SEMI, ";");
}