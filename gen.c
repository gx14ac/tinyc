#include "token.h"
#include "symbol.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// ASTからAssemblyを再帰的に生成する

int genAST(struct ASTnode *n, int reg) {
	int leftreg, rightreg;

	// registerのIDを取得
	if (n->left) leftreg = genAST(n->left, -1);
	if (n->right) rightreg = genAST(n->right, leftreg);

	switch (n->op) {
		case AST_ADD: // '+'
			return cgadd(leftreg, rightreg);
		case AST_SUBSTRACT: // '-'
			return cgsub(leftreg, rightreg);
		case AST_MULTIPLY: // '*'
			return cgmul(leftreg, rightreg);
		case AST_DIVIDE: // '/'
			return cgdiv(leftreg, rightreg);
		case AST_EQ:
			return cgequal(leftreg, rightreg);
		case AST_NE:
			return cgnotequal(leftreg, rightreg);
		case AST_LT:
			return cglessthan(leftreg, rightreg);
		case AST_GT:
			return cggreaterthan(leftreg, rightreg);
		case AST_LE:
			return cglessequal(leftreg, rightreg);
		case AST_GE:
			return cggreaterequal(leftreg, rightreg);
		case AST_INTLIT: // interger literal
			// レジスタに値をロードし、値をもつレジスタのIDを返す
			return cgloadInt(n->v.intvalue);
		case AST_IDENT:
			return cgloadGlob(Gsym[n->v.id].name);
		case AST_LVIDENT:
			// Gsym[n]に名前がある変数にレジスタを格納する。
			return cgstorGlob(reg, Gsym[n->v.id].name);
		case AST_ASSIGN:
			// 作業はすでに終了しているので、結果を返す
			return rightreg;
		default:
			fprintf(stderr, "unknown ast operator %d\n", n->op);
			exit(1);
	}
}

void genPreamble() {
	cgPreamble();
}

void genPostamble() {
	cgPostamble();
}

void genFreeregs() {
	freeall_registers();
}

void genPrintInt(int reg) {
	cgPrintInt(reg);
}

void genGlobSym(char *s) {
	cgGlobSym(s);
}