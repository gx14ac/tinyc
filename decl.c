#include "token.h"
#include "symbol.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// 変数宣言を解析する
void var_declaration(void) {
    // intトークンの後に識別子とセミコロンがあることを確認する
    // テキストには識別子の名前が入る
    // これを既知の識別子として追加
    match(TOKEN_INT, "int");
    ident();
    addGlob(Text);
    genGlobSym(Text);
    semi();
}