#include "ast.h"
#include "symbol.h"
#include "token.h"
#include "data.h"
#include "decl.h"

void print_statement(void) {
    struct ASTnode *tree;
    int reg;
    
    // 今保持しているTokenと合っているか確認
    match(TOKEN_PRINT, "print");

    // generate assembly code
    tree = binexpr(0);
    reg = genAST(tree, -1);
    genPrintInt(reg);
    genFreeregs();

    semi();
}

void assignment_statement(void) {
    struct ASTnode *left, *right, *tree;
    int id;

    // 識別子を確保する
    ident();

    // 識別子が定義されていることを確認し、そのリーフノードを作成します。
    if ((id = findGlob(Text)) == -1) {
        fatals("undeclared variable", Text);
    }
    // LVIDENTはleft value identifierの略
    // rightに代入する理由はこのノードがlvalueを表すためである。
    // lvalueとは特定の場所に紐づいた値のこと。 つまり変数の値を保持するメモリ上のアドレスのことを指す
    right = create_ast_leaf(AST_LVIDENT, id);

    // = 符号があることを確認する
    match(TOKEN_ASSIGN, "=");

    // 式をパースして、ツリーを生成
    left = binexpr(0);

    tree = new_ast_node(AST_ASSIGN, left, right, 0);

    genAST(tree, -1);
    genFreeregs();

    semi();
}

// すべての主要な文のパースコードを保持
// BNF文法を解析する再帰的な関数
void statements(void) {
    struct ASTnode *tree;
    int reg;

    while (1) {
      switch (Token.token) {
      case TOKEN_PRINT:
        print_statement();
        break;
      case TOKEN_INT:
        var_declaration();
        break;
      case TOKEN_IDENT:
        assignment_statement();
        break;
      case TOKEN_EOF:
        return;
      default:
        fatald("Syntax error, token", Token.token);
      }
    }
}