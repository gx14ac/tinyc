#include "token.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

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

// 二項演算子をルートとするASTツリーを返却する
// 2 * 3 + 4 * 5のような式の場合は以下のようなツリーになるだろう
/*
     *
    / \
   2   +
      / \
     3   *
        / \
       4   5
*/
struct ASTnode *binexpr(void) {
	struct ASTnode *n, *left, *right;
	int nodetype;
	
	// 左側の整数リテラルを取得する。
  	// 同時に次のトークンを取得。
	left = primary();

	// トークンが残っていない場合、左のノードだけを返す
	if (Token.token == TOKEN_EOF) {
		return left;
	}

	// トークンをノードに変換
	nodetype = artihop(Token.token);

	// 次のトークンを取得
	scan(&Token);

	// 右辺の木を再帰的に取得する
	right = binexpr();

	// 両方のサブツリーを持つツリーを構築する
	n = new_ast_node(nodetype, left, right, 0);

	return n;
}
