#include "token.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// すべての主要な文のパースコードを保持
// BNF文法を解析する再帰的な関数
void statements(void) {
    struct ASTnode *tree;
    int reg;

    while (1) {
        match(TOKEN_PRINT, "print");

        tree = binexpr(0);
        reg = genAST(tree);
        genPrintInt(reg);
        genFreeregs();

        semi();
        if (Token.token == TOKEN_EOF) {
            return;
        }
    }
}