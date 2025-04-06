#pragma once

#include "ast_node.h"

typedef struct {
	char* name;
	int value;
} lxs_variable;

typedef struct {
	char* name;

	// if is cfunc, cast to void* and call
	// if else, interpret function as lxscript
	lxs_ast_node* function_body;
	char is_cfunc;

	lxs_ast_node** parameters;
	int param_count;
} lxs_function;

typedef struct {
	lxs_variable* variables;
	int var_count;
	lxs_function* functions;
	int func_count;
} lxs_context;

int lxs_eval_expression(lxs_context*, lxs_ast_node*);

void lxs_execute_statement(lxs_context*, lxs_ast_node*);
void lxs_execute_function(lxs_context*, lxs_ast_node*);

void lxs_execute_program(lxs_context*, lxs_ast_node* program);
