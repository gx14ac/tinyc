#include "token.h"
#include "symbol.h"
#include "data.h"
#include "decl.h"

// 文字列s中の文字cのポインタを返す。cが見つからなければ-1を返す
static int chrpos(char *s, int c) {
	char *p;

	p = strchr(s, c);
	return (p ? p - s : -1);
}

// 文字列をすすめるか戻すかの処理
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

// 文字列を戻す
static int putback(int c) {
	Putback = c;
	return 0;
}

// 空白や改行を飛ばした後に処理する必要がある文字列を返す
static int skip(void) {
	int c;
	
	c = next();
	while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
		c = next();
	}

	return c;
}

// 英数字をバッファに読み込んで、英数字以外の文字にぶつかるまで読み込む
static int scanident(int c, char *buf, int lim) {
	int i = 0;

	// 数字、アルファベット、アンダースコアは許可する
	while (isalpha(c) || isdigit(c) || '_' == c) {
		// 識別子の長さ制限
		if (lim - 1 == i) {
			printf("identfier too long on line %d\n", Line);
			exit(1);
		} else if (i < lim - 1) {
			buf[i++] = c;
		}
		c = next();
	}

	// 有効でない文字に当たったので元に戻す
  	// buf[]をNUL終端し、長さを返す. これで有効な文字列と有効でない文字列を分けることができる
	putback(c);
	buf[i] = '\0';
	return i;
}

static int keyword(char *s) {
	switch (*s) {
		case 'p':
			if (!strcmp(s, "print")) {
				return TOKEN_PRINT;
				break;
			}
		case 'i':
			if (!strcmp(s, "int")) {
				return TOKEN_INT;
				break;
			}
	}
	return 0;
}

// 10進数文字の時にスキャンした文字列を整数で返す
static int scan_int(int c) {
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

// INFILEの文字列を順番にスキャンする
int scan(struct token *t) {
	int c;
	int tokentype;

	c = skip();

	switch (c) {
	case EOF:
		t->token = TOKEN_EOF;
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
		t->token = TOKEN_SLASH;
		break;
	case ';':
		t->token = TOKEN_SEMI;
		break;
	case '=':
		if ((c = next()) == '=') {
			t->token = TOKEN_EQUALS;
		} else {
			putback(c);
			t->token = TOKEN_ASSIGN;
		}
		break;
	case '!':
		if ((c = next()) == '=') {
			t->token = TOKEN_NE;
		} else {
			fatalc("Unrecognised character", c);
		}
		break;
	case '<':
		if ((c = next()) == '=') {
			t->token = TOKEN_LE;
		} else {
			putback(c);
			t->token = TOKEN_LT;
		}
		break;
	case '>':
		if ((c = next()) == '=') {
			t->token = TOKEN_GE;
		} else {
			putback(c);
			t->token = TOKEN_GT;
		}
		break;
	default:
		// when interger literal
		if (isdigit(c)) {
			t->intvalue = scan_int(c);
			t->token = TOKEN_INTLIT;
			break;
		} else if (isalpha(c) || '_' == c) {
			// キーワードや識別子を読み込む
			scanident(c, Text, TEXTLEN);

			// 登録されているキーワードなら返す
			if ((tokentype = keyword(Text))) {
				t->token = tokentype;
				break;
			}
			// Textがkeywordで無い場合は識別子として認識して返す
			t->token = TOKEN_IDENT;
			break;
		}
		// どのトークンにも当てはまらない場合
		fatalc("Unrecognised character", c);
	}

	return 1;
}