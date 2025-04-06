#pragma once

typedef enum {
	lxs_node_literal = 0,
	lxs_node_variable = 1,
	lxs_node_binary_op = 2,
	lxs_node_assignment = 3,
	lxs_node_if = 4,
	lxs_node_while = 5,
	lxs_node_func_call = 6,
	lxs_node_func_def = 7,
	lxs_node_ret = 8,
	lxs_node_program = 9
} lxs_node_type;

typedef struct lxs_ast_node {
	lxs_node_type type;

	struct lxs_ast_node* left;
	struct lxs_ast_node* right;
	
	char* identifier;
	int value;
	
	struct lxs_ast_node** body;
	int body_count;
	struct lxs_ast_node** params;
	int param_count;
} lxs_ast_node;

void lxs_print_ast_node(lxs_ast_node* node, int depth);
