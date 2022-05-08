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
// その整数リテラル値を保持するASTノードを構築する関数
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

// '*' または '/' の二項演算子をルートとするASTツリーを返す
// additive_expr()と似ていますが、
// primary()を呼んで整数リテラルを取得している点が異なる
// 優先順位の高い演算子、 '*' や '/' 演算子があるときだけループさせるようにしている。
// 優先順位の低い演算子にぶつかったら、その時点までで作ったサブツリーを返す。
// 優先順位の低い演算子を処理するためadditive_expr() に戻るため
struct ASTnode *multiplicative_expr(void) {
	struct ASTnode *left, *right;
	int tokentype;

	// 左側の整数リテラルを取得。
  	// 同時に次のトークンを取得。
	left = primary();

	// トークンがもうなければ、左のノードだけ返す
	tokentype = Token.token;
	if (tokentype == TOKEN_EOF) {
		return left;
	}

	while ((tokentype == TOKEN_ASTAR || (tokentype == TOKEN_SLASH))) {
		// 次の整数リテラルを取得。
		scan(&Token);
		right = primary();

		// 左の整数リテラルと結合する
		left = new_ast_node(artihop(tokentype), left, right, 0);

		tokentype = Token.token;
    	if (tokentype == TOKEN_EOF) {
      		break;
		}
	}

	return left;
}

// '+' または '-' の二項演算子をルートとするASTツリーを返す
/*
 最初の演算子が高い優先順位の '*' または '/' である場合に備えて、先に multiplicative_expr() を呼び出しています。この関数は、低い優先順位の '+' または '-' 演算子に出会ったときだけ返されます。
*/
struct ASTnode *additive_expr(void) {
	struct ASTnode *left, *right;
	int tokentype;

	// 左のサブツリーを我々より高い優先度で取得する
	left = multiplicative_expr();
	tokentype = Token.token;

	// トークンがもうなければ、左のノードだけ返す
	if (tokentype == TOKEN_EOF) {
		return left;
	}

	// 自分より高い優先順位のトークンに対してループ処理を行う
	// whileループに入った時点で、'+'または'-'オペレータがあることがわかる。
	// 入力にトークンがなくなるまで、つまりTOKEN_EOFトークンにぶつかるまでループする。
	while (1) {
		// 次の整数リテラルを取り込む
		scan(&Token);

		// 自分より高い優先順位で正しいサブツリーを取得する
		right = multiplicative_expr();

		// 2つのサブツリーを低優先度演算子で結合する
		left = new_ast_node(artihop(tokentype), left, right, 0);

		// 我々の優先順位で取得する
		tokentype = Token.token;
		if (tokentype == TOKEN_EOF) {
			break;
		}
	}

	// 作成したツリーを返す
	return left;
}

struct ASTnode *binexpr(int n) {
  return (additive_expr());
}

