#include "token.h"
#include "symbol.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// 1が空き、0が割り当て済み

// 使用するレジスタの種類
// https://www.mztn.org/lxasm64/amd04.html
static int freereg[4];
// 汎用レジスタであるr0、r1、r2、r3に対して動作
// CPUアーキテクチャから独立している
static char *reglist[4] = { "%r8", "%r9", "%r10", "%r11" };

// 全てのレジスタを使用可能にする
void freeall_registers(void) {
	freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// 空いてるレジスタを割り当て、そのレジスタの値を返す
// 利用可能なレジスタがない場合は終了
static int alloc_register(void) {
	for (int i = 0; i < 4; i++) {
		if (freereg[i]) { // !=0
			freereg[i] = 0;
			return i;
		}
	}
	fatal("out of registers");
}

// 使用可能なレジスタのリストにレジスタを戻す
static void free_register(int reg) {
	// 割り当て済みでないか確認
	if (freereg[reg] != 0) {
		fatald("error trying to free register", reg);
	}
	freereg[reg] = 1;
}

// asmの開始部分を定義
void cgPreamble(void) {
	freeall_registers();
 	fputs(
 		"\t.text\n"
 	  	".LC0:\n"
 	  	"\t.string\t\"%d\\n\"\n"
 	  	"printint:\n"
 	  	"\tpushq\t%rbp\n"
 	  	"\tmovq\t%rsp, %rbp\n"
 	  	"\tsubq\t$16, %rsp\n"
 	  	"\tmovl\t%edi, -4(%rbp)\n"
 	  	"\tmovl\t-4(%rbp), %eax\n"
 	  	"\tmovl\t%eax, %esi\n"
 	  	"\tleaq	.LC0(%rip), %rdi\n"
 	  	"\tmovl	$0, %eax\n"
 	  	"\tcall	printf@PLT\n"
 	  	"\tnop\n"
 	  	"\tleave\n"
 	  	"\tret\n"
 	  	"\n"
 	  	"\t.globl\tmain\n"
 	  	"\t.type\tmain, @function\n"
 	  	"main:\n"
 	  	"\tpushq\t%rbp\n"
 	  	"\tmovq	%rsp, %rbp\n",
 	Outfile);
}

// asmの終了部分を定義
void cgPostamble(void) {
	fputs(
		"\tmovl	$0, %eax\n"
		"\tpopq	%rbp\n"
		"\tret\n",
  	Outfile);
}

void cgPrintInt(int r) {
	fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  	fprintf(Outfile, "\tcall\tprintint\n");
  	free_register(r);
}

// 整数リテラル値をレジスタにロード
// レジスタの番号を返す
int cgloadInt(int value) {
	// レジスタの確保
	int r = alloc_register();

	fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
	return r;
}

// グローバル変数から値を取得する
int cgloadGlob(char *identifier) {
	// レジスタの確保
	int r = alloc_register();

	// シンボルテーブルの値を初期化するためのコードを出力
	fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, reglist[r]);
	return r;
}

// シンボルテーブルの自動生成
void cgGlobSym(char *sym) {
	fprintf(Outfile, "\t.comm\t%s,8,8\n", sym);
}

// レジスタの値を変数に格納する
int cgstorGlob(int r, char *identifier) {
  fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], identifier);
  return r;
}

// 2つのレジスタを足し合わせて結果を格納したレジスタの番号を返す
int cgadd(int r1, int r2) {
	fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
	free_register(r1);
	return r2;
}

// 2つのレジスタを乗算した結果を格納したレジスタの番号を返す
int cgmul(int r1, int r2) {
	fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  	free_register(r1);
  	return r2;
}

// 2番目のレジスタは1番目のレジスタから減算されるので、1番目のレジスタを返し、2番目のレジスタを解放します。
int cgsub(int r1, int r2) {
	// r1 -= r2
  	fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  	free_register(r2);
  	return r1;
}

// まずr1(割られる数を)%raxをロードする必要がある
// その後にcqoで8バイトに拡張する必要がある
// idivqは%raxをr2の除数で割って商を%raxに残す
int cgdiv(int r1, int r2) {
  	fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  	fprintf(Outfile, "\tcqo\n");
  	fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
  	fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  	free_register(r2);
  	return r1;
}

static int cgcompare(int r1, int r2, char *how) {
  	fprintf(Outfile, "\tcmpq\t%s, %s\n", reglist[r2], reglist[r1]);
  	fprintf(Outfile, "\t%s\t%s\n", how, reglist[r2]);
  	fprintf(Outfile, "\tandq\t$255,%s\n", reglist[r2]);
  	free_register(r1);
  	return r2;
}

int cgequal(int r1, int r2) {
	return cgcompare(r1, r2, "sete");
}

int cgnotequal(int r1, int r2) {
	return cgcompare(r1, r2, "setne");
}

int cglessthan(int r1, int r2) {
	return cgcompare(r1, r2, "setl");
}

int cggreaterthan(int r1, int r2) {
	return cgcompare(r1, r2, "setg");
}

int cglessequal(int r1, int r2) {
	return cgcompare(r1, r2, "setle");
}

int cggreaterequal(int r1, int r2) {
	return cgcompare(r1, r2, "setge");
}