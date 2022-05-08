// 字句解析を行う関数
int scan(struct token *t);
struct ASTnode *binexpr(int rpb);
struct ASTnode *new_ast_node(int op, struct ASTnode *left, struct ASTnode *right, int intvale);
struct ASTnode *create_ast_leaf(int op, int intvalue);
struct ASTnode *create_ast_unary(int op, struct ASTnode *left, int intvale);
int interpretAST(struct ASTnode *n);
