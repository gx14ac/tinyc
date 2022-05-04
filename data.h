#ifndef extern_
 #define extern_ extern
#endif

// 行数
extern_ int Line;
// 文字列を戻す
extern_ int	Putback;
// 読み込みファイル
extern_ FILE *Infile;
// 入力からスキャンされた最新のトークンを常に持っている
extern_ struct token Token;
