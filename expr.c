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

// 各トークンの優先順位
static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };

// 二項演算子があることを確認し
// その優先順位を返す。
static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (prec == 0) {
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
    exit(1);
  }

  return prec;
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
// ptp引数は直前のトークンの優先順位である。
struct ASTnode *binexpr(int ptp) {
	struct ASTnode *n, *left, *right;
	int tokentype;
	
	// 左側の整数リテラルを取得する。
  	// 同時に次のトークンを取得。
	left = primary();

	// トークンが残っていない場合、左のノードだけを返す
	tokentype = Token.token;
	if (tokentype == TOKEN_SEMI) {
		return left;
	}

	while (op_precedence(tokentype) > ptp) {
		// 次の整数リテラルを取得
		scan(&Token);
    	
		// トークンの優先順位を指定してbinexpr()を再帰的に呼び出し、サブツリーを構築する。
		right = binexpr(OpPrec[tokentype]);
    	
		// そのサブツリーと結合する。トークンを変換し、ノードを構築
		left = new_ast_node(artihop(tokentype), left, right, 0);

		tokentype = Token.token;
		if (tokentype == TOKEN_SEMI) {
			return left;
		}
	}

	// 優先順位が同じか低い場合のツリーを返す
	return left;
}
