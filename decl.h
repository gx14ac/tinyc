// 字句解析を行う関数
// scan.c
int scan(struct token *t);

// expr.c
struct ASTnode *binexpr(int rpb);

// tree.c
struct ASTnode *new_ast_node(int op, struct ASTnode *left, struct ASTnode *right, int intvale);
struct ASTnode *create_ast_leaf(int op, int intvalue);
struct ASTnode *create_ast_unary(int op, struct ASTnode *left, int intvale);

// interp.c
int interpretAST(struct ASTnode *n);

// stmt.c
void statements(void);

// misc.c
void match(int t, char *what);
void semi(void);

// gen.c
int genAST(struct ASTnode *n);
void genPreamble();
void genPostamble();
void genFreeregs();
void genPrintInt(int reg);

// cg.c
void freeall_registers(void);
void cgPreamble(void);
void cgPostamble(void);
void cgPrintInt(int r);
int cgload(int value);
int cgadd(int r1, int r2);
int cgmul(int r1, int r2);
int cgsub(int r1, int r2);
int cgdiv(int r1, int r2);