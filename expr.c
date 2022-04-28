#include "token.h"
#include "data.h"
#include "decl.h"

// トークンとASTノードの概念を分けて考えたいため
// トークンをASTに変換する
int artihop(int tok) {
	switch (tok) {
		case TOKEN_PLUS:
			return AST_ADD;
		case TOKEN_MINUS:
			return AST_SUBSTRACT;
		case TOKEN_ASTAR:
			return AST_MULTIPLY;
		case TOKEN_SLASH:
			return AST_DIVIDE;
		default:
			// 対応していないトークン
			fprintf(stderr, "unknown token in artihop() on line %d\n", Line);
			exit(1);
	}
}

// トークンが整数リテラルであることをチェックし
// そのリテラル値を保持するASTノードを構築する関数
static struct ASTnode *primary(void) {
	struct ASTnode *n;


	// INTLITトークンに対して
	// それ用のリーフASTノードを作成し、次のトークンをスキャンする
	switch (Token.token) {
		case TOKEN_INTLIT:
			// 葉のASTノード（子を持たないノード）を作る関数
			n = create_ast_leaf(AST_INTLIT, Token.intvalue);
			scan(&Token);
			return n;
		default:
			fprintf(stderr, "syntax error on line %d\n", Line);
			exit(1);
	}

}
