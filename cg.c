#include "token.h"
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
	fprintf(stderr, "out of registers!\n");
	exit(1);
}

// 使用可能なレジスタのリストにレジスタを戻す
static void free_register(int reg) {
	// 割り当て済みでないか確認
	if (freereg[reg] != 0) {
		fprintf(stderr, "error trying to free register %d\n", reg);
		exit(1);
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
int cgload(int value) {
	// 空いているレジスターを取得
	int r = alloc_register();

	fprintf(Outfile, "\tmovq\t$%d, %s\n", value, reglist[r]);
	
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
