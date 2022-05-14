#include "token.h"
#include "symbol.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// グローバルシンボルの位置
static int Globs = 0;

// sがグローバルシンボルテーブルにあるか確認する
// あればその位置を返し、なければ-1を返す
int findGlob(char *s) {
    int i;
    for (i = 0; i < Globs; i++) {
        if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name)) {
            return i;
        }
    }

    return -1;
}

// 新しいグローバルシンボルスロットの位置を取得し、位置を使い果たした場合はfatalする
static int newGlob(void) {
    int p;

    if ((p = Globs++) >= NSYMBOLS) {
        fatal("too many global sysmbols");
    }

    return p;
}

// 新しくシンボルテーブルを作成する。存在する場合は既存のシンボルテーブルの位置を返す
int addGlob(char *name) {
    int y;

    // 既にシンボルテーブルある場合は既存の位置を返す
    if ((y = findGlob(name)) != -1) {
        return y;
    }
    
    // そうでなければ新しいシンボルテーブルを作る
    y = newGlob();
    Gsym[y].name = strdup(name);
    return y;
}