#include "ast_node.h"

#include <stdio.h>

void lxs_print_ast_node(lxs_ast_node* node, int depth) {
	if (!node) return;

	for (int i = 0; i < depth; ++i) printf("\t");

	switch (node->type) {
		case lxs_node_literal:
			printf("Literal > %d\n", node->value);
			break;
		
		case lxs_node_variable:
			printf("Variable > %s\n", node->identifier);
			break;
		
		case lxs_node_assignment:
			printf("Assignment > %s\n", node->identifier);
			lxs_print_ast_node(node->left, depth+1);
			lxs_print_ast_node(node->right, depth+1);
			break;
		
		case lxs_node_binary_op:
			printf("Binary Op > %s\n", node->identifier);
			lxs_print_ast_node(node->left, depth+1);
			lxs_print_ast_node(node->right, depth+1);
			break;
		
		case lxs_node_if:
			printf("If Condition >\n");
			lxs_print_ast_node(node->left, depth+1);
			printf("Then >\n");
			for (int i = 0; i < node->body_count; ++i) {
				lxs_print_ast_node(node->body[i], depth+1);
			}
			break;

		case lxs_node_ret:
			printf("Return >\n");
			lxs_print_ast_node(node->left, depth+1);
			break;

		default:
			break;
	}
}
