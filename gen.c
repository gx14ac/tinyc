#include "token.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// ASTからAssemblyを再帰的に生成する
static int genAST(struct ASTnode *n) {
	int leftreg, rightreg;

	// registerのIDを取得
	if (n->left) leftreg = genAST(n->left);
	if (n->right) rightreg = genAST(n->right);

	switch (n->op) {
		case AST_ADD: // '+'
			return cgadd(leftreg, rightreg);
		case AST_SUBSTRACT: // '-'
			return cgsub(leftreg, rightreg);
		case AST_MULTIPLY: // '*'
			return cgmul(leftreg, rightreg);
		case AST_DIVIDE: // '/'
			return cgdiv(leftreg, rightreg);
		case AST_INTLIT: // interger literal
			// レジスタに値をロードし、値をもつレジスタのIDを返す
			return cgload(n->intvalue);
		default:
			fprintf(stderr, "unknown ast operator %d\n", n->op);
			exit(1);
	}
}

void generateCode(struct ASTnode *n) {
	int reg;

	cgPreamble();
	reg = genAST(n);
	// 結果を持つレジスタをint型として表示
	cgPrintInt(reg);
	cgPostamble();
}
