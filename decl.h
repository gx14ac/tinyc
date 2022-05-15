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
void ident(void);
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

// gen.c
int genAST(struct ASTnode *n, int reg);
void genPreamble();
void genPostamble();
void genFreeregs();
void genPrintInt(int reg);
void genGlobSym(char *s);

// cg.c
void freeall_registers(void);
void cgPreamble(void);
void cgPostamble(void);
void cgPrintInt(int r);
int cgloadInt(int value);
int cgloadGlob(char *identifier);
void cgGlobSym(char *sym);
int cgstorGlob(int r, char *identifier);
int cgadd(int r1, int r2);
int cgmul(int r1, int r2);
int cgsub(int r1, int r2);
int cgdiv(int r1, int r2);
int cgequal(int r1, int r2);
int cgnotequal(int r1, int r2);
int cglessthan(int r1, int r2);
int cggreaterthan(int r1, int r2);
int cglessequal(int r1, int r2);
int cggreaterequal(int r1, int r2);

// sym.c
int findGlob(char *s);
int addGlob(char *name);

// decl.c
void var_declaration(void);