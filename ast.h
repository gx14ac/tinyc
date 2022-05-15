// AST node types
enum {
	AST_ADD=1, // '+'
	AST_SUBSTRACT, // '-'
	AST_MULTIPLY, // '*'
	AST_DIVIDE, // '/'

	// ==, !=, <, >, <=, >=
	AST_EQ, AST_NE, AST_LT, AST_GT, AST_LE, AST_GE,

	AST_INTLIT, // interger literal

	AST_IDENT, AST_LVIDENT, AST_ASSIGN
};

// Abstract Syntax Tree structure
// op値A_INTLITを持つASTノードは整数値を表す
// これはサブツリーの子を持たず、intvalueフィールドに値を持つだけ
// 左と右のツリー. 後で、子木の値を加算したり減算したりするために右と左で分ける
struct ASTnode {
	int op; // ツリーに対して行う操作
	struct ASTnode *left;
	struct ASTnode *right;
	union {
		int intvalue; // AST_INTLITの場合の整数値
		int id; // AST_IDNETの場合のシンボルテーブルの位置
	} v;
};
