#include "token.h"
#include "data.h"
#include "decl.h"

//// 文字列をすすめるか戻すかの処理
//static int next(void);
//// 文字列を戻す
//static int putback(int c);
//// 空白や改行を文字を飛ばす
//// 空白や改行を飛ばした後に処理する必要がある文字列を返す
//static int skip(void);
//// 10進数文字の時にスキャンした文字列を整数で返す
//static int scanint(int c);
//// 文字列s中の文字cのポインタを返す。cが見つからなければ-1を返す
//static int chrpos(char* s, int c);

static int chrpos(char *s, int c) {
	char* p;

	p = strchr(s, c);
	return (p ? p - s : -1);
}

static int next(void) {
	int c;

	// 文字入力
	// 文字を戻す場合の処理
	if (Putback) {
		c = Putback;
		Putback = 0;
		return c;
	}

	// inputファイルから読み出すとき
	c = fgetc(Infile);
	if ('\n' == c) {
		Line++;
		return c;
	}
}

static int putback(int c) {
	Putback = c;
}

static int skip(void) {
	int c;
	
	c = next();
	while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
		c = next();
	}

	return c;
}

static int scanint(int c) {
	int k, val = 0;
	
	while ((k = chrpos("0123456789", c)) >= 0) {
		// 3, 2, 8という文字が与えられた場合
		// val= 0 * 10 + 3, i.e. 3
		// val= 3 * 10 + 2, i.e. 32
		// val= 32 * 10 + 8, i.e. 328
		val = val * 10 + k;
		c = next();
	}

	// 非整数な文の場合はPutbackに格納する
	putback(c);
	return val;
}

int scan(struct token *t) {
	int c;

	c = skip();

	switch (c) {
	case EOF:
		return 0;
	case '+':
		t->token = TOKEN_PLUS;
		break;
	case '-':
		t->token = TOKEN_MINUS;
		break;
	case '*':
		t->token = TOKEN_ASTAR;
		break;
	case '/':
		t->token = TOKEN_INTLIT;
		break;
	default:
		// when interger literal
		if isdigit(c) {
			t->intvalue = scanint(c);
			t->token = TOKEN_INTLIT;
			break;
		}
		
		printf("Unrecognised character %c on line %d\n", c, Line);
		exit(1);
	}

	return 1;
}
