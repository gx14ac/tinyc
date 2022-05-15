#include "token.h"
#include "symbol.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// 各トークンの優先順位
static int OpPrec[] = {
	0, 10, 10, // TOKEN_EOF, TOKEN_PLUS, TOKEN_MINUS
	20, 20, // TOKEN_STAR, TOKEN_SLASH
	30, 30, // TOKEN_EQUALS, TOKEN_NE
	40, 40, 40, 40 // TOKEN_LT, TOKEN_GT, TOKEN_LE, TOKEN_GE
};

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

// トークンとASTノードの概念を分けて考えたいため
// トークンをASTに変換する
int artihop(int tok) {
	if (tok > TOKEN_EOF && tok < TOKEN_INTLIT) {
		return tok;
	}
	fatald("syntax error, token", tok);
}

// トークンが整数リテラルであることをチェックし
// そのリテラル値を保持するASTノードを構築する関数
static struct ASTnode *primary(void) {
	struct ASTnode *n;
	int id;

	switch (Token.token) {
		// INTLITトークンに対して、そのリーフASTノードを作成
		case TOKEN_INTLIT:
			// 葉のASTノード（子を持たないノード）を作る
			n = create_ast_leaf(AST_INTLIT, Token.intvalue);
			break;
		case TOKEN_IDENT:
			// 識別子が存在するかどうかを確認する
			id = findGlob(Text);
			if (id == -1) {
				fatals("unknown variable", Text);
			}
			// 葉のASTノード（子を持たないノード）を作る
			n = create_ast_leaf(AST_IDENT, id);
			break;
		default:
			fatald("Syntax error, token", Token.token);
	}

	// 次のトークンをスキャンしてリーフノードを返す
	scan(&Token);
	return n;
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

	// このトークンの優先順位が前のトークンの優先順位よりも高い場合
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
