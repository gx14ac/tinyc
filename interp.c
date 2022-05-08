#include "token.h"
#include "data.h"
#include "decl.h"
#include "ast.h"

// List of AST operators
static char *ASTop[] = { "+", "-", "*", "/" };

// 左側の部分木を解釈し、その値を取得する
// 次に右側の部分木を解釈し、その値を取得する
// 木の根元のノードで、2つの部分木の値に対して演算を行いを実行しこの値を返す。以下のようなツリーになる
/*
          +
         / \
        /   \
       /     \
      *       *
     / \     / \
    2   3   4   5
*/
// ASTが与えられたら、その中の演算子を解釈する。
// 解釈した後に最終的な値を返す。
int interpretAST(struct ASTnode *n) {
	int leftVal, rightVal;

	if (n->left) {
		leftVal = interpretAST(n->left);
	}
	if (n->right) {
		rightVal = interpretAST(n->right);
	}

  	// Debug: Print what we are about to do
  	//	if (n->op == AST_INTLIT) {
  	//	  printf("int %d\n", n->intvalue);
	//	} else {
  	//	  printf("%d %s %d\n", leftVal, ASTop[n->op], rightVal);
	//	}

	switch (n->op) {
		case AST_ADD: // '+'
			return leftVal + rightVal;
		case AST_SUBSTRACT: // '-'
			return leftVal - rightVal;
		case AST_MULTIPLY: // '*'
			return leftVal * rightVal;
		case AST_DIVIDE: // '/'
			return leftVal / rightVal;
		case AST_INTLIT: // interger literal
			return n->intvalue;
		default:
			fprintf(stderr, "unknown ast operator %d\n", n->op);
			exit(1);
	}
}
