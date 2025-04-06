#include "context.h"
#include "str_utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int lxs_eval_expression(lxs_context* ctx, lxs_ast_node* node) {
	if (!node) return 0;

	switch (node->type) {
		case lxs_node_literal:
			printf("[DEBUG] Evaluating literal: %d\n", node->value);
			return node->value;

		case lxs_node_variable:
			for (int i = 0; i < ctx->var_count; i++) {
            if (strcmp(ctx->variables[i].name, node->identifier) == 0) {
                printf("[DEBUG] Fetching variable %s = %d\n", node->identifier, ctx->variables[i].value);
                return ctx->variables[i].value;
            }
        }
        printf("[ERROR] Undefined variable: %s\n", node->identifier);
        exit(1);

		case lxs_node_binary_op:
			int left = lxs_eval_expression(ctx, node->left);
			int right = lxs_eval_expression(ctx, node->right);

			printf("[DEBUG] Evaluating binary op: %d %s %d\n", left, node->identifier, right);

			if (!strcmp(node->identifier, "+")) return left + right;
			if (!strcmp(node->identifier, "-")) return left - right;
			if (!strcmp(node->identifier, "*")) return left * right;
			if (!strcmp(node->identifier, "/")) return left / right;
			if (!strcmp(node->identifier, ">")) return left > right;
			if (!strcmp(node->identifier, "<")) return left < right;
			if (!strcmp(node->identifier, "==")) return left == right;
			if (!strcmp(node->identifier, "!=")) return left != right;
			if (!strcmp(node->identifier, ">=")) return left >= right;
			if (!strcmp(node->identifier, "<=")) return left <= right;
			break;
	}

	return 0;
}

void lxs_execute_statement(lxs_context* ctx, lxs_ast_node* node) {
	if (!node) return;

	switch (node->type) {
		case lxs_node_assignment:
			for (int i = 0; i < ctx->var_count; ++i) {
				if (!strcmp(ctx->variables[i].name, node->identifier)) {
					ctx->variables[i].value = lxs_eval_expression(ctx, node->left);
					return;
				}
			}

			ctx->var_count++;
			ctx->variables = realloc(ctx->variables, ctx->var_count * sizeof(lxs_variable));
			ctx->variables[ctx->var_count-1].name = lxs_strndup(node->identifier, strlen(node->identifier));
			ctx->variables[ctx->var_count-1].value = lxs_eval_expression(ctx, node->left);
			return;

		case lxs_node_if:
			if (lxs_eval_expression(ctx, node->left)) {
				for (int i = 0; i < node->body_count; ++i) {
					lxs_execute_statement(ctx, node->body[i]);
				}
			}
			return;

		case lxs_node_ret:
			int result = lxs_eval_expression(ctx, node->left);
			exit(result);
	}
}

void lxs_execute_function(lxs_context* ctx, lxs_ast_node* node) {
	lxs_function func;
	func.is_cfunc = 0;
	func.name = lxs_strndup(node->identifier, strlen(node->identifier));
	func.function_body = node;
	func.parameters = node->params;
	func.param_count = node->param_count;

	ctx->func_count++;
	ctx->functions = realloc(ctx->functions, ctx->func_count*sizeof(lxs_function));
	ctx->functions[ctx->func_count-1] = func;
}

void lxs_execute_program(lxs_context* ctx, lxs_ast_node* program) {
	for (int i = 0; i < program->body_count; ++i) {
		printf("Body Type > %d\n", program->body[i]->type);
		lxs_execute_statement(ctx, program->body[i]);
	}
}
