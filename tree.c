#include "token.h"
#include "data.h"
#include "ast.h"
#include "decl.h"

// build and return generic ast node
// ASTノードの4つのフィールドすべてに値をとる
// ノードを確保、フィールドの値を入力、そのノードへのポインタを返す
struct ASTnode *new_ast_node(int op, struct ASTnode *left,
						   struct ASTnode *right, int intvalue) {
	struct ASTnode *node;

	node = (struct ASTnode *) malloc(sizeof(struct ASTnode));
	if (node == NULL) {
		fprintf(stderr, "Unable to malloc in new_ast_node\n");
		exit(1);
	}

	node->op = op;
	node->left = left;
	node->right = right;
	node->intvalue = intvalue;

	return node;
}

// 葉のASTノード（子を持たないノード）を作る関数
struct ASTnode *create_ast_leaf(int op, int intvalue) {
	return new_ast_node(op, NULL, NULL, intvalue);
}

// 子を1つ持つASTノードを作る関数
struct ASTnode *create_ast_unary(int op, struct ASTnode *left, int intvalue) {
	return new_ast_node(op, left, NULL, intvalue);
}
