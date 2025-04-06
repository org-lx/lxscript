#include "parser.h"
#include "str_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void lxs_init_parser(lxs_parser* p, lxs_lexer* l) {
	p->lexer = l;
	p->current = lxs_next_token(l);
}

lxs_token lxs_consume_parser(lxs_parser* p, lxs_token_type expected) {
	if (p->current.token_type == expected) {
		lxs_token consumed = p->current;
		p->current = lxs_next_token(p->lexer);
		return consumed;
	}

	fprintf(
			stderr,
			"Syntax Error: Unexpected Token '%.*s'\n",
			p->current.length,
			p->current.start);
	exit(1);
}


lxs_ast_node* lxs_parse_literal(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
	node->type = lxs_node_literal;
	node->left = node->right = NULL;

	if (p->current.token_type == lxs_token_number) {
		node->value = atoi(p->current.start);
		lxs_consume_parser(p, lxs_token_number);
	} else {
		printf("Syntax Error: Expected a number\n");
	}

	return node;
}

lxs_ast_node* lxs_parse_identifier(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
	node->type = lxs_node_variable;
	node->identifier = lxs_strndup(p->current.start, p->current.length);
	node->left = node->right = NULL;

	lxs_consume_parser(p, lxs_token_identifier);

	if (p->current.token_type == lxs_token_lparen) {
		node->type = lxs_node_func_call;
		lxs_consume_parser(p, lxs_token_lparen);
		lxs_consume_parser(p, lxs_token_rparen);
	}
	
	return node;
}

lxs_ast_node* lxs_parse_expression(lxs_parser* p) {
	return lxs_parse_binary_op(p);
}

lxs_ast_node* lxs_parse_term(lxs_parser* p) {
	if (p->current.token_type == lxs_token_number) {
		return lxs_parse_literal(p);
	} else if (p->current.token_type == lxs_token_identifier) {
		return lxs_parse_identifier(p);
	}

	fprintf(stderr, "Syntax Error: Unexpected token in expression\n");
	exit(1);
}

lxs_ast_node* lxs_parse_binary_op(lxs_parser* p) {
	lxs_ast_node* left = lxs_parse_term(p);

	while (p->current.token_type == lxs_token_plus ||
			 p->current.token_type == lxs_token_minus ||
			 p->current.token_type == lxs_token_star ||
			 p->current.token_type == lxs_token_slash ||
			 p->current.token_type == lxs_token_gt ||
			 p->current.token_type == lxs_token_geq ||
			 p->current.token_type == lxs_token_lt ||
			 p->current.token_type == lxs_token_leq ||
			 p->current.token_type == lxs_token_eq ||
			 p->current.token_type == lxs_token_neq
		) {
		lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
		node->type = lxs_node_binary_op;
		node->left = left;
		node->right = NULL;

		node->identifier = lxs_strndup(p->current.start, p->current.length);
		lxs_consume_parser(p, p->current.token_type);
		node->right = lxs_parse_term(p);
		left = node;
	}

	return left;
}

lxs_ast_node* lxs_parse_assignment(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));

	node->type = lxs_node_assignment;
	node->identifier = lxs_strndup(p->current.start, p->current.length);
	node->left = lxs_parse_identifier(p);

	lxs_consume_parser(p, lxs_token_assign);

	node->right = lxs_parse_expression(p);
	lxs_consume_parser(p, lxs_token_semicolon);

	return node;
}

lxs_ast_node* lxs_parse_if(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
	node->type = lxs_node_if;

	lxs_consume_parser(p, lxs_token_if);
	lxs_consume_parser(p, lxs_token_lparen);
	node->left = lxs_parse_expression(p);
	lxs_consume_parser(p, lxs_token_rparen);

	lxs_consume_parser(p, lxs_token_lbrace);

	node->body_count = 0;
	node->body = NULL;

	while (p->current.token_type != lxs_token_rbrace) {
		lxs_ast_node* stmt = lxs_parse_statement(p);
		node->body_count++;
		node->body = realloc(node->body, node->body_count*sizeof(lxs_ast_node*));
		node->body[node->body_count-1] = stmt;
	}

	lxs_consume_parser(p, lxs_token_rbrace);

	return node;
}

lxs_ast_node* lxs_parse_while(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
	node->type = lxs_node_while;

	lxs_consume_parser(p, lxs_token_if);
	lxs_consume_parser(p, lxs_token_lparen);
	node->left = lxs_parse_expression(p);
	lxs_consume_parser(p, lxs_token_rparen);

	lxs_consume_parser(p, lxs_token_lbrace);

	node->body_count = 0;
	node->body = NULL;

	while (p->current.token_type != lxs_token_rbrace) {
		lxs_ast_node* stmt = lxs_parse_statement(p);
		node->body_count++;
		node->body = realloc(node->body, node->body_count*sizeof(lxs_ast_node*));
		node->body[node->body_count-1] = stmt;
	}

	lxs_consume_parser(p, lxs_token_rbrace);

	return node;
}

lxs_ast_node* lxs_parse_function(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
	node->type = lxs_node_func_def;
	node->left = NULL;

	lxs_consume_parser(p, lxs_token_function);
	node->identifier = lxs_strndup(p->current.start, p->current.length);
	lxs_consume_parser(p, lxs_token_identifier);

	// arguments TODO
	lxs_consume_parser(p, lxs_token_lparen);
	node->param_count = 0;
	node->params = NULL;

	if (p->current.token_type != lxs_token_rparen) {
		do {
			lxs_ast_node* param = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
			param->type = lxs_node_variable;
			node->identifier = lxs_strndup(p->current.start, p->current.length);
			lxs_consume_parser(p, lxs_token_identifier);

			node->param_count++;
			node->params = realloc(node->params, node->param_count * sizeof(lxs_ast_node*));
			node->params[node->param_count-1] = param;
		} while (p->current.token_type == lxs_token_comma &&
					lxs_consume_parser(p, lxs_token_comma).token_type);
	}

	lxs_consume_parser(p, lxs_token_rparen);

	// func implementation
	lxs_consume_parser(p, lxs_token_lbrace);

	node->body_count = 0;
	node->body = NULL;

	while (p->current.token_type != lxs_token_rbrace) {
		lxs_ast_node* stmt = lxs_parse_statement(p);
		node->body_count++;
		node->body = realloc(node->body, node->body_count*sizeof(lxs_ast_node*));
		node->body[node->body_count-1] = stmt;
	}

	lxs_consume_parser(p, lxs_token_rbrace);
	return node;
}

lxs_ast_node* lxs_parse_return(lxs_parser* p) {
	lxs_ast_node* node = (lxs_ast_node*)malloc(sizeof(lxs_ast_node));
	node->type = lxs_node_ret;

	lxs_consume_parser(p, lxs_token_return);
	node->left = lxs_parse_expression(p);
	lxs_consume_parser(p, lxs_token_semicolon);

	return node;
}

lxs_ast_node* lxs_parse_statement(lxs_parser* p) {
	if (p->current.token_type == lxs_token_if) return lxs_parse_if(p);
	if (p->current.token_type == lxs_token_while) return lxs_parse_while(p);
	if (p->current.token_type == lxs_token_function) return lxs_parse_function(p);
	if (p->current.token_type == lxs_token_return) return lxs_parse_return(p);
	if (p->current.token_type == lxs_token_identifier) {
		if ((p->lexer->current[0]) == '(') return lxs_parse_expression(p);
		return lxs_parse_assignment(p);
	}

   printf("Syntax Error: Unexpected statement\n");
   exit(1);
}

/*
lxs_ast_node* lxs_parse_program(lxs_parser* p) {
	lxs_ast_node* program = malloc(sizeof(lxs_ast_node));
	program->type = lxs_node_program;
	program->body = NULL;
	program->body_count = 0;

	while (p->current.token_type != lxs_token_eof) {
		lxs_ast_node* stmt = lxs_parse_statement(p);
		if (stmt) {
			program->body_count++;
			program->body = realloc(program->body, program->body_count * sizeof(lxs_ast_node*));
			program->body[program->body_count-1] = stmt;
		}
	}

	return program;
}*/

lxs_ast_node* lxs_parse_program(lxs_parser* p) {
	lxs_ast_node* program = malloc(sizeof(lxs_ast_node));
	program->type = lxs_node_program;
	program->body = NULL;
	program->body_count = 0;

	printf("[DEBUG] Starting program parsing...\n");

	while (p->current.token_type != lxs_token_eof) {
		printf("[DEBUG] Parsing statement at token: %d\n", p->current.token_type);

		lxs_ast_node* stmt = lxs_parse_statement(p);
		if (stmt) {
			printf("[DEBUG] Parsed statement of type: %d\n", stmt->type);

			program->body_count++;
			lxs_ast_node** new_body = realloc(program->body, program->body_count * sizeof(lxs_ast_node*));
			if (!new_body) {
				fprintf(stderr, "[ERROR] Failed to allocate memory for program body.\n");
				exit(1);
			}
			program->body = new_body;
			program->body[program->body_count - 1] = stmt;
		} else {
			printf("[WARNING] Encountered NULL statement, skipping...\n");
		}
	}

	printf("[DEBUG] Finished program parsing. Total statements: %d\n", program->body_count);
	return program;
}
